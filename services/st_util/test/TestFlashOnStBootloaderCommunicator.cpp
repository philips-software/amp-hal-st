#include "infra/stream/ByteInputStream.hpp"
#include "infra/stream/ByteOutputStream.hpp"
#include "infra/util/ByteRange.hpp"
#include "infra/util/Function.hpp"
#include "infra/util/test_helper/MockCallback.hpp"
#include "services/st_util/FlashOnStBootloaderCommunicator.hpp"
#include "services/st_util/StBootloaderCommunicator.hpp"
#include "services/st_util/test_doubles/StBootloaderCommunicatorMock.hpp"
#include "gmock/gmock.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <gtest/gtest.h>

template<class Result>
class NotifyingVerifyingFunctionMock;

template<class Result, class... Args>
class NotifyingVerifyingFunctionMock<Result(Args...)>
{
public:
    explicit NotifyingVerifyingFunctionMock(infra::Function<Result(Args...)> onCalled)
        : onCalled(onCalled)
    {
        EXPECT_CALL(*this, callback());
    }

    MOCK_CONST_METHOD0_T(callback, Result());

    operator infra::Function<Result(Args...)>()
    {
        return [this](Args&&... args)
        {
            onCalled(args...);
            return callback();
        };
    }

private:
    infra::Function<Result(Args...)> onCalled;
};

class FlashOnStBootloaderCommunicatorTest
    : public testing::Test
{
public:
    services::StBootloaderCommunicatorMock stBootloaderCommunicator;
    services::FlashHomogeneousOnStBootloaderCommunicator flashOnStBootloaderCommunicator{ 10, 1, stBootloaderCommunicator };
};

TEST_F(FlashOnStBootloaderCommunicatorTest, erase_all_sectors)
{
    infra::VerifyingFunctionMock<void()> onDone;

    EXPECT_CALL(stBootloaderCommunicator, ExtendedMassErase(services::MassEraseSubcommand::global, testing::_)).WillOnce([](auto, const auto& onDone)
        {
            onDone();
        });
    flashOnStBootloaderCommunicator.EraseAll(onDone);
}

TEST_F(FlashOnStBootloaderCommunicatorTest, erase_some_sectors)
{
    infra::VerifyingFunctionMock<void()> onDone;
    uint8_t iterator = 0;

    EXPECT_CALL(stBootloaderCommunicator, ExtendedErase(testing::_, testing::_)).WillRepeatedly([&iterator](auto pages, const auto& onDone)
        {
            ASSERT_TRUE(iterator < 5);
            EXPECT_EQ(pages.front(), 0);
            EXPECT_EQ(pages.back(), iterator++);
            onDone();
        });
    flashOnStBootloaderCommunicator.EraseSectors(0, 5, onDone);
}

TEST_F(FlashOnStBootloaderCommunicatorTest, read_small_buffer)
{
    std::array<uint8_t, 4> readData = {};
    std::array<uint8_t, 4> simulatedData = { 0, 1, 2, 3 };
    infra::ByteInputStream stream(simulatedData);

    EXPECT_CALL(stBootloaderCommunicator, ReadMemory(0x8000000, testing::_, testing::_)).WillOnce([&stream](auto, auto& data, const auto& onDone)
        {
            stream >> data;
            onDone();
        });

    NotifyingVerifyingFunctionMock<void()> onDone([&readData, &simulatedData]()
        {
            EXPECT_TRUE(readData == simulatedData);
        });

    flashOnStBootloaderCommunicator.ReadBuffer(infra::MakeByteRange(readData), 0, onDone);
}

TEST_F(FlashOnStBootloaderCommunicatorTest, read_big_buffer)
{
    std::array<uint8_t, 500> readData = {};
    std::array<uint8_t, 500> simulatedData = { 0 };
    std::fill(simulatedData.begin(), simulatedData.end(), 15);
    infra::ByteInputStream stream(simulatedData);

    EXPECT_CALL(stBootloaderCommunicator, ReadMemory(testing::_, testing::_, testing::_)).Times(2).WillRepeatedly([&stream](auto address, auto& data, const auto& onDone)
        {
            static auto relativeAddress = 0;
            ASSERT_EQ(address, 0x8000000 + relativeAddress);
            relativeAddress += data.size();

            stream >> data;
            onDone();
        });

    NotifyingVerifyingFunctionMock<void()> onDone([&readData, &simulatedData]()
        {
            EXPECT_TRUE(readData == simulatedData);
        });

    flashOnStBootloaderCommunicator.ReadBuffer(infra::MakeByteRange(readData), 0, onDone);
}

TEST_F(FlashOnStBootloaderCommunicatorTest, write_small_buffer)
{
    std::array<uint8_t, 4> writeData = { 0, 1, 2, 3 };
    infra::ByteOutputStream::WithStorage<4> stream;

    EXPECT_CALL(stBootloaderCommunicator, WriteMemory(0x8000000, testing::_, testing::_)).WillOnce([&stream](auto, auto data, const auto& onDone)
        {
            stream << data;
            onDone();
        });

    NotifyingVerifyingFunctionMock<void()> onDone([&writeData, &stream]()
        {
            EXPECT_TRUE(writeData == stream.Storage());
        });

    flashOnStBootloaderCommunicator.WriteBuffer(writeData, 0, onDone);
}

TEST_F(FlashOnStBootloaderCommunicatorTest, write_big_buffer)
{
    std::array<uint8_t, 500> writeData = { 0 };
    std::fill(writeData.begin(), writeData.end(), 15);
    infra::ByteOutputStream::WithStorage<500> stream;

    EXPECT_CALL(stBootloaderCommunicator, WriteMemory(testing::_, testing::_, testing::_)).Times(2).WillRepeatedly([&stream](auto address, auto data, const auto& onDone)
        {
            stream << data;

            static auto relativeAddress = 0;
            ASSERT_EQ(address, 0x8000000 + relativeAddress);
            relativeAddress += data.size();

            onDone();
        });

    NotifyingVerifyingFunctionMock<void()> onDone([&writeData, &stream]()
        {
            EXPECT_TRUE(writeData == stream.Storage());
        });

    flashOnStBootloaderCommunicator.WriteBuffer(writeData, 0, onDone);
}

// TODO data received but no commandActions in queue
