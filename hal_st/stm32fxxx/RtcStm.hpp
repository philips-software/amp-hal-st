#ifndef HAL_RTC_STM_HPP
#define HAL_RTC_STM_HPP

#include DEVICE_HEADER

namespace hal
{
    class RtcBaseStm
    {
    public:
        struct Config
        {
            uint32_t hourFormat{ RTC_HOURFORMAT_24 };
            uint32_t asynchPrediv{ 127 };
            uint32_t synchPrediv{ 255 };
            uint32_t output{ RTC_OUTPUT_DISABLE };
            uint32_t outputPolarity{ RTC_OUTPUT_POLARITY_HIGH };
            uint32_t outputType{ RTC_OUTPUT_TYPE_OPENDRAIN };
        };

        explicit RtcBaseStm(const Config& config);

    private:
        RTC_HandleTypeDef handle{};
        const uint8_t rtcIndex = 0;

    public:
        auto& Handle()
        {
            return handle;
        }
    };
}
#endif // HAL_RTC_STM_HPP
