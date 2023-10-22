#include "hal/interfaces/test_doubles/SerialCommunicationMock.hpp"
#include "infra/timer/Timer.hpp"
#include "infra/timer/test_helper/ClockFixture.hpp"
#include "infra/util/BoundedString.hpp"
#include "infra/util/ByteRange.hpp"
#include "infra/util/Function.hpp"
#include "infra/util/test_helper/MockCallback.hpp"
#include "services/st_util/StUartBootloaderCommandHandler.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <array>
#include <chrono>
#include <cstdint>
#include <stdint.h>

class StUartBootloaderCommandHandlerTest
    : public testing::Test
    , public infra::ClockFixture
{
public:
    void Initialize()
    {
        ForwardTime(std::chrono::milliseconds(20));
        serialCommunication.dataReceived(std::vector<uint8_t>{ 0x79 });

        EXPECT_CALL(onInitializedMock, callback());
        ExecuteAllActions();
    }

    void ExpectSendData(std::vector<uint8_t> data, infra::Function<void()> onSendData = infra::emptyFunction)
    {
        EXPECT_CALL(serialCommunication, SendDataMock(data)).WillOnce(testing::Invoke([this, onSendData](infra::ConstByteRange data)
            {
                onSendData();
                serialCommunication.actionOnCompletion();
            }));
    }

    void ExpectSendData(std::vector<uint8_t> data, uint8_t checksum)
    {
        ExpectSendData(data, [this, checksum]()
            {
                ExpectSendData({ checksum });
            });
    }

    void ExpectSendCommand(uint8_t command, uint8_t checksum)
    {
        ExpectSendData({ command }, [this, checksum]()
            {
                ExpectSendData({ checksum });
            });
    }

    void ExpectErrorOnTimeout(infra::Duration timeout, std::string errorMessage)
    {
        EXPECT_CALL(onErrorMock, callback(infra::BoundedConstString{ errorMessage }));
        ForwardTime(timeout);
    }

    void ExpectReceiveData(std::vector<uint8_t> data)
    {
        serialCommunication.dataReceived(data);
        ExecuteAllActions();
    }

public:
    testing::StrictMock<hal::SerialCommunicationMock> serialCommunication;
    testing::StrictMock<infra::MockCallback<void()>> onInitializedMock;
    testing::StrictMock<infra::MockCallback<void(infra::BoundedConstString)>> onErrorMock;
    infra::Execute execute{ [this]
        {
            ExpectSendData({ 0x7f });
        } };
    const infra::Function<void()> onInitialized{ [this]()
        {
            onInitializedMock.callback();
        } };
    const infra::Function<void(infra::BoundedConstString reason)> onError{ [this](infra::BoundedConstString reason)
        {
            onErrorMock.callback(reason);
        } };
    services::StUartBootloaderCommandHandler handler{ serialCommunication, onInitialized, onError };
};

TEST_F(StUartBootloaderCommandHandlerTest, creation_initializes_uart_bootloader)
{
    Initialize();
}

TEST_F(StUartBootloaderCommandHandlerTest, creation_initializes_uart_bootloader_fails)
{
    infra::BoundedConstString::WithStorage<64> errorMessage("Timeout waiting for bootloader initialization");
    EXPECT_CALL(onErrorMock, callback(errorMessage));
    ForwardTime(std::chrono::milliseconds(1000));
}

// TEST_F(StUartBootloaderCommandHandlerTest, receive_nack)
// TEST_F(StUartBootloaderCommandHandlerTest, receive_failing_stream)
// TEST_F(StUartBootloaderCommandHandlerTest, receive_invalid_data)

TEST_F(StUartBootloaderCommandHandlerTest, GetCommand)
{
    Initialize();
    infra::VerifyingFunctionMock<void(uint8_t, uint8_t)> ondone(3, 2);
    std::array<uint8_t, 15> commandsbuffer = {};
    infra::ByteRange commands(commandsbuffer);

    ExpectSendCommand(0x00, 0xff);
    handler.GetCommand(commands, ondone);
    ExpectReceiveData({ 0x79, 0x0b, 0x32, 0x00, 0x01, 0x02, 0x11, 0x21, 0x31, 0x44, 0x63, 0x73, 0x82, 0x92, 0x79 });

    EXPECT_EQ(commands.size(), 0x0b);
    EXPECT_EQ((std::array<uint8_t, 15>{ 0x00, 0x01, 0x02, 0x11, 0x21, 0x31, 0x44, 0x63, 0x73, 0x82, 0x92 }), commandsbuffer);
}

