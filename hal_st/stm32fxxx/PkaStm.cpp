#include "hal_st/stm32fxxx/PkaStm.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "infra/util/ReallyAssert.hpp"
#include <cstdlib>
#include <utility>

namespace hal
{
    namespace
    {
        constexpr uint32_t comparisonEqual = 0xED2C;
        constexpr uint32_t comparisonGreater = 0x7AF8;
        constexpr uint32_t errorFlags = PKA_SR_OPERRF | PKA_SR_RAMERRF | PKA_SR_ADDRERRF;

        PKA_TypeDef& Pka()
        {
            return *peripheralPka[0];
        }

        uint32_t NumberOfBits(infra::ConstByteRange operand)
        {
            return static_cast<uint32_t>(operand.size() * 8);
        }
    }

    PkaStm::PkaStm()
        : interrupt(peripheralPkaIrq[0], [this]()
              {
                  OnInterrupt();
              })
    {
        Enable();
    }

    PkaStm::~PkaStm()
    {
        Pka().CR &= ~PKA_CR_EN;
    }

    infra::ClaimableResource& PkaStm::Resource()
    {
        return resource;
    }

    void PkaStm::ScalarMultiplication(const Curve& curve, infra::ConstByteRange scalar, infra::ConstByteRange x, infra::ConstByteRange y, infra::ByteRange resultX, infra::ByteRange resultY, const infra::Function<void(bool success)>& onDone)
    {
        really_assert(resultX.size() == curve.p.size() && resultY.size() == curve.p.size());

        WriteValue(PKA_ECC_SCALAR_MUL_IN_EXP_NB_BITS, NumberOfBits(curve.n));
        WriteValue(PKA_ECC_SCALAR_MUL_IN_OP_NB_BITS, NumberOfBits(curve.p));
        WriteValue(PKA_ECC_SCALAR_MUL_IN_A_COEFF_SIGN, curve.aIsPositive ? 0 : 1);
        WriteOperand(PKA_ECC_SCALAR_MUL_IN_A_COEFF, curve.absA);
        WriteOperand(PKA_ECC_SCALAR_MUL_IN_B_COEFF, curve.b);
        WriteOperand(PKA_ECC_SCALAR_MUL_IN_MOD_GF, curve.p);
        WriteOperand(PKA_ECC_SCALAR_MUL_IN_K, scalar);
        WriteOperand(PKA_ECC_SCALAR_MUL_IN_INITIAL_POINT_X, x);
        WriteOperand(PKA_ECC_SCALAR_MUL_IN_INITIAL_POINT_Y, y);
        WriteOperand(PKA_ECC_SCALAR_MUL_IN_N_PRIME_ORDER, curve.n);

        result1 = resultX;
        result2 = resultY;
        onBoolResult = onDone;
        Start(PKA_MODE_ECC_MUL, true);
    }

    void PkaStm::CheckPointOnCurve(const Curve& curve, infra::ConstByteRange x, infra::ConstByteRange y, const infra::Function<void(bool onCurve)>& onDone)
    {
        WriteValue(PKA_POINT_CHECK_IN_MOD_NB_BITS, NumberOfBits(curve.p));
        WriteValue(PKA_POINT_CHECK_IN_A_COEFF_SIGN, curve.aIsPositive ? 0 : 1);
        WriteOperand(PKA_POINT_CHECK_IN_A_COEFF, curve.absA);
        WriteOperand(PKA_POINT_CHECK_IN_B_COEFF, curve.b);
        WriteOperand(PKA_POINT_CHECK_IN_MOD_GF, curve.p);
        WriteOperand(PKA_POINT_CHECK_IN_INITIAL_POINT_X, x);
        WriteOperand(PKA_POINT_CHECK_IN_INITIAL_POINT_Y, y);
        WriteOperand(PKA_POINT_CHECK_IN_MONTGOMERY_PARAM, curve.montgomeryParameter);

        onBoolResult = onDone;
        Start(PKA_MODE_POINT_CHECK, false);
    }

