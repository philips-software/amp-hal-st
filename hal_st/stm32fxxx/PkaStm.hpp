#ifndef HAL_PKA_STM_HPP
#define HAL_PKA_STM_HPP

#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "hal_st/cortex/InterruptCortex.hpp"
#include "infra/util/Function.hpp"
#include "services/util/EllipticCurve.hpp"

#if defined(HAS_PERIPHERAL_PKA)

namespace hal
{
    class PkaStm
        : public services::EllipticCurveOperations
        , protected hal::DispatchedInterruptHandler
    {
    public:
        PkaStm();
        ~PkaStm();

        void ScalarMultiplication(const services::EllipticCurveExtendedParameters& curve, infra::ConstByteRange scalar, infra::ConstByteRange x, infra::ConstByteRange y, const MultiplicationResult& onDone) const override;
        void CheckPointOnCurve(const services::EllipticCurveExtendedParameters& curve, infra::ConstByteRange x, infra::ConstByteRange y, const infra::Function<void(PointOnCurveResult)>& onDone) const override;
        void Comparison(infra::ConstByteRange a, infra::ConstByteRange b, const infra::Function<void(ComparisonResult)>& onDone) const override;

    private:
        void Enable() const;
        void Disable() const;

        void Start(const infra::Function<void()>& onDone) const;
        void WriteInput(uint32_t index, uint32_t value) const;
        void WriteOperand(uint32_t index, infra::ConstByteRange operand) const;
        uint32_t ReadOutput(uint32_t index) const;
        void ReadResult(uint32_t index, infra::ByteRange operand) const;

    private:
        static constexpr std::size_t pkaIndex = 0;
        mutable MultiplicationResult onMultiplicationResult;
        mutable infra::Function<void(PointOnCurveResult)> onCheckPointOnCurveResult;
        mutable infra::Function<void(ComparisonResult)> onComparisonResult;
        mutable infra::Function<void()> onInterrupt;
        mutable std::array<uint8_t, 64> buffer;
    };
}

#endif
#endif