TEST_F(StUartBootloaderCommandHandlerTest, GetCommand_receive_intermittend)
{
    Initialize();
    infra::VerifyingFunctionMock<void(uint8_t, uint8_t)> ondone(3, 2);
    std::array<uint8_t, 15> commandsbuffer = {};
    infra::ByteRange commands(commandsbuffer);

    ExpectSendCommand(0x00, 0xff);
    handler.GetCommand(commands, ondone);
    ExpectReceiveData({ 0x79, 0x0b, 0x32, 0x00, 0x01 });
    ExpectReceiveData({ 0x02, 0x11, 0x21, 0x31, 0x44, 0x63, 0x73, 0x82, 0x92, 0x79 });

    EXPECT_EQ((std::array<uint8_t, 15>{ 0x00, 0x01, 0x02, 0x11, 0x21, 0x31, 0x44, 0x63, 0x73, 0x82, 0x92 }), commandsbuffer);
}

TEST_F(StUartBootloaderCommandHandlerTest, GetCommand_timeout)
{
    Initialize();
    testing::StrictMock<infra::MockCallback<void()>> ondone;
    std::array<uint8_t, 15> commandsbuffer = {};
    infra::ByteRange commands(commandsbuffer);

    ExpectSendCommand(0x00, 0xff);
    handler.GetCommand(commands, [&ondone](uint8_t, uint8_t)
        {
            ondone.callback();
        });
    ExpectErrorOnTimeout(std::chrono::seconds(1), "Timeout getting commands");
}

TEST_F(StUartBootloaderCommandHandlerTest, GetVersion)
{
    Initialize();
    infra::VerifyingFunctionMock<void(uint8_t, uint8_t)> ondone(3, 1);

    ExpectSendCommand(0x01, 0xfe);
    handler.GetVersion(ondone);
    ExpectReceiveData({ 0x79, 0x31, 0x00, 0x00, 0x79 });
}

TEST_F(StUartBootloaderCommandHandlerTest, GetVersion_timeout)
{
    Initialize();
    testing::StrictMock<infra::MockCallback<void()>> ondone;

    ExpectSendCommand(0x01, 0xfe);
    handler.GetVersion([&ondone](uint8_t, uint8_t)
        {
            ondone.callback();
        });
    ExpectErrorOnTimeout(std::chrono::seconds(1), "Timeout getting version");
}

TEST_F(StUartBootloaderCommandHandlerTest, GetId)
{
    Initialize();
    infra::VerifyingFunctionMock<void(uint16_t)> ondone(0x0495);

    ExpectSendCommand(0x02, 0xfd);
    handler.GetId(ondone);
    ExpectReceiveData({ 0x79, 0x01, 0x04, 0x95, 0x79 });
}

TEST_F(StUartBootloaderCommandHandlerTest, GetId_timeout)
{
    Initialize();
    testing::StrictMock<infra::MockCallback<void()>> ondone;

    ExpectSendCommand(0x02, 0xfd);
    handler.GetId([&ondone](uint16_t)
        {
            ondone.callback();
        });
    ExpectErrorOnTimeout(std::chrono::seconds(1), "Timeout getting id");
}

TEST_F(StUartBootloaderCommandHandlerTest, ReadMemory)
{
    Initialize();
    infra::VerifyingFunctionMock<void()> ondone;
    uint32_t address = 0x01020304;
    std::array<uint8_t, 4> dataBuffer = {};
    infra::ByteRange data(dataBuffer);

    ExpectSendCommand(0x11, 0xEE);
    handler.ReadMemory(address, 3, data, ondone);

    ExpectSendData({ 0x01, 0x02, 0x03, 0x04 }, 0x04);
    ExpectReceiveData({ 0x79 });

    ExpectSendCommand(0x02, 0xFD);
    ExpectReceiveData({ 0x79 });

    ExpectReceiveData({ 0x79 });
    ExpectReceiveData({ 0x05, 0x06, 0x07 });

    EXPECT_EQ((std::array<uint8_t, 4>{ 0x05, 0x06, 0x07, 0x00 }), dataBuffer);
}

