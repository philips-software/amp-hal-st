#ifndef SYNCHRONOUS_HAL_TIME_KEEPER_STM_HPP
#define SYNCHRONOUS_HAL_TIME_KEEPER_STM_HPP

#include "hal/synchronous_interfaces/TimeKeeper.hpp"
#include "hal/synchronous_interfaces/TimeService.hpp"
#include "hal_st/stm32fxxx/SystemTickTimerService.hpp"

namespace hal 
{
    class TimeKeeperSt 
        : public TimeKeeper 
    {
    public:
        explicit TimeKeeperSt(infra::Duration duration, hal::SystemTickTimerService& timeService);

        virtual bool Timeout() override;
        virtual void Reset() override;

    private:
        const infra::Duration duration;
        hal::SystemTickTimerService& timeService;
        infra::TimePoint endTime;
    };
}

#endif
