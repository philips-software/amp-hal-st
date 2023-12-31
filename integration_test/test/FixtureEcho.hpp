#ifndef FIXTURES_ECHO_HPP
#define FIXTURES_ECHO_HPP

#include "hal/windows/UartWindows.hpp"
#include "services/util/EchoInstantiation.hpp"

namespace main_
{
    struct FixtureEcho
    {
        hal::UartWindows serial{ "COM43" };
        services::MethodSerializerFactory::OnHeap serializerFactory;
        hal::BufferedSerialCommunicationOnUnbuffered::WithStorage<256> bufferedSerial{ serial };
        main_::EchoOnSesame<256> echo{ bufferedSerial, serializerFactory };
    };
}

#endif