TEST_F(StUartBootloaderCommandHandlerTest, ReadMemory_timeout)
{
    Initialize();
    testing::StrictMock<infra::MockCallback<void()>> ondone;
    std::array<uint8_t, 4> dataBuffer = {};
    infra::ByteRange data(dataBuffer);

    ExpectSendCommand(0x11, 0xEE);
    handler.ReadMemory(0x01020304, 3, data, [&ondone]()
        {
            ondone.callback();
        });
    ExpectErrorOnTimeout(std::chrono::seconds(1), "Timeout reading memory");
}

TEST_F(StUartBootloaderCommandHandlerTest, Go)
{
    Initialize();
    infra::VerifyingFunctionMock<void()> ondone;
    uint32_t address = 0x01020304;

    ExpectSendCommand(0x21, 0xde);
    handler.Go(address, ondone);

    ExpectSendData({ 0x01, 0x02, 0x03, 0x04 }, 0x04);
    ExpectReceiveData({ 0x79 });

    ExpectReceiveData({ 0x79 });
}

TEST_F(StUartBootloaderCommandHandlerTest, Go_timeout)
{
    Initialize();
    testing::StrictMock<infra::MockCallback<void()>> ondone;
    uint32_t address = 0x01020304;

    ExpectSendCommand(0x21, 0xde);
    handler.Go(address, [&ondone]()
        {
            ondone.callback();
        });
    ExpectErrorOnTimeout(std::chrono::seconds(1), "Timeout go");
}

TEST_F(StUartBootloaderCommandHandlerTest, WriteMemory)
{
    Initialize();
    infra::VerifyingFunctionMock<void()> ondone;
    std::array<uint8_t, 4> data = { 0x05, 0x06, 0x07, 0x08 };
    uint32_t address = 0x01020304;

    ExpectSendCommand(0x31, 0xce);
    handler.WriteMemory(address, infra::MakeByteRange(data), ondone);

    ExpectSendData({ 0x01, 0x02, 0x03, 0x04 }, 0x04);
    ExpectReceiveData({ 0x79 });

    ExpectSendData({ 0x05, 0x05, 0x06, 0x07, 0x08 }, 0x09);
    ExpectReceiveData({ 0x79 });
    ExpectReceiveData({ 0x79 });
}

TEST_F(StUartBootloaderCommandHandlerTest, WriteMemory_timeout)
{
    Initialize();
    testing::StrictMock<infra::MockCallback<void()>> ondone;
    std::array<uint8_t, 4> data;
    uint32_t address = 0x01020304;

    ExpectSendCommand(0x31, 0xce);
    handler.WriteMemory(address, infra::MakeByteRange(data), [&ondone]()
        {
            ondone.callback();
        });
    ExpectErrorOnTimeout(std::chrono::seconds(1), "Timeout writing memory");
}

TEST_F(StUartBootloaderCommandHandlerTest, Erase_global)
{
    Initialize();
    infra::VerifyingFunctionMock<void()> ondone;

    ExpectSendCommand(0x43, 0xbc);
    handler.Erase(0xff, ondone);
    ExpectSendCommand(0xff, 0x00);
    ExpectReceiveData({ 0x79 });
    ExpectReceiveData({ 0x79 });
}

TEST_F(StUartBootloaderCommandHandlerTest, Erase_pages)
{
    Initialize();
    infra::VerifyingFunctionMock<void()> ondone;
    std::array<uint8_t, 4> pages = { 0x01, 0x02, 0x03, 0x04 };

    ExpectSendCommand(0x43, 0xbc);
    handler.Erase(4, infra::MakeByteRange(pages), ondone);
    ExpectSendData({ 0x05, 0x01, 0x02, 0x03, 0x04 }, 0x01);
    ExpectReceiveData({ 0x79 });
    ExpectReceiveData({ 0x79 });
}