    void PkaStm::EcdsaSign(const Curve& curve, infra::ConstByteRange privateKey, infra::ConstByteRange k, infra::ConstByteRange hash, infra::ByteRange r, infra::ByteRange s, const infra::Function<void(bool success)>& onDone)
    {
        really_assert(r.size() == curve.n.size() && s.size() == curve.n.size());

        WriteValue(PKA_ECDSA_SIGN_IN_ORDER_NB_BITS, NumberOfBits(curve.n));
        WriteValue(PKA_ECDSA_SIGN_IN_MOD_NB_BITS, NumberOfBits(curve.p));
        WriteValue(PKA_ECDSA_SIGN_IN_A_COEFF_SIGN, curve.aIsPositive ? 0 : 1);
        WriteOperand(PKA_ECDSA_SIGN_IN_A_COEFF, curve.absA);
        WriteOperand(PKA_ECDSA_SIGN_IN_B_COEFF, curve.b);
        WriteOperand(PKA_ECDSA_SIGN_IN_MOD_GF, curve.p);
        WriteOperand(PKA_ECDSA_SIGN_IN_K, k);
        WriteOperand(PKA_ECDSA_SIGN_IN_INITIAL_POINT_X, curve.gX);
        WriteOperand(PKA_ECDSA_SIGN_IN_INITIAL_POINT_Y, curve.gY);
        WriteOperand(PKA_ECDSA_SIGN_IN_HASH_E, hash);
        WriteOperand(PKA_ECDSA_SIGN_IN_PRIVATE_KEY_D, privateKey);
        WriteOperand(PKA_ECDSA_SIGN_IN_ORDER_N, curve.n);

        result1 = r;
        result2 = s;
        onBoolResult = onDone;
        Start(PKA_MODE_ECDSA_SIGNATURE, true);
    }

    void PkaStm::EcdsaVerify(const Curve& curve, infra::ConstByteRange publicKeyX, infra::ConstByteRange publicKeyY, infra::ConstByteRange r, infra::ConstByteRange s, infra::ConstByteRange hash, const infra::Function<void(bool valid)>& onDone)
    {
        WriteValue(PKA_ECDSA_VERIF_IN_ORDER_NB_BITS, NumberOfBits(curve.n));
        WriteValue(PKA_ECDSA_VERIF_IN_MOD_NB_BITS, NumberOfBits(curve.p));
        WriteValue(PKA_ECDSA_VERIF_IN_A_COEFF_SIGN, curve.aIsPositive ? 0 : 1);
        WriteOperand(PKA_ECDSA_VERIF_IN_A_COEFF, curve.absA);
        WriteOperand(PKA_ECDSA_VERIF_IN_MOD_GF, curve.p);
        WriteOperand(PKA_ECDSA_VERIF_IN_INITIAL_POINT_X, curve.gX);
        WriteOperand(PKA_ECDSA_VERIF_IN_INITIAL_POINT_Y, curve.gY);
        WriteOperand(PKA_ECDSA_VERIF_IN_PUBLIC_KEY_POINT_X, publicKeyX);
        WriteOperand(PKA_ECDSA_VERIF_IN_PUBLIC_KEY_POINT_Y, publicKeyY);
        WriteOperand(PKA_ECDSA_VERIF_IN_SIGNATURE_R, r);
        WriteOperand(PKA_ECDSA_VERIF_IN_SIGNATURE_S, s);
        WriteOperand(PKA_ECDSA_VERIF_IN_HASH_E, hash);
        WriteOperand(PKA_ECDSA_VERIF_IN_ORDER_N, curve.n);

        onBoolResult = onDone;
        Start(PKA_MODE_ECDSA_VERIFICATION, false);
    }

    void PkaStm::Comparison(infra::ConstByteRange a, infra::ConstByteRange b, const infra::Function<void(ComparisonResult result)>& onDone)
    {
        WriteValue(PKA_COMPARISON_IN_OP_NB_BITS, NumberOfBits(a));
        WriteOperand(PKA_COMPARISON_IN_OP1, a);
        WriteOperand(PKA_COMPARISON_IN_OP2, b);

        onComparisonResult = onDone;
        Start(PKA_MODE_COMPARISON, false);
    }

    void PkaStm::Enable() const
    {
        // The PKA RAM is erased with the help of the RNG when the PKA gets enabled
        const bool rngWasEnabled = __HAL_RCC_RNG_IS_CLK_ENABLED();
        if (!rngWasEnabled)
            __HAL_RCC_RNG_CLK_ENABLE();

        EnableClockPka(0);

        while ((Pka().CR & PKA_CR_EN) == 0)
            Pka().CR = PKA_CR_EN;

        while ((Pka().SR & PKA_SR_INITOK) == 0)
            ;

        Pka().CLRFR = PKA_CLRFR_PROCENDFC | PKA_CLRFR_RAMERRFC | PKA_CLRFR_ADDRERRFC | PKA_CLRFR_OPERRFC;

        if (!rngWasEnabled)
            __HAL_RCC_RNG_CLK_DISABLE();
    }

