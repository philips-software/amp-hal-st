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
        main_::EchoOnSerialCommunication<256> echo{ serial, serializerFactory };
    };
}

#endif
