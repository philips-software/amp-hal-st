#include "integration_test/flasher/FirmwareSender.hpp"
#include "infra/timer/Waiting.hpp"

namespace
{
    const std::array<uint32_t, 12> stm32f767SectorSizes{ { 32 * 1024,
        32 * 1024,
        32 * 1024,
        32 * 1024,
        128 * 1024,
        256 * 1024,
        256 * 1024,
        256 * 1024,
        256 * 1024,
        256 * 1024,
        256 * 1024,
        256 * 1024 } };
}

namespace application
{
    FirmwareSender::FirmwareSender(const std::vector<uint8_t>& firmware, services::Echo& echo, services::Tracer& tracer)
        : testing::TesterObserver(echo)
        , firmware(firmware)
        , flash(echo, infra::MakeRange(stm32f767SectorSizes))
        , tester(echo)
        , tracer(tracer)
    {
        ResetTested();
        SetTestedInProgrammingMode();
        UploadFirmware();
    }

    bool FirmwareSender::Done() const
    {
        return done;
    }

    void FirmwareSender::TestedModeSet(infra::BoundedConstString message)
    {
        modeSet = true;
        MethodDone();
    }

    void FirmwareSender::ResetTested()
    {
        tester.RequestSend([this]()
            {
                tester.SetTestedMode(testing::TestedMode::reset);
            });

        infra::WaitFor([this]()
            {
                return modeSet;
            },
            infra::Duration::max());
        modeSet = false;
    }

    void FirmwareSender::SetTestedInProgrammingMode()
    {
        tester.RequestSend([this]()
            {
                tester.SetTestedMode(testing::TestedMode::programming);
            });
        infra::WaitFor([this]()
            {
                return modeSet;
            },
            infra::Duration::max());
    }

    void FirmwareSender::UploadFirmware()
    {
        tracer.Trace() << "Erasing chip...";
        flash.EraseAll([this]()
            {
                tracer.Trace() << "Writing firmware...";
                flash.WriteBuffer(infra::MakeRange(this->firmware), 0, [this]()
                    {
                        tracer.Trace() << "Uploading done";
                        done = true;
                    });
            });
    }
}
