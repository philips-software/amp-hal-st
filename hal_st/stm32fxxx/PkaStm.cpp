#include "hal_st/stm32fxxx/PkaStm.hpp"
#include "infra/util/ByteRange.hpp"
#include "infra/util/MemoryRange.hpp"
#include DEVICE_HEADER

#if defined(HAS_PERIPHERAL_PKA)

namespace
{
#if defined(STM32WBA)
    constexpr uint32_t pkaPointOnCurve = PKA_NO_ERROR;
    constexpr uint32_t pkaCompareResultAEqualsB = 0xED2C;
    constexpr uint32_t pkaCompareResultAGreaterThanB = 0x7AF8;
    constexpr uint32_t pkaInterruptStatusFlags = PKA_SR_PROCENDF | PKA_SR_OPERRF | PKA_SR_RAMERRF | PKA_SR_ADDRERRF | PKA_SR_BUSY;
#elif defined(STM32WB)
    constexpr uint32_t pkaPointOnCurve = 0;
    constexpr uint32_t pkaCompareResultAEqualsB = 0;
    constexpr uint32_t pkaCompareResultAGreaterThanB = 1;
    constexpr uint32_t pkaInterruptStatusFlags = PKA_SR_PROCENDF | PKA_SR_RAMERRF | PKA_SR_ADDRERRF | PKA_SR_BUSY;
#endif

    void SetMode(const std::size_t& index, uint32_t mode)
    {
        hal::peripheralPka[index]->CR = (hal::peripheralPka[index]->CR & ~PKA_CR_MODE) | (mode << PKA_CR_MODE_Pos);
    }

    uint32_t Revert(infra::MemoryRange<const std::byte> source, std::size_t size, uint32_t index)
    {
        return static_cast<uint32_t>(source[(size - (index * 4) - 1)]) | static_cast<uint32_t>(source[(size - (index * 4) - 2)]) << 8 | static_cast<uint32_t>(source[(size - (index * 4) - 3)]) << 16 | static_cast<uint32_t>(source[(size - (index * 4) - 4)]) << 24;
    }

    void CopyToPkaRam(infra::MemoryRange<volatile uint32_t> destination, infra::MemoryRange<const std::byte> source)
    {
        auto size = source.size();
        auto missingBytes = size % 4;
        auto index = 0;

        for (; index < (size / 4); ++index)
            destination[index] = Revert(source, size, index);

        if (missingBytes == 1)
            destination[index] = static_cast<uint32_t>(source[(size - (index * 4) - 1)]);

        if (missingBytes == 2)
            destination[index] = static_cast<uint32_t>(source[(size - (index * 4) - 1)]) | static_cast<uint32_t>(source[(size - (index * 4) - 2)]) << 8;

        if (missingBytes == 3)
            destination[index] = static_cast<uint32_t>(source[(size - (index * 4) - 1)]) | static_cast<uint32_t>(source[(size - (index * 4) - 2)]) << 8 | static_cast<uint32_t>(source[(size - (index * 4) - 3)]) << 16;
    }

    void CopyFromPkaRam(infra::MemoryRange<std::byte> destination, infra::MemoryRange<volatile uint32_t> source)
    {
        auto size = source.size();
        auto missingBytes = size % 4;
        auto index = 0;

        for (; index < (size / 4); ++index)
        {
            auto destinationIndex = size - 4 - (index * 4);
            destination[destinationIndex + 3] = static_cast<std::byte>((source[index] & 0x000000FF));
            destination[destinationIndex + 2] = static_cast<std::byte>((source[index] & 0x0000FF00) >> 8);
            destination[destinationIndex + 1] = static_cast<std::byte>((source[index] & 0x00FF0000) >> 16);
            destination[destinationIndex] = static_cast<std::byte>((source[index] & 0xFF000000) >> 24);
        }

        if (missingBytes == 1)
            destination[0] = static_cast<std::byte>((source[index] & 0x000000FF));

        if (missingBytes == 2)
        {
            destination[1] = static_cast<std::byte>((source[index] & 0x000000FF));
            destination[0] = static_cast<std::byte>((source[index] & 0x0000FF00) >> 8);
        }

        if (missingBytes == 3)
        {
            destination[2] = static_cast<std::byte>((source[index] & 0x000000FF));
            destination[1] = static_cast<std::byte>((source[index] & 0x0000FF00) >> 8);
            destination[0] = static_cast<std::byte>((source[index] & 0x00FF0000) >> 16);
        }
    }
}

