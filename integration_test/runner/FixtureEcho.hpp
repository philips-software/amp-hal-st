#ifndef FIXTURES_ECHO_HPP
#define FIXTURES_ECHO_HPP

#include "generated/echo/TracingTesting.pb.hpp"
#ifdef EMIL_HAL_WINDOWS
#include "hal/windows/UartWindows.hpp"
#else
#include "hal/unix/UartUnix.hpp"
#endif
#include "services/network/EchoOnConnection.hpp"
#include "services/network/HttpClientImpl.hpp"
#include "services/network/WebSocketClientConnectionObserver.hpp"
#include "services/tracer/GlobalTracer.hpp"
#include "services/tracer/TracingEchoInstantiation.hpp"

namespace main_
{
    struct FixtureEchoSerialBase
    {
        FixtureEchoSerialBase(const std::string& portName)
            : serial(portName)
        {}

#ifdef EMIL_HAL_WINDOWS
        hal::UartWindows serial;
#else
        hal::UartUnix serial;
#endif
        services::MethodSerializerFactory::OnHeap serializerFactory;
        hal::BufferedSerialCommunicationOnUnbuffered::WithStorage<256> bufferedSerial{ serial };
    };

    struct FixtureEchoSerial
        : FixtureEchoSerialBase
    {
        main_::EchoOnSesame<256> echoOnSesame{ bufferedSerial, serializerFactory };

        services::Echo& echo{ echoOnSesame.echo };
    };

    struct FixtureTracingEchoSerial
        : FixtureEchoSerialBase
    {
        main_::TracingEchoOnSesame<256> echoOnSesame{ bufferedSerial, serializerFactory, services::GlobalTracer() };

        services::Echo& echo{ echoOnSesame.echo };
        testing::TesterTracer testerTracer{ echoOnSesame.echo };
        testing::TestedTracer testedTracer{ echoOnSesame.echo };
        testing::TestedObserverTracer testedObserverTracer{ echoOnSesame.echo };
        testing::GpioTesterTracer gpioTesterTracer{ echoOnSesame.echo };
        testing::GpioTestedTracer gpioTestedTracer{ echoOnSesame.echo };
        testing::GpioObserverTracer gpioObserverTracer{ echoOnSesame.echo };
    };

    class EchoClientWebSocket
        : private services::WebSocketClientObserverFactory
    {
    public:
        using OnDoneType = infra::Function<void(services::Echo&), 2 * sizeof(void*) + sizeof(std::shared_ptr<void>)>;

        EchoClientWebSocket(services::ConnectionFactoryWithNameResolver& connectionFactory,
            infra::BoundedString url, hal::SynchronousRandomDataGenerator& randomDataGenerator);

        void OnDone(const OnDoneType& onDone);

    private:
        infra::BoundedString Url() const override;
        uint16_t Port() const override;
        void ConnectionEstablished(infra::AutoResetFunction<void(infra::SharedPtr<services::ConnectionObserver> client)>&& createdClientObserver) override;
        void ConnectionFailed(ConnectFailReason reason) override;

    private:
        infra::BoundedString url;
        services::HttpClientConnectorWithNameResolverImpl<> clientConnector;
        infra::Creator<services::Stoppable, services::HttpClientWebSocketInitiation, void(services::WebSocketClientObserverFactory& clientObserverFactory, services::HttpClientWebSocketInitiationResult& result, hal::SynchronousRandomDataGenerator& randomDataGenerator)> httpClientInitiationCreator;
        services::WebSocketClientFactorySingleConnection webSocketFactory;

        infra::SharedOptional<services::EchoOnConnection> echoConnection;
        OnDoneType onDone;
        services::MethodSerializerFactory::OnHeap serializerFactory;
    };
}

#endif