    void PkaStm::WriteValue(uint32_t index, uint32_t value) const
    {
        Pka().RAM[index] = value;
    }

    void PkaStm::WriteOperand(uint32_t index, infra::ConstByteRange operand) const
    {
        const auto words = (operand.size() + 3) / 4;

        for (std::size_t word = 0; word != words; ++word)
        {
            uint32_t value = 0;
            for (std::size_t byte = 0; byte != 4; ++byte)
            {
                const auto position = word * 4 + byte;
                if (position < operand.size())
                    value |= static_cast<uint32_t>(operand[operand.size() - 1 - position]) << (8 * byte);
            }

            Pka().RAM[index + word] = value;
        }

        Pka().RAM[index + words] = 0;
        Pka().RAM[index + words + 1] = 0;
    }

    void PkaStm::ReadOperand(uint32_t index, infra::ByteRange operand) const
    {
        for (std::size_t position = 0; position != operand.size(); ++position)
            operand[operand.size() - 1 - position] = static_cast<uint8_t>(Pka().RAM[index + position / 4] >> (8 * (position % 4)));
    }

    uint32_t PkaStm::ReadValue(uint32_t index) const
    {
        return Pka().RAM[index];
    }

    void PkaStm::Start(uint32_t mode, bool usesPrivateKey)
    {
        really_assert(!busy);

        this->mode = mode;
        busy = true;
        clearRamWhenDone = usesPrivateKey;

        Pka().CLRFR = PKA_CLRFR_PROCENDFC;
        Pka().CR = (Pka().CR & ~PKA_CR_MODE) | (mode << PKA_CR_MODE_Pos) | PKA_CR_PROCENDIE | PKA_CR_RAMERRIE | PKA_CR_ADDRERRIE | PKA_CR_OPERRIE;
        Pka().CR |= PKA_CR_START;
    }

    void PkaStm::OnInterrupt()
    {
        really_assert((Pka().SR & errorFlags) == 0);

        // The PKA raises a second interrupt after the operation; only the first one, with PROCENDF set, completes the operation
        if (!busy || (Pka().SR & PKA_SR_PROCENDF) == 0)
            return;

        Pka().CLRFR = PKA_CLRFR_PROCENDFC;
        while ((Pka().SR & PKA_SR_BUSY) != 0)
            ;

        interrupt.ClearPending();
        Processed();
    }

    void PkaStm::Processed()
    {
        busy = false;

        bool success = true;
        ComparisonResult comparison = ComparisonResult::aLessThanB;

        switch (mode)
        {
            case PKA_MODE_ECC_MUL:
                success = ReadValue(PKA_ECC_SCALAR_MUL_OUT_ERROR) == PKA_NO_ERROR;
                if (success)
                {
                    ReadOperand(PKA_ECC_SCALAR_MUL_OUT_RESULT_X, result1);
                    ReadOperand(PKA_ECC_SCALAR_MUL_OUT_RESULT_Y, result2);
                }
                break;
            case PKA_MODE_POINT_CHECK:
                success = ReadValue(PKA_POINT_CHECK_OUT_ERROR) == PKA_NO_ERROR;
                break;
            case PKA_MODE_ECDSA_SIGNATURE:
                success = ReadValue(PKA_ECDSA_SIGN_OUT_ERROR) == PKA_NO_ERROR;
                if (success)
                {
                    ReadOperand(PKA_ECDSA_SIGN_OUT_SIGNATURE_R, result1);
                    ReadOperand(PKA_ECDSA_SIGN_OUT_SIGNATURE_S, result2);
                }
                break;
            case PKA_MODE_ECDSA_VERIFICATION:
                success = ReadValue(PKA_ECDSA_VERIF_OUT_RESULT) == PKA_NO_ERROR;
                break;
            case PKA_MODE_COMPARISON:
            {
                const auto value = ReadValue(PKA_COMPARISON_OUT_RESULT);
                if (value == comparisonEqual)
                    comparison = ComparisonResult::aEqualsB;
                else if (value == comparisonGreater)
                    comparison = ComparisonResult::aGreaterThanB;
                break;
            }
            default:
                std::abort();
        }

        if (clearRamWhenDone)
            ClearRam();

        result1 = infra::ByteRange();
        result2 = infra::ByteRange();

        if (mode == PKA_MODE_COMPARISON)
            std::exchange(onComparisonResult, nullptr)(comparison);
        else
            std::exchange(onBoolResult, nullptr)(success);
    }

    void PkaStm::ClearRam() const
    {
        for (auto& word : Pka().RAM)
            word = 0;
    }
}