namespace hal
{
    PkaStm::PkaStm()
        : DispatchedInterruptHandler(peripheralPkaIrq[pkaIndex], [this]()
              {
                  // Workaround: PKA triggers two interrupts: one when the operation is done and another one when the peripheral is released.
                  if (!(peripheralPka[pkaIndex]->SR & pkaInterruptStatusFlags))
                      this->onInterrupt();

                  if (peripheralPka[pkaIndex]->SR & PKA_SR_PROCENDF)
                      peripheralPka[pkaIndex]->CLRFR = PKA_CLRFR_PROCENDFC;
              })
    {
        Enable();
    }

    PkaStm::~PkaStm()
    {
        Disable();
    }

    void PkaStm::ScalarMultiplication(const services::EllipticCurveExtendedParameters& curve, infra::ConstByteRange scalar, infra::ConstByteRange x, infra::ConstByteRange y, const MultiplicationResult& onDone) const
    {
        onMultiplicationResult = onDone;

        WriteInput(PKA_ECC_SCALAR_MUL_IN_EXP_NB_BITS, curve.n.size() * 8);
        WriteInput(PKA_ECC_SCALAR_MUL_IN_OP_NB_BITS, curve.p.size() * 8);
        WriteInput(PKA_ECC_SCALAR_MUL_IN_A_COEFF_SIGN, curve.isAPositive ? 0 : 1);
        WriteOperand(PKA_ECC_SCALAR_MUL_IN_A_COEFF, curve.absA);
#if defined(PKA_ECC_SCALAR_MUL_IN_B_COEFF)
        WriteOperand(PKA_ECC_SCALAR_MUL_IN_B_COEFF, curve.b);
#endif
        WriteOperand(PKA_ECC_SCALAR_MUL_IN_MOD_GF, curve.p);
        WriteOperand(PKA_ECC_SCALAR_MUL_IN_K, scalar);
        WriteOperand(PKA_ECC_SCALAR_MUL_IN_INITIAL_POINT_X, !x.empty() ? x : curve.gX);
        WriteOperand(PKA_ECC_SCALAR_MUL_IN_INITIAL_POINT_Y, !y.empty() ? y : curve.gY);
#if defined(PKA_ECC_SCALAR_MUL_IN_N_PRIME_ORDER)
        WriteOperand(PKA_ECC_SCALAR_MUL_IN_N_PRIME_ORDER, curve.n);
#endif
        SetMode(pkaIndex, PKA_MODE_ECC_MUL);
        Start([this]()
            {
                auto range = infra::MakeRange(buffer);

                ReadResult(PKA_ECC_SCALAR_MUL_OUT_RESULT_X, infra::Head(range, range.size() / 2));
                ReadResult(PKA_ECC_SCALAR_MUL_OUT_RESULT_Y, infra::Tail(range, range.size() / 2));

                onMultiplicationResult(infra::Head(range, 32), infra::Tail(range, range.size() / 2));
            });
    }

    void PkaStm::CheckPointOnCurve(const services::EllipticCurveExtendedParameters& curve, infra::ConstByteRange x, infra::ConstByteRange y, const infra::Function<void(PointOnCurveResult)>& onDone) const
    {
        onCheckPointOnCurveResult = onDone;

        WriteInput(PKA_POINT_CHECK_IN_MOD_NB_BITS, curve.p.size() * 8);
        WriteInput(PKA_POINT_CHECK_IN_A_COEFF_SIGN, curve.isAPositive ? 0 : 1);
        WriteOperand(PKA_POINT_CHECK_IN_A_COEFF, curve.absA);
        WriteOperand(PKA_POINT_CHECK_IN_B_COEFF, curve.b);
        WriteOperand(PKA_POINT_CHECK_IN_MOD_GF, curve.p);
        WriteOperand(PKA_POINT_CHECK_IN_INITIAL_POINT_X, curve.gX);
        WriteOperand(PKA_POINT_CHECK_IN_INITIAL_POINT_Y, curve.gY);
#if defined(PKA_POINT_CHECK_IN_MONTGOMERY_PARAM)
        WriteOperand(PKA_POINT_CHECK_IN_MONTGOMERY_PARAM, curve.montgomeryR2);
#endif
        SetMode(pkaIndex, PKA_MODE_POINT_CHECK);
        Start([this]()
            {
                if (ReadOutput(PKA_POINT_CHECK_OUT_ERROR) == pkaPointOnCurve)
                    onCheckPointOnCurveResult(PointOnCurveResult::pointOnCurve);
                else
                    onCheckPointOnCurveResult(PointOnCurveResult::pointNotOnCurve);
            });
    }