TEST_F(StUartBootloaderCommandHandlerTest, Erase_timeout)
{
    Initialize();
    testing::StrictMock<infra::MockCallback<void()>> ondone;
    std::array<uint8_t, 4> data;

    ExpectSendCommand(0x43, 0xbc);
    handler.Erase(0xff, infra::ByteRange{}, [&ondone]()
        {
            ondone.callback();
        });
    ExpectErrorOnTimeout(std::chrono::seconds(1), "Timeout erasing memory");
}

TEST_F(StUartBootloaderCommandHandlerTest, ExtendedErase_global)
{
    Initialize();
    infra::VerifyingFunctionMock<void()> ondone;

    ExpectSendCommand(0x44, 0xbb);
    handler.ExtendedErase(0xfffe, ondone);
    ExpectSendData({ 0xff, 0xfe }, 0x01);
    ExpectReceiveData({ 0x79 });
    ExpectReceiveData({ 0x79 });
}

TEST_F(StUartBootloaderCommandHandlerTest, ExtendedErase_pages)
{
    Initialize();
    infra::VerifyingFunctionMock<void()> ondone;
    std::array<infra::BigEndian<uint16_t>, 4> pages = { 0x0001, 0x0002, 0x0003, 0x0004 };

    ExpectSendCommand(0x44, 0xbb);
    handler.ExtendedErase(4, infra::MakeByteRange(pages), ondone);
    ExpectSendData({ 0x00, 0x05, 0x00, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x04 }, 0x01);
    ExpectReceiveData({ 0x79 });
    ExpectReceiveData({ 0x79 });
}

TEST_F(StUartBootloaderCommandHandlerTest, ExtendedErase_timeout)
{
    Initialize();
    testing::StrictMock<infra::MockCallback<void()>> ondone;

    ExpectSendCommand(0x44, 0xbb);
    handler.ExtendedErase(0xffff, [&ondone]()
        {
            ondone.callback();
        });
    ExpectErrorOnTimeout(std::chrono::seconds(1), "Timeout extended erasing memory");
}

TEST_F(StUartBootloaderCommandHandlerTest, Special)
{
    Initialize();
    infra::VerifyingFunctionMock<void()> ondone;
    uint16_t subcommand = 0x54;
    std::array<uint8_t, 4> txData = { 0x01, 0x02, 0x03, 0x04 };
    std::array<uint8_t, 4> rxDataBuffer;
    std::array<uint8_t, 4> rxStatusBuffer;
    infra::ByteRange rxData(rxDataBuffer);
    infra::ByteRange rxStatus(rxStatusBuffer);

    ExpectSendCommand(0x50, 0xaf);
    handler.Special(subcommand, infra::MakeConstByteRange(txData), rxData, rxStatus, ondone);

    ExpectSendData({ 0x00, 0x54 }, 0x54);
    ExpectReceiveData({ 0x79 });

    ExpectSendData({0x00, 0x04});
    ExpectSendData({ 0x01, 0x02, 0x03, 0x04 }, 0x04);
    ExpectReceiveData({ 0x79 });
    ExpectReceiveData({ 0x79 });

    ExpectReceiveData({ 0x00, 0x04, 0x05, 0x06, 0x07, 0x08 });
    ExpectReceiveData({ 0x00, 0x04, 0x09, 0x0a, 0x0b, 0x0c });

    EXPECT_EQ((std::array<uint8_t, 4>{ 0x05, 0x06, 0x07, 0x08 }), rxDataBuffer);
    EXPECT_EQ((std::array<uint8_t, 4>{ 0x09, 0x0a, 0x0b, 0x0c }), rxStatusBuffer);
}

