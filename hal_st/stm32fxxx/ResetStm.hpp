#ifndef HAL_RESET_STM_HPP
#define HAL_RESET_STM_HPP

#include "hal/interfaces/Reset.hpp"

namespace hal
{
    class ResetStm
        : public Reset
    {
    public:
        virtual void ResetModule(const char* resetReason) override;
    };
}

#endif
