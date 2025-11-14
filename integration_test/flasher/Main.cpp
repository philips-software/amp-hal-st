#include "args.hxx"
#include "generated/echo/Testing.pb.hpp"
#include "generated/echo/TracingFlash.pb.hpp"
#include "generated/echo/TracingTesting.pb.hpp"
#include "hal/generic/FileSystemGeneric.hpp"
#include "hal/generic/TimerServiceGeneric.hpp"
#include "infra/timer/Waiting.hpp"
#include "integration_test/logic/Flash.hpp"
#include "services/network_instantiations/EchoInstantiation.hpp"
#include "services/network_instantiations/NetworkAdapter.hpp"
#include "services/tracer/GlobalTracer.hpp"
#include "services/tracer/TracerOnIoOutputInfrastructure.hpp"

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

class FirmwareSender
    : private testing::TesterObserver
{
public:
    FirmwareSender(const std::vector<uint8_t>& firmware, services::Echo& echo)
        : testing::TesterObserver(echo)
        , firmware(firmware)
        , flash(echo, infra::MakeRange(stm32f767SectorSizes))
        , tester(echo)
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

        tester.RequestSend([this]()
            {
                tester.SetTestedMode(testing::TestedMode::programming);
            });
        infra::WaitFor([this]()
            {
                return modeSet;
            },
            infra::Duration::max());

        services::GlobalTracer().Trace() << "Erasing chip...";
        flash.EraseAll([this]()
            {
                services::GlobalTracer().Trace() << "Writing firmware...";
                flash.WriteBuffer(infra::MakeRange(this->firmware), 0, [this]()
                    {
                        services::GlobalTracer().Trace() << "Uploading done";
                        done = true;
                    });
            });
    }

    bool Done() const
    {
        return done;
    }

private:
    void TestedModeSet(infra::BoundedConstString message) override
    {
        modeSet = true;
        MethodDone();
    }

private:
    std::vector<uint8_t> firmware;
    application::FlashProxy flash;
    testing::TesterProxy tester;
    bool done = false;
    bool modeSet = false;

    infra::TimerSingleShot timeoutTimer{ std::chrono::minutes(2), [this]()
        {
            done = true;
        } };
};

struct FlashTracer
{
    FlashTracer(services::TracingEchoOnStreams& echoTracer)
        : testerTracer(echoTracer)
        , testerObserverTracer(echoTracer)
        , flashTracer(echoTracer)
        , flashResultTracer(echoTracer)
    {}

    testing::TesterTracer testerTracer;
    testing::TesterObserverTracer testerObserverTracer;
    flash::FlashTracer flashTracer;
    flash::FlashResultTracer flashResultTracer;
};

int main(int argc, char** argv)
{
    args::ArgumentParser parser("Flasher");
    args::Group positionals(parser, "Positional arguments:");
    args::Positional<std::string> targetArgument(positionals, "target", "COM port or hostname", args::Options::Required);
    args::Positional<std::string> firmwareArgument(positionals, "firmware", "Binary firmware with which the target attached to Postmaster is upgraded");
    args::HelpFlag help(parser, "help", "display this help menu.", { 'h', "help" });

    try
    {
        parser.ParseCLI(argc, argv);

        static hal::TimerServiceGeneric timerService;
        static main_::TracerOnIoOutputInfrastructure tracer;
        static main_::NetworkAdapter network;
        static hal::FileSystemGeneric fileSystem;

        auto firmware = fileSystem.ReadBinaryFile(args::get(firmwareArgument));
        auto [echo, echoTracer] = application::OpenTracingEcho(args::get(targetArgument), network.ConnectionFactoryWithNameResolver(), tracer.tracer);

        tracer.tracer.Trace() << "Flashing " << args::get(firmwareArgument);

        FlashTracer flashTracer(*echoTracer);
        FirmwareSender sender(firmware, *echo);

        network.ExecuteUntil([&]()
            {
                return sender.Done();
            });

        tracer.tracer.Trace() << "Flashing done";
    }
    catch (const args::Help&)
    {
        std::cout << parser;
        return 1;
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