TEST_F(StUartBootloaderCommandHandlerTest, Special_empty_rxData)
{
    Initialize();
    infra::VerifyingFunctionMock<void()> ondone;
    uint16_t subcommand = 0x54;
    std::array<uint8_t, 4> txData = { 0x01, 0x02, 0x03, 0x04 };
    std::array<uint8_t, 4> rxDataBuffer = {};
    std::array<uint8_t, 4> rxStatusBuffer;
    infra::ByteRange rxData(rxDataBuffer);
    infra::ByteRange rxStatus(rxStatusBuffer);

    ExpectSendCommand(0x50, 0xaf);
    handler.Special(subcommand, infra::MakeConstByteRange(txData), rxData, rxStatus, ondone);

    ExpectSendData({ 0x00, 0x54 }, 0x54);
    ExpectReceiveData({ 0x79 });

    ExpectSendData({0x00, 0x04});
    ExpectSendData({ 0x01, 0x02, 0x03, 0x04 }, 0x04);
    ExpectReceiveData({ 0x79 });
    ExpectReceiveData({ 0x79 });

    ExpectReceiveData({ 0x00, 0x00 });
    ExpectReceiveData({ 0x00, 0x04, 0x09, 0x0a, 0x0b, 0x0c });

    EXPECT_EQ((std::array<uint8_t, 4>{  }), rxDataBuffer);
    EXPECT_EQ((std::array<uint8_t, 4>{ 0x09, 0x0a, 0x0b, 0x0c }), rxStatusBuffer);
}

TEST_F(StUartBootloaderCommandHandlerTest, Special_empty_rxStatus)
{
    Initialize();
    infra::VerifyingFunctionMock<void()> ondone;
    uint16_t subcommand = 0x54;
    std::array<uint8_t, 4> txData = { 0x01, 0x02, 0x03, 0x04 };
    std::array<uint8_t, 4> rxDataBuffer;
    std::array<uint8_t, 4> rxStatusBuffer = {};
    infra::ByteRange rxData(rxDataBuffer);
    infra::ByteRange rxStatus(rxStatusBuffer);

    ExpectSendCommand(0x50, 0xaf);
    handler.Special(subcommand, infra::MakeConstByteRange(txData), rxData, rxStatus, ondone);

    ExpectSendData({ 0x00, 0x54 }, 0x54);
    ExpectReceiveData({ 0x79 });

    ExpectSendData({0x00, 0x04});
    ExpectSendData({ 0x01, 0x02, 0x03, 0x04 }, 0x04);
    ExpectReceiveData({ 0x79 });
    ExpectReceiveData({ 0x79 });

    ExpectReceiveData({ 0x00, 0x04, 0x05, 0x06, 0x07, 0x08 });
    ExpectReceiveData({ 0x00, 0x00 });

    EXPECT_EQ((std::array<uint8_t, 4>{ 0x05, 0x06, 0x07, 0x08 }), rxDataBuffer);
    EXPECT_EQ((std::array<uint8_t, 4>{  }), rxStatusBuffer);
}

TEST_F(StUartBootloaderCommandHandlerTest, Special_empty_rxData_rxStatus)
{
    Initialize();
    infra::VerifyingFunctionMock<void()> ondone;
    uint16_t subcommand = 0x54;
    std::array<uint8_t, 4> txData = { 0x01, 0x02, 0x03, 0x04 };
    std::array<uint8_t, 4> rxDataBuffer = {};
    std::array<uint8_t, 4> rxStatusBuffer = {};
    infra::ByteRange rxData(rxDataBuffer);
    infra::ByteRange rxStatus(rxStatusBuffer);

    ExpectSendCommand(0x50, 0xaf);
    handler.Special(subcommand, infra::MakeConstByteRange(txData), rxData, rxStatus, ondone);

    ExpectSendData({ 0x00, 0x54 }, 0x54);
    ExpectReceiveData({ 0x79 });

    ExpectSendData({0x00, 0x04});
    ExpectSendData({ 0x01, 0x02, 0x03, 0x04 }, 0x04);
    ExpectReceiveData({ 0x79 });
    ExpectReceiveData({ 0x79 });

    ExpectReceiveData({ 0x00, 0x00 });
    ExpectReceiveData({ 0x00, 0x00 });

    EXPECT_EQ((std::array<uint8_t, 4>{  }), rxDataBuffer);
    EXPECT_EQ((std::array<uint8_t, 4>{  }), rxStatusBuffer);
}