    void PkaStm::Comparison(infra::ConstByteRange a, infra::ConstByteRange b, const infra::Function<void(ComparisonResult)>& onDone) const
    {
        onComparisonResult = onDone;

#if defined(PKA_COMPARISON_IN_OP_NB_BITS)
        WriteInput(PKA_COMPARISON_IN_OP_NB_BITS, a.size() * 8);
#elif defined(PKA_COMPARISON_NB_BITS)
        WriteInput(PKA_COMPARISON_NB_BITS, a.size() * 8);
#endif
        WriteOperand(PKA_COMPARISON_IN_OP1, a);
        WriteOperand(PKA_COMPARISON_IN_OP2, b);

        SetMode(pkaIndex, PKA_MODE_COMPARISON);
        Start([this]()
            {
                auto result = ReadOutput(PKA_COMPARISON_OUT_RESULT);

                if (result == pkaCompareResultAEqualsB)
                    onComparisonResult(ComparisonResult::aEqualsB);
                else if (result == pkaCompareResultAGreaterThanB)
                    onComparisonResult(ComparisonResult::aGreaterThanB);
                else
                    onComparisonResult(ComparisonResult::aLessThanB);
            });
    }

    void PkaStm::WriteInput(uint32_t index, uint32_t value) const
    {
        peripheralPka[pkaIndex]->RAM[index] = value;
    }

    void PkaStm::WriteOperand(uint32_t index, infra::ConstByteRange operand) const
    {
        auto source = infra::ReinterpretCastMemoryRange<const std::byte>(operand);
        auto destination = infra::MemoryRange<volatile uint32_t>(&peripheralPka[pkaIndex]->RAM[index], &peripheralPka[pkaIndex]->RAM[index] + operand.size());
        CopyToPkaRam(destination, source);
        peripheralPka[pkaIndex]->RAM[index + operand.size()] = 0;
    }

    uint32_t PkaStm::ReadOutput(uint32_t index) const
    {
        return peripheralPka[pkaIndex]->RAM[index];
    }

    void PkaStm::ReadResult(uint32_t index, infra::ByteRange operand) const
    {
        auto source = infra::MemoryRange<volatile uint32_t>(&peripheralPka[pkaIndex]->RAM[index], &peripheralPka[pkaIndex]->RAM[index] + operand.size());
        auto destination = infra::ReinterpretCastMemoryRange<std::byte>(operand);
        CopyFromPkaRam(destination, source);
    }

    void PkaStm::Start(const infra::Function<void()>& onDone) const
    {
        onInterrupt = onDone;
        peripheralPka[pkaIndex]->CLRFR |= PKA_CLRFR_PROCENDFC;
        peripheralPka[pkaIndex]->CR |= PKA_CR_START | PKA_CR_PROCENDIE;
    }

    void PkaStm::Enable() const
    {
        auto isRngEnabled = __HAL_RCC_RNG_IS_CLK_ENABLED();

        if (!isRngEnabled)
            EnableClockRng(0);

        EnableClockPka(pkaIndex);

        peripheralPka[pkaIndex]->CR |= PKA_CR_EN;

#if defined(PKA_SR_INITOK)
        while (!(peripheralPka[pkaIndex]->SR & PKA_SR_INITOK))
            ;
#endif

#if defined(PKA_CLRFR_OPERRFC)
        peripheralPka[pkaIndex]->CLRFR |= PKA_CLRFR_OPERRFC;
#endif
        peripheralPka[pkaIndex]->CLRFR |= PKA_CLRFR_PROCENDFC | PKA_CLRFR_RAMERRFC | PKA_CLRFR_ADDRERRFC;

        if (!isRngEnabled)
            DisableClockRng(0);
    }

    void PkaStm::Disable() const
    {
        peripheralPka[pkaIndex]->CR &= ~PKA_CR_EN;
        DisableClockPka(pkaIndex);
    }
}

#endif
