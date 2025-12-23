#ifndef INTEGRATION_TEST_FIRMWARE_SENDER_HPP
#define INTEGRATION_TEST_FIRMWARE_SENDER_HPP

#include "generated/echo/Testing.pb.hpp"
#include "infra/timer/Timer.hpp"
#include "integration_test/logic/Flash.hpp"
#include "services/tracer/Tracer.hpp"

namespace application
{
    class FirmwareSender
        : private testing::TesterObserver
    {
    public:
        FirmwareSender(const std::vector<uint8_t>& firmware, services::Echo& echo, services::Tracer& tracer);

        bool Done() const;

    private:
        void TestedModeSet(infra::BoundedConstString message) override;

        void ResetTested();
        void SetTestedInProgrammingMode();
        void UploadFirmware();

    private:
        std::vector<uint8_t> firmware;
        application::FlashProxy flash;
        testing::TesterProxy tester;
        services::Tracer& tracer;
        bool done = false;
        bool modeSet = false;

        infra::TimerSingleShot timeoutTimer{ std::chrono::minutes(2), [this]()
            {
                done = true;
            } };
    };
}

#endif