TEST_F(StUartBootloaderCommandHandlerTest, Special_empty_txData)
{
    Initialize();
    infra::VerifyingFunctionMock<void()> ondone;
    uint16_t subcommand = 0x54;
    std::array<uint8_t, 4> rxDataBuffer;
    std::array<uint8_t, 4> rxStatusBuffer;
    infra::ByteRange rxData(rxDataBuffer);
    infra::ByteRange rxStatus(rxStatusBuffer);

    ExpectSendCommand(0x50, 0xaf);
    handler.Special(subcommand, infra::ConstByteRange{}, rxData, rxStatus, ondone);

    ExpectSendData({ 0x00, 0x54 }, 0x54);
    ExpectReceiveData({ 0x79 });

    ExpectSendData({0x00, 0x00});
    ExpectReceiveData({ 0x79 });
    ExpectReceiveData({ 0x79 });

    ExpectReceiveData({ 0x00, 0x04, 0x05, 0x06, 0x07, 0x08 });
    ExpectReceiveData({ 0x00, 0x04, 0x09, 0x0a, 0x0b, 0x0c });

    EXPECT_EQ((std::array<uint8_t, 4>{ 0x05, 0x06, 0x07, 0x08 }), rxDataBuffer);
    EXPECT_EQ((std::array<uint8_t, 4>{ 0x09, 0x0a, 0x0b, 0x0c }), rxStatusBuffer);
}

TEST_F(StUartBootloaderCommandHandlerTest, Special_timeout)
{
    Initialize();
    testing::StrictMock<infra::MockCallback<void()>> ondone;
    infra::ByteRange rxData;
    infra::ByteRange rxStatus;

    ExpectSendCommand(0x50, 0xaf);
    handler.Special(0x54, infra::ConstByteRange{}, rxData, rxStatus, [&ondone]()
        {
            ondone.callback();
        });
    ExpectErrorOnTimeout(std::chrono::seconds(1), "Timeout special command");
}

TEST_F(StUartBootloaderCommandHandlerTest, ExtendedSpecial)
{
    Initialize();
    infra::VerifyingFunctionMock<void()> ondone;
    uint16_t subcommand = 0x54;
    std::array<uint8_t, 4> txData1 = { 0x01, 0x02, 0x03, 0x04 };
    std::array<uint8_t, 4> txData2 = { 0x05, 0x06, 0x07, 0x08 };
    std::array<uint8_t, 4> rxDataBuffer;
    infra::ByteRange rxData(rxDataBuffer);

    ExpectSendCommand(0x51, 0xae);
    handler.ExtendedSpecial(subcommand, infra::MakeConstByteRange(txData1), infra::MakeConstByteRange(txData2), rxData, ondone);

    ExpectSendData({ 0x00, 0x54 }, 0x54);
    ExpectReceiveData({ 0x79 });

    ExpectSendData({0x00, 0x04});
    ExpectSendData({ 0x01, 0x02, 0x03, 0x04 }, 0x04);
    ExpectReceiveData({ 0x79 });

    ExpectSendData({0x00, 0x04});
    ExpectSendData({ 0x05, 0x06, 0x07, 0x08 }, 0x0c);
    ExpectReceiveData({ 0x79 });
    ExpectReceiveData({ 0x79 });

    ExpectReceiveData({ 0x00, 0x04, 0x09, 0x0a, 0x0b, 0x0c });

    EXPECT_EQ((std::array<uint8_t, 4>{ 0x09, 0x0a, 0x0b, 0x0c }), rxDataBuffer);
}

TEST_F(StUartBootloaderCommandHandlerTest, ExtendedSpecial_timeout)
{
    Initialize();
    testing::StrictMock<infra::MockCallback<void()>> ondone;
    infra::ByteRange rxData;

    ExpectSendCommand(0x51, 0xae);
    handler.ExtendedSpecial(0x54, infra::ConstByteRange{}, infra::ConstByteRange{}, rxData, [&ondone]()
        {
            ondone.callback();
        });
    ExpectErrorOnTimeout(std::chrono::seconds(1), "Timeout extended special command");
}
