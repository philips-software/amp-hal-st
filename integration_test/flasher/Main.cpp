#include "args.hxx"
#include "generated/echo/Testing.pb.hpp"
#include "generated/echo/TracingFlash.pb.hpp"
#include "generated/echo/TracingTesting.pb.hpp"
#include "hal/generic/FileSystemGeneric.hpp"
#include "hal/generic/TimerServiceGeneric.hpp"
#include "integration_test/flasher/FirmwareSender.hpp"
#include "integration_test/logic/Flash.hpp"
#include "services/network_instantiations/EchoInstantiation.hpp"
#include "services/network_instantiations/NetworkAdapter.hpp"
#include "services/tracer/TracerOnIoOutputInfrastructure.hpp"

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
        auto echo = application::OpenEcho(args::get(targetArgument), network.ConnectionFactoryWithNameResolver());
        //auto [echo, echoTracer] = application::OpenTracingEcho(args::get(targetArgument), network.ConnectionFactoryWithNameResolver(), tracer.tracer);

        tracer.tracer.Trace() << "Flashing " << args::get(firmwareArgument);

        //FlashTracer flashTracer(*echoTracer);
        application::FirmwareSender sender(firmware, *echo, tracer.tracer);

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
