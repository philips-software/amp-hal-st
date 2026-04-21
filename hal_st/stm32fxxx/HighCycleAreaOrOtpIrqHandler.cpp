#include "hal_st/stm32fxxx/HighCycleAreaOrOtpIrqHandler.hpp"
#include "infra/util/ReallyAssert.hpp"
#include DEVICE_HEADER

namespace hal
{
    HighCycleAreaOrOtpIrqHandler::HighCycleAreaOrOtpIrqHandler()
        : nmi{
            IRQn_Type::NonMaskableInt_IRQn, []
            {
                // From RM0481 Rev 4:
                // Section 7.3.4 FLASH read operations:
                // If the application reads an OTP data or flash high-cycle data not previously written, a
                // double ECC error is reported and only a word full of set bits is returned (see
                // Section (fixed:) 7.9.10 for details). The read data (in 16 bits) is stored in FLASH_ECCDR
                // register, so that the user can identify if the double ECC error is due to a virgin data or a
                // real ECC error.
                really_assert(__HAL_FLASH_GET_FLAG(FLASH_FLAG_ECCD) && READ_REG(FLASH->ECCDR) == 0xFFFF);

                __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ECCD);
            }
        }
    {}
}
