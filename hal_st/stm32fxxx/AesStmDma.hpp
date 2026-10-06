#ifndef HAL_AES_STM_DMA_HPP
#define HAL_AES_STM_DMA_HPP

#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "infra/event/ClaimableResource.hpp"
#include "infra/util/ByteRange.hpp"
#include "infra/util/Function.hpp"
#include <array>
#include <cstdint>

namespace hal
{
    // AES-128-GCM without additional authenticated data on the AES peripheral.
    // Full blocks of large, word-aligned payloads are moved by DMA; everything else is processed by the CPU.
    class AesStmDma
    {
    public:
        static constexpr std::size_t keySize = 16;
        static constexpr std::size_t ivSize = 12;
        static constexpr std::size_t blockSize = 16;
        static constexpr std::size_t minimumDmaSize = 64;

        enum class Direction : uint8_t
        {
            encrypt,
            decrypt
        };

        AesStmDma(DmaStm::TransmitStream& inputStream, DmaStm::ReceiveStream& outputStream);
        AesStmDma(const AesStmDma& other) = delete;
        AesStmDma& operator=(const AesStmDma& other) = delete;
        ~AesStmDma();

        // ProcessGcm may only be called while holding a claim on this resource
        infra::ClaimableResource& Resource();

        // Key and iv are only used during the call; data and tag must stay valid until onDone, which is called from the event dispatcher
        void ProcessGcm(Direction direction, infra::ConstByteRange key, infra::ConstByteRange iv, infra::ByteRange data, infra::ByteRange tag, const infra::Function<void()>& onDone);

    private:
        void Reset() const;
        void LoadKey(infra::ConstByteRange key) const;
        void LoadIv(infra::ConstByteRange iv) const;
        void SetPhase(uint32_t phase) const;
        void WaitForComputationComplete() const;
        void ProcessBlock(std::array<uint32_t, 4>& block) const;
        void ProcessBlocksWithCpu(infra::ByteRange blocks) const;
        void PayloadTransferred();
        void FinishPayload();
        void ProcessLastPartialBlock() const;
        void ComputeTag() const;

    private:
        TransmitDmaChannel inputDma;
        ReceiveDmaChannel outputDma;
        infra::ClaimableResource resource;

        Direction direction = Direction::encrypt;
        infra::ByteRange data;
        infra::ByteRange tag;
        infra::Function<void()> onDone;
    };
}

#endif
