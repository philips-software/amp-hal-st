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

template<class Result, std::size_t ExtraSize = INFRA_DEFAULT_FUNCTION_EXTRA_SIZE>
class NotifyingVerifyingFunctionMock;

template<class Result, std::size_t ExtraSize>
class NotifyingVerifyingFunctionMock<Result(), ExtraSize>
{
public:
    NotifyingVerifyingFunctionMock(infra::Function<void()> onCalled)
        : onCalled(onCalled)
    {
        EXPECT_CALL(*this, callback());
    }

    MOCK_CONST_METHOD0_T(callback, Result());

    operator infra::Function<Result(), ExtraSize>()
    {
        return [this]()
        {
            onCalled();
            return callback();
        };
    }

private:
    infra::Function<void()> onCalled;
};

class FlasOnStBootloaderCommunicatorTest
    : public testing::Test
{
public:
    services::StBootloaderCommunicatorMock stBootloaderCommunicator;
    services::FlasHomogeneousOnStBootloaderCommunicator flasOnStBootloaderCommunicator{ 10, 1, stBootloaderCommunicator };
};

TEST_F(FlasOnStBootloaderCommunicatorTest, erase_all_sectors)
{
    infra::VerifyingFunctionMock<void()> onDone;

    EXPECT_CALL(stBootloaderCommunicator, ExtendedMassErase(services::MassEraseSubcommand::global, testing::_)).WillOnce([](auto, const auto& onDone)
        {
            onDone();
        });
    flasOnStBootloaderCommunicator.EraseAll(onDone);
}

TEST_F(FlasOnStBootloaderCommunicatorTest, erase_some_sectors)
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
    flasOnStBootloaderCommunicator.EraseSectors(0, 5, onDone);
}

TEST_F(FlasOnStBootloaderCommunicatorTest, read_small_buffer)
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

    flasOnStBootloaderCommunicator.ReadBuffer(infra::MakeByteRange(readData), 0, onDone);
}

TEST_F(FlasOnStBootloaderCommunicatorTest, read_big_buffer)
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

    flasOnStBootloaderCommunicator.ReadBuffer(infra::MakeByteRange(readData), 0, onDone);
}

TEST_F(FlasOnStBootloaderCommunicatorTest, write_small_buffer)
{
    std::array<uint8_t, 4> writeData = { 0, 1, 2, 3 };
    std::array<uint8_t, 4> targetBuffer = { 0 };
    infra::ByteOutputStream stream(targetBuffer);

    EXPECT_CALL(stBootloaderCommunicator, WriteMemory(0x8000000, testing::_, testing::_)).WillOnce([&stream](auto, auto data, const auto& onDone)
        {
            stream << data;
            onDone();
        });

    NotifyingVerifyingFunctionMock<void()> onDone([&writeData, &targetBuffer]()
        {
            EXPECT_TRUE(writeData == targetBuffer);
        });

    flasOnStBootloaderCommunicator.WriteBuffer(writeData, 0, onDone);
}

TEST_F(FlasOnStBootloaderCommunicatorTest, write_big_buffer)
{
    std::array<uint8_t, 500> writeData = { 0 };
    std::fill(writeData.begin(), writeData.end(), 15);
    std::array<uint8_t, 500> targetBuffer = { 0 };
    infra::ByteOutputStream stream(targetBuffer);

    EXPECT_CALL(stBootloaderCommunicator, WriteMemory(testing::_, testing::_, testing::_)).Times(2).WillRepeatedly([&stream](auto address, auto data, const auto& onDone)
        {
            stream << data;

            static auto relativeAddress = 0;
            ASSERT_EQ(address, 0x8000000 + relativeAddress);
            relativeAddress += data.size();

            onDone();
        });

    NotifyingVerifyingFunctionMock<void()> onDone([&writeData, &targetBuffer]()
        {
            EXPECT_TRUE(writeData == targetBuffer);
        });

    flasOnStBootloaderCommunicator.WriteBuffer(writeData, 0, onDone);
}
