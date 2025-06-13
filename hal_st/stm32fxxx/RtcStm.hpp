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
            uint32_t hourFormat{RTC_HOURFORMAT_24};
            uint32_t asynchPrediv{127};
            uint32_t synchPrediv{255};
            uint32_t output{RTC_OUTPUT_DISABLE};
            uint32_t outputRemap{RTC_OUTPUT_REMAP_NONE};
            uint32_t outputPolarity{RTC_OUTPUT_POLARITY_HIGH};
            uint32_t outputType{RTC_OUTPUT_TYPE_OPENDRAIN};
            uint32_t outputPullUp{RTC_OUTPUT_PULLUP_NONE};
            uint32_t binMode{RTC_BINARY_NONE};
        };

        explicit RtcBaseStm(const Config& config);

    protected:
        RTC_HandleTypeDef handle_{};

    private:
        const uint8_t rtcIndex_ = 0;
    };
}
#endif // HAL_RTC_STM_HPP
