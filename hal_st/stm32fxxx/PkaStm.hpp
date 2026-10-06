#ifndef HAL_PKA_STM_HPP
#define HAL_PKA_STM_HPP

#include "hal_st/cortex/InterruptCortex.hpp"
#include "infra/event/ClaimableResource.hpp"
#include "infra/util/ByteRange.hpp"
#include "infra/util/Function.hpp"
#include "infra/util/InterfaceConnector.hpp"
#include <cstdint>

namespace hal
{
    // Single owner of the PKA peripheral and its interrupt, shared by all users through a claimable resource
    class PkaStm
        : public infra::InterfaceConnector<PkaStm>
    {
    public:
        struct Curve
        {
            infra::ConstByteRange p;
            infra::ConstByteRange absA;
            bool aIsPositive;
            infra::ConstByteRange b;
            infra::ConstByteRange gX;
            infra::ConstByteRange gY;
            infra::ConstByteRange n;
            infra::ConstByteRange montgomeryParameter;
        };

        enum class ComparisonResult : uint8_t
        {
            aEqualsB,
            aGreaterThanB,
            aLessThanB
        };

        PkaStm();
        PkaStm(const PkaStm& other) = delete;
        PkaStm& operator=(const PkaStm& other) = delete;
        ~PkaStm();

        // Operations may only be started while holding a claim on this resource
        infra::ClaimableResource& Resource();

        // Operands are big-endian byte strings that are only used during the call; result ranges must stay valid until onDone,
        // which is called from the event dispatcher
        void ScalarMultiplication(const Curve& curve, infra::ConstByteRange scalar, infra::ConstByteRange x, infra::ConstByteRange y, infra::ByteRange resultX, infra::ByteRange resultY, const infra::Function<void(bool success)>& onDone);
        void CheckPointOnCurve(const Curve& curve, infra::ConstByteRange x, infra::ConstByteRange y, const infra::Function<void(bool onCurve)>& onDone);
        // Fails when k results in r or s being 0; retry with another k
        void EcdsaSign(const Curve& curve, infra::ConstByteRange privateKey, infra::ConstByteRange k, infra::ConstByteRange hash, infra::ByteRange r, infra::ByteRange s, const infra::Function<void(bool success)>& onDone);
        void EcdsaVerify(const Curve& curve, infra::ConstByteRange publicKeyX, infra::ConstByteRange publicKeyY, infra::ConstByteRange r, infra::ConstByteRange s, infra::ConstByteRange hash, const infra::Function<void(bool valid)>& onDone);
        void Comparison(infra::ConstByteRange a, infra::ConstByteRange b, const infra::Function<void(ComparisonResult result)>& onDone);

    private:
        void Enable() const;
        void WriteValue(uint32_t index, uint32_t value) const;
        void WriteOperand(uint32_t index, infra::ConstByteRange operand) const;
        void ReadOperand(uint32_t index, infra::ByteRange operand) const;
        uint32_t ReadValue(uint32_t index) const;
        void Start(uint32_t mode, bool usesPrivateKey);
        void OnInterrupt();
        void Processed();
        void ClearRam() const;

    private:
        DispatchedInterruptHandler interrupt;
        infra::ClaimableResource resource;

        uint32_t mode = 0;
        bool busy = false;
        bool clearRamWhenDone = false;
        infra::ByteRange result1;
        infra::ByteRange result2;
        infra::Function<void(bool)> onBoolResult;
        infra::Function<void(ComparisonResult)> onComparisonResult;
    };
}

#endif
