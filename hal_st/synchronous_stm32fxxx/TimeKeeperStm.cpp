#include "hal_st/synchronous_stm32fxxx/TimeKeeperStm.hpp"

namespace hal 
{
    TimeKeeperSt::TimeKeeperSt(infra::Duration duration, hal::SystemTickTimerService& timeService)
        : duration(duration)
        , timeService(timeService) 
    {
        Reset();
    }

    bool TimeKeeperSt::Timeout()
    {
        return timeService.Now() > endTime;
    }

    void TimeKeeperSt::Reset()
    {
        endTime = timeService.Now() + duration;
    }
}
