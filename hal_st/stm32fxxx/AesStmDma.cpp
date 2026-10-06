#include "hal_st/stm32fxxx/AesStmDma.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "infra/util/ReallyAssert.hpp"
#include <cstring>
#include <limits>
#include <utility>

namespace hal
{
    namespace
    {
        constexpr uint32_t phaseInit = 0;
        constexpr uint32_t phasePayload = AES_CR_GCMPH_1;
        constexpr uint32_t phaseFinal = AES_CR_GCMPH_0 | AES_CR_GCMPH_1;
        constexpr uint32_t initialCounter = 2;

        AES_TypeDef& Aes()
        {
            return *peripheralAes[0];
        }

        uint32_t BigEndianWord(infra::ConstByteRange bytes, std::size_t wordIndex)
        {
            auto word = &bytes[wordIndex * sizeof(uint32_t)];
            return (static_cast<uint32_t>(word[0]) << 24) | (static_cast<uint32_t>(word[1]) << 16) | (static_cast<uint32_t>(word[2]) << 8) | word[3];
        }

        bool IsWordAligned(const void* address)
        {
            return reinterpret_cast<uintptr_t>(address) % sizeof(uint32_t) == 0;
        }

        void ClearComputationComplete()
        {
            Aes().ICR = AES_ICR_CCF;
        }
    }

    AesStmDma::AesStmDma(DmaStm::TransmitStream& inputStream, DmaStm::ReceiveStream& outputStream)
        : inputDma(inputStream, &peripheralAes[0]->DINR, sizeof(uint32_t), infra::emptyFunction, DmaStm::StreamInterruptHandler::immediate)
        , outputDma(outputStream, &peripheralAes[0]->DOUTR, sizeof(uint32_t), [this]()
              {
                  PayloadTransferred();
              })
    {
        EnableClockAes(0);
    }

    AesStmDma::~AesStmDma()
    {
        Reset();
    }

    infra::ClaimableResource& AesStmDma::Resource()
    {
        return resource;
    }

    void AesStmDma::ProcessGcm(Direction direction, infra::ConstByteRange key, infra::ConstByteRange iv, infra::ByteRange data, infra::ByteRange tag, const infra::Function<void()>& onDone)
    {
        really_assert(key.size() == keySize && iv.size() == ivSize && tag.size() == blockSize && this->onDone == nullptr);

        this->direction = direction;
        this->data = data;
        this->tag = tag;
        this->onDone = onDone;

        // The AES clock is shared with other users that may have turned it off or reset the peripheral
        EnableClockAes(0);
        Reset();
        Aes().CR = AES_CR_CHMOD_0 | AES_CR_CHMOD_1 | AES_CR_DATATYPE_1 | (direction == Direction::decrypt ? AES_CR_MODE_1 : 0);
        LoadKey(key);
        LoadIv(iv);

        SetPhase(phaseInit);
        WaitForComputationComplete();

        SetPhase(phasePayload);
        auto fullBlocks = infra::Head(data, data.size() - data.size() % blockSize);
        if (fullBlocks.size() >= minimumDmaSize && fullBlocks.size() <= std::numeric_limits<uint16_t>::max() && IsWordAligned(fullBlocks.begin()))
        {
            outputDma.StartReceive(infra::ReinterpretCastMemoryRange<uint32_t>(fullBlocks));
            inputDma.StartTransmit(infra::ReinterpretCastMemoryRange<const uint32_t>(fullBlocks));
            Aes().CR |= AES_CR_DMAINEN | AES_CR_DMAOUTEN;
        }
        else
        {
            ProcessBlocksWithCpu(fullBlocks);
            FinishPayload();
        }
    }

    void AesStmDma::Reset() const
    {
        Aes().CR &= ~AES_CR_EN;
        Aes().CR |= AES_CR_IPRST;
        Aes().CR = 0;
    }

    void AesStmDma::LoadKey(infra::ConstByteRange key) const
    {
        Aes().KEYR3 = BigEndianWord(key, 0);
        Aes().KEYR2 = BigEndianWord(key, 1);
        Aes().KEYR1 = BigEndianWord(key, 2);
        Aes().KEYR0 = BigEndianWord(key, 3);

        while ((Aes().SR & AES_SR_KEYVALID) == 0)
            ;
    }

    void AesStmDma::LoadIv(infra::ConstByteRange iv) const
    {
        Aes().IVR3 = BigEndianWord(iv, 0);
        Aes().IVR2 = BigEndianWord(iv, 1);
        Aes().IVR1 = BigEndianWord(iv, 2);
        Aes().IVR0 = initialCounter;
    }

    void AesStmDma::SetPhase(uint32_t phase) const
    {
        while ((Aes().SR & AES_SR_BUSY) != 0)
            ;

        Aes().CR = (Aes().CR & ~AES_CR_GCMPH) | phase;
        Aes().CR |= AES_CR_EN;
    }

    void AesStmDma::WaitForComputationComplete() const
    {
        while ((Aes().ISR & AES_ISR_CCF) == 0)
            ;

        ClearComputationComplete();
    }

    void AesStmDma::ProcessBlock(std::array<uint32_t, 4>& block) const
    {
        for (auto word : block)
            Aes().DINR = word;

        WaitForComputationComplete();

        for (auto& word : block)
            word = Aes().DOUTR;
    }

    void AesStmDma::ProcessBlocksWithCpu(infra::ByteRange blocks) const
    {
        std::array<uint32_t, 4> block;

        for (auto offset = 0u; offset != blocks.size(); offset += blockSize)
        {
            std::memcpy(block.data(), blocks.begin() + offset, blockSize);
            ProcessBlock(block);
            std::memcpy(blocks.begin() + offset, block.data(), blockSize);
        }
    }

    void AesStmDma::PayloadTransferred()
    {
        Aes().CR &= ~(AES_CR_DMAINEN | AES_CR_DMAOUTEN);
        ClearComputationComplete();
        FinishPayload();
    }

    void AesStmDma::FinishPayload()
    {
        ProcessLastPartialBlock();
        ComputeTag();
        Reset();

        data = infra::ByteRange();
        tag = infra::ByteRange();
        std::exchange(onDone, nullptr)();
    }

    void AesStmDma::ProcessLastPartialBlock() const
    {
        const auto remainder = data.size() % blockSize;
        if (remainder == 0)
            return;

        // Encryption must exclude the padding bytes from the tag; decryption hashes the zero-padded input itself
        if (direction == Direction::encrypt)
            Aes().CR = (Aes().CR & ~AES_CR_NPBLB) | ((blockSize - remainder) << AES_CR_NPBLB_Pos);

        std::array<uint32_t, 4> block{};
        auto lastBlock = infra::Tail(data, remainder);
        std::memcpy(block.data(), lastBlock.begin(), remainder);
        ProcessBlock(block);
        std::memcpy(lastBlock.begin(), block.data(), remainder);
    }

    void AesStmDma::ComputeTag() const
    {
        SetPhase(phaseFinal);

        const uint64_t payloadBits = static_cast<uint64_t>(data.size()) * 8;
        std::array<uint8_t, blockSize> lengthBlock{};
        for (auto i = 0u; i != sizeof(payloadBits); ++i)
            lengthBlock[blockSize - 1 - i] = static_cast<uint8_t>(payloadBits >> (8 * i));

        // Written as a byte stream like the payload, so that it gets the same byte swapping
        std::array<uint32_t, 4> block;
        std::memcpy(block.data(), lengthBlock.data(), blockSize);
        ProcessBlock(block);
        std::memcpy(tag.begin(), block.data(), blockSize);
    }
}
