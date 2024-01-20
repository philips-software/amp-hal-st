#include "integration_test/runner/FixtureEcho.hpp"

namespace main_
{
    EchoClientWebSocket::EchoClientWebSocket(services::ConnectionFactoryWithNameResolver& connectionFactory,
        infra::BoundedString url, hal::SynchronousRandomDataGenerator& randomDataGenerator)
        : url(url)
        , clientConnector(connectionFactory)
        , httpClientInitiationCreator(
              [this](infra::Optional<services::HttpClientWebSocketInitiation>& value, services::WebSocketClientObserverFactory& clientObserverFactory,
                  services::HttpClientWebSocketInitiationResult& result, hal::SynchronousRandomDataGenerator& randomDataGenerator)
              {
                  value.Emplace(clientObserverFactory, clientConnector, result, randomDataGenerator);
              })
        , webSocketFactory(randomDataGenerator, { httpClientInitiationCreator })
    {
        webSocketFactory.Connect(*this);
    }

    void EchoClientWebSocket::OnDone(const OnDoneType& onDone)
    {
        this->onDone = onDone;
    }

    infra::BoundedString EchoClientWebSocket::Url() const
    {
        return url;
    }

    uint16_t EchoClientWebSocket::Port() const
    {
        return 80;
    }

    void EchoClientWebSocket::ConnectionEstablished(infra::AutoResetFunction<void(infra::SharedPtr<services::ConnectionObserver> client)>&& createdClientObserver)
    {
        auto echoConnectionPtr = echoConnection.Emplace(serializerFactory);
        createdClientObserver(echoConnectionPtr);
        onDone(*echoConnectionPtr);
    }

    void EchoClientWebSocket::ConnectionFailed(ConnectFailReason reason)
    {
        throw std::runtime_error("Creating ECHO over WebSocket failed");
    }
}
