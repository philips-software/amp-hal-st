#ifndef SERVICES_ST_UTIL_FUS_WIRELESS_STACk_UPGRADE_HPP
#define SERVICES_ST_UTIL_FUS_WIRELESS_STACk_UPGRADE_HPP

#include <cstdint>

namespace hal
{
    class FusWirelessStackUpgrade
    {
    public:
        enum class ErrorCode : uint8_t
        {
            noError = 0x00,
            imgNotFound = 0x01,
            imgCorrupt = 0x02,
            imgNotAuthentic = 0x03,
            imgNotEnoughSpace = 0x04,
            imageUsrAbort = 0x05,
            imageErsError = 0x06,
            imageWrtError = 0x07,
            authTagStNotFound = 0x08,
            authTagCustNotFound = 0x09,
            authKeyLocked = 0x0A,
            fwRollbackError = 0x11,
            stateNotRunning = 0xFE,
            errUnknown = 0xFF,
        };

        enum class State : uint8_t
        {
            idle = 0x00,
            fwUpgrdOngoing = 0x10,
            fwUpgrdOngoingEnd = 0x1F, // All values between 0x10 and 0x1F have the same meaning
            fusUpgrdOngoing = 0x20,
            fusUpgrdOngoingEnd = 0x2F, // All values between 0x20 and 0x2F have the same meaning
            serviceOngoing = 0x30,
            serviceOngoingEnd = 0x3F, // All values between 0x30 and 0x3F have the same meaning
            error = 0xFF,
        };

        struct StateWithErrorCode
        {
            State state;
            ErrorCode errorCode;
        };

    public:
        virtual ~FusWirelessStackUpgrade() = default;

        virtual StateWithErrorCode GetFusState() const = 0;
        virtual void FirmwareUpgrade() = 0;
        virtual void StartWirelessStack() = 0;
        virtual void DeleteWirelessStack() = 0;
    };
}

#endif
