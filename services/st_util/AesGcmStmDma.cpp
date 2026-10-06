#include "services/st_util/AesGcmStmDma.hpp"
#include "infra/util/ReallyAssert.hpp"
#include <utility>

namespace services
{
    AesGcmStmDma::AesGcmStmDma(hal::AesStmDma& engine)
        : engine(engine)
        , claimer(engine.Resource())
    {}

    AesGcmStmDma::~AesGcmStmDma()
    {
        volatile uint8_t* wipe = key.data();
        for (std::size_t i = 0; i != key.size(); ++i)
            wipe[i] = 0;
    }

    void AesGcmStmDma::SetEncryptKey(infra::ConstByteRange key)
    {
        SetKey(key, hal::AesStmDma::Direction::encrypt);
    }

    void AesGcmStmDma::SetDecryptKey(infra::ConstByteRange key)
    {
        SetKey(key, hal::AesStmDma::Direction::decrypt);
    }

    void AesGcmStmDma::Process(infra::ConstByteRange iv, infra::ByteRange data, infra::ByteRange mac, const infra::Function<void()>& onDone)
    {
        really_assert(this->onDone == nullptr && iv.size() == this->iv.size());

        infra::Copy(iv, infra::MakeRange(this->iv));
        this->data = data;
        this->mac = mac;
        this->onDone = onDone;

        claimer.Claim([this]()
            {
                engine.ProcessGcm(direction, infra::MakeRange(key), infra::MakeRange(this->iv), this->data, this->mac, [this]()
                    {
                        Processed();
                    });
            });
    }

    void AesGcmStmDma::SetKey(infra::ConstByteRange newKey, hal::AesStmDma::Direction newDirection)
    {
        really_assert(newKey.size() == key.size());

        infra::Copy(newKey, infra::MakeRange(key));
        direction = newDirection;
    }

    void AesGcmStmDma::Processed()
    {
        claimer.Release();
        data = infra::ByteRange();
        mac = infra::ByteRange();
        std::exchange(onDone, nullptr)();
    }
}
