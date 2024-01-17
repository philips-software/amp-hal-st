#include "cucumber-cpp/Hooks.hpp"
#include "args.hxx"
#include "generated/echo/Testing.pb.hpp"
#include "hal/generic/TimerServiceGeneric.hpp"
#include "infra/event/EventDispatcherThreadAware.hpp"
#include "integration_test/logic/Tested.hpp"
#include "integration_test/test/FixtureEcho.hpp"
#include "integration_test/test/Waiting.hpp"
#include "gtest/gtest.h"

HOOK_BEFORE_ALL()
{
    args::ArgumentParser parser("Integration test runner for amp-hal-st");
    args::Group arguments(parser, "Arguments:");
    args::Positional<std::string> serialPort(arguments, "port", "Serial port of the amp-hal-st integration test board", args::Options::Required);
    args::Group flags(parser, "Optional flags:");
    args::HelpFlag help(flags, "help", "Display this help menu.", { 'h', "help" });

    try
    {
        const auto& args = context.Get<std::vector<std::string_view>>("args");
        std::vector<std::string> stringArgs(args.begin(), args.end());
        parser.ParseArgs(stringArgs);

        context.Emplace<infra::EventDispatcherThreadAware::WithSize<50>>();
        context.Emplace<hal::TimerServiceGeneric>();

        auto echoFixture = std::make_shared<main_::FixtureEchoSerial>(get(serialPort));
        auto echo = std::shared_ptr<services::Echo>(echoFixture, &echoFixture->echo);
        context.SetShared(echo);

        context.Emplace<testing::TesterProxy>(*echo);
        context.Emplace<testing::TestedProxy>(*echo);
        context.Emplace<application::TestedObserver>(context.Get<services::Echo>());
    }
    catch (const args::Error& error)
    {
        std::ostringstream errorPlusHelp;
        errorPlusHelp << error.what() << parser;
        throw std::runtime_error(errorPlusHelp.str());
    }
}

HOOK_BEFORE_SCENARIO()
{
    infra::WaitUntilDone(context, [&](const std::function<void()>& done)
        {
            context.Get<testing::TesterProxy>().RequestSend([&]()
                {
                    context.Get<testing::TesterProxy>().Reset();
                    done();
                });
        });

    infra::WaitUntilDone(context, [&](const std::function<void()>& done)
        {
            context.Get<testing::TestedProxy>().RequestSend([&]()
                {
                    context.Get<testing::TestedProxy>().Ping();
                    done();
                });
        });

    infra::WaitFor(context, [&]()
        {
            return context.Get<application::TestedObserver>().ReceivedPong();
        });
}
