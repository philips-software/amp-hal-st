// Reproduces the circular RX DMA overrun that aborted a reverse firmware upgrade on STM32WBA65.
//
// Board: NUCLEO-WBA65RI. Connect a jumper between PD9 (USART3 TX) and PC5 (USART3 RX);
// USART3 on GPDMA1 channels 5/6 is the exact peripheral set that failed in the field.
// Results are traced over USART1 (PB12/PA8, ST-Link VCP) at 921600 baud.
//
// USART3 streams a byte pattern into its own receiver, so the circular RX DMA never sees an idle
// line. A repeating timer then masks interrupts for longer than the ring takes to fill, emulating
// the blocking flash programming and radio critical sections that starve the DMA interrupt during
// an upgrade.
//
// With a driver that infers the DMA position from which callback fired, this aborts on
// really_assert(lastReceivedPosition <= currentPosition). With the position read from the DMA
// counter, the link keeps running and the loss is reported through OverrunCount().

#include "hal_st/cortex/FaultTracer.hpp"
#include "hal_st/instantiations/NucleoUi.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/UartStmDma.hpp"
#include "hal_st/stm32fxxx/UartStmDuplexDma.hpp"
#include "infra/timer/Timer.hpp"
#include "infra/util/ByteRange.hpp"
#include "infra/util/MemoryRange.hpp"
#include "services/tracer/GlobalTracer.hpp"
#include "services/tracer/StreamWriterOnSerialCommunication.hpp"
#include "services/tracer/Tracer.hpp"
#include "services/tracer/TracerWithDateTime.hpp"
#include "services/util/DebugLed.hpp"
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>

#include DEVICE_HEADER

extern "C" uint32_t link_code_location;
extern "C" uint32_t link_code_end;
extern "C" uint32_t _estack;

unsigned int hse_value = 32'000'000;

namespace
{
    constexpr uint32_t baudrate = 921600;

    // 256 bytes fills in ~2.8 ms at 921600 8N1, so a few ms of masked interrupts laps the ring.
    constexpr std::size_t rxBufferSize = 256;

    // One transfer must outlast a stall, otherwise the line goes idle and the ring stops filling.
    constexpr std::size_t txChunkSize = 1024;

    constexpr uint32_t stallMicroseconds = 4000;
    constexpr auto stallInterval = std::chrono::milliseconds{ 50 };
    constexpr auto reportInterval = std::chrono::seconds{ 1 };

    // A volatile loop iteration is a handful of cycles; the stall only has to exceed the ring, not be exact.
    constexpr uint32_t approximateCyclesPerIteration = 5;

    void StallWithInterruptsMasked(uint32_t microseconds)
    {
        const auto iterations = SystemCoreClock / 1'000'000U * microseconds / approximateCyclesPerIteration;

        __disable_irq();

        for (volatile uint32_t i = 0; i != iterations; ++i)
            __NOP();

        __enable_irq();
    }

    class LoopbackStream
    {
    public:
        explicit LoopbackStream(hal::UartStmDuplexDma& uart)
            : uart(uart)
        {
            uart.ReceiveData([this](infra::ConstByteRange data)
                {
                    Received(data);
                });

            SendNext();
        }

        uint32_t BytesSent() const
        {
            return bytesSent;
        }

        uint32_t BytesReceived() const
        {
            return bytesReceived;
        }

        uint32_t SequenceGaps() const
        {
            return sequenceGaps;
        }

    private:
        void SendNext()
        {
            for (auto& byte : txBuffer)
                byte = txPattern++;

            bytesSent += txBuffer.size();

            uart.SendData(txBuffer, [this]()
                {
                    SendNext();
                });
        }

        // Runs in DMA and USART interrupt context; counters only, tracing here would stall the very ISR under test.
        void Received(infra::ConstByteRange data)
        {
            for (auto byte : data)
            {
                if (synchronized && byte != expected)
                    ++sequenceGaps;

                synchronized = true;
                expected = static_cast<uint8_t>(byte + 1);
            }

            bytesReceived += data.size();
        }

    private:
        hal::UartStmDuplexDma& uart;
        std::array<uint8_t, txChunkSize> txBuffer{};
        uint8_t txPattern = 0;
        uint8_t expected = 0;
        bool synchronized = false;
        uint32_t bytesSent = 0;
        uint32_t bytesReceived = 0;
        uint32_t sequenceGaps = 0;
    };
}

int main()
{
    HAL_Init();

    static main_::StmEventInfrastructure eventInfrastructure;
    static main_::Nucleo64WBA6Ui ui;
    static services::DebugLed debugLed(ui.ledGreen);
    static hal::DmaStm dmaStm;

    static hal::GpioPinStm traceUartTxPin{ hal::Port::B, 12 };
    static hal::GpioPinStm traceUartRxPin{ hal::Port::A, 8 };
    static hal::DmaStm::TransmitStream traceTransmitStream{ dmaStm, hal::DmaChannelId{ 1, 1, GPDMA1_REQUEST_USART1_TX } };
    static hal::UartStmDma traceUart{ traceTransmitStream, 1, traceUartTxPin, traceUartRxPin, hal::UartStmDma::Config{ .baudrate = baudrate } };

    static services::StreamWriterOnSerialCommunication::WithStorage<256> streamWriter{ traceUart };
    static infra::TextOutputStream::WithErrorPolicy textOutputStream{ streamWriter };
    static services::TracerWithDateTime tracer{ textOutputStream };
    services::SetGlobalTracerInstance(tracer);

    static const auto defaultFaultHandler = hal::DefaultFaultTracer(infra::MakeRange<const uint32_t>(&link_code_location, &link_code_end), &_estack, []() -> services::Tracer&
        {
            return services::GlobalTracer();
        });

    static hal::GpioPinStm loopbackUartTxPin{ hal::Port::D, 9 };
    static hal::GpioPinStm loopbackUartRxPin{ hal::Port::C, 5 };
    static hal::DmaStm::TransmitStream loopbackTransmitStream{ dmaStm, hal::DmaChannelId{ 1, 5, GPDMA1_REQUEST_USART3_TX } };
    static hal::DmaStm::ReceiveStream loopbackReceiveStream{ dmaStm, hal::DmaChannelId{ 1, 6, GPDMA1_REQUEST_USART3_RX } };
    static hal::UartStmDuplexDma::WithRxBuffer<rxBufferSize> loopbackUart{ loopbackTransmitStream, loopbackReceiveStream, 3, loopbackUartTxPin, loopbackUartRxPin,
        hal::UartStmDma::Config{ .baudrate = baudrate, .priority = hal::InterruptPriority::High } };

    services::GlobalTracer().Trace() << "UART DMA RX overrun example, ring " << rxBufferSize << " bytes at " << baudrate << " baud";
    services::GlobalTracer().Trace() << "Connect PD9 to PC5; without the jumper no bytes are received";

    static LoopbackStream loopbackStream{ loopbackUart };

    static infra::TimerRepeating stallTimer{ stallInterval, []()
        {
            StallWithInterruptsMasked(stallMicroseconds);
        } };

    static infra::TimerRepeating reportTimer{ reportInterval, []()
        {
            services::GlobalTracer().Trace() << "sent " << loopbackStream.BytesSent()
                                             << ", received " << loopbackStream.BytesReceived()
                                             << ", sequence gaps " << loopbackStream.SequenceGaps()
                                             << ", overruns " << loopbackUart.OverrunCount();
        } };

    eventInfrastructure.Run();
    __builtin_unreachable();
}
