#include "infra/stream/ByteInputStream.hpp"
#include "infra/stream/StdVectorInputStream.hpp"
#include "infra/util/test_helper/MockCallback.hpp"
#include "services/st_util/FirmwareUpgradeService.hpp"
#include "services/st_util/FusOnStBootloaderCommunicator.hpp"
#include "services/st_util/test_doubles/StBootloaderCommunicatorMock.hpp"
#include "gmock/gmock.h"
#include <cstdint>
#include <gtest/gtest.h>

class FusOnStBootloaderCommunicatorTest
    : public testing::Test
{
public:
    void FillRange(infra::ByteRange& range, uint8_t firstByte)
    {
        infra::StdVectorInputStream::WithStorage stream(infra::inPlace, std::vector<uint8_t>{ firstByte });
        stream >> infra::Head(range, stream.Available());
    }

    void FillRange(infra::ByteRange& range, uint8_t firstByte, hal::FirmwareUpgradeService::State state, hal::FirmwareUpgradeService::ErrorCode error)
    {
        infra::StdVectorInputStream::WithStorage stream(infra::inPlace, std::vector<uint8_t>{ { firstByte, static_cast<uint8_t>(state), static_cast<uint8_t>(error) } });
        stream >> range;
    }

public:
    testing::StrictMock<services::StBootloaderCommunicatorMock> stBootloaderCommunicator;
    services::FusOnStBootloaderCommunicator fusOnStBootloaderCommunicator{ stBootloaderCommunicator };
};

TEST_F(FusOnStBootloaderCommunicatorTest, get_fus_state)
{
    infra::VerifyingInvoker<void(infra::Optional<hal::FirmwareUpgradeService::StateWithErrorCode>)> onDone([](auto stateWithErrorCode)
        {
            EXPECT_EQ(stateWithErrorCode->state, hal::FirmwareUpgradeService::State::fusUpgrdOngoing);
            EXPECT_EQ(stateWithErrorCode->errorCode, hal::FirmwareUpgradeService::ErrorCode::imgNotFound);
        });

    EXPECT_CALL(stBootloaderCommunicator, Special(0x54, testing::_, testing::_, testing::_, testing::_)).WillOnce([this](auto subcommand, auto txData, auto& rxData, auto& rxStatus, const auto& onDone)
        {
            this->FillRange(rxData, 0x00, hal::FirmwareUpgradeService::State::fusUpgrdOngoing, hal::FirmwareUpgradeService::ErrorCode::imgNotFound);
            this->FillRange(rxStatus, 0x00);
            onDone();
        });

    fusOnStBootloaderCommunicator.GetState(onDone);
}

TEST_F(FusOnStBootloaderCommunicatorTest, firmware_upgrade_passes)
{
    infra::VerifyingInvoker<void(infra::Optional<hal::FirmwareUpgradeService::StateWithErrorCode>)> onDone([](auto stateWithErrorCode)
        {
            EXPECT_EQ(stateWithErrorCode, infra::none);
        });

    EXPECT_CALL(stBootloaderCommunicator, ExtendedSpecial(0x53, testing::_, testing::_, testing::_, testing::_)).WillOnce([this](auto subcommand, auto txData1, auto txData2, auto& rxData, const auto& onDone)
        {
            this->FillRange(rxData, 0x00);
            onDone();
        });
    fusOnStBootloaderCommunicator.Upgrade(onDone);
}

TEST_F(FusOnStBootloaderCommunicatorTest, firmware_upgrade_fails)
{
    infra::VerifyingInvoker<void(infra::Optional<hal::FirmwareUpgradeService::StateWithErrorCode>)> onDone([](auto stateWithErrorCode)
        {
            EXPECT_EQ(stateWithErrorCode->state, hal::FirmwareUpgradeService::State::error);
            EXPECT_EQ(stateWithErrorCode->errorCode, hal::FirmwareUpgradeService::ErrorCode::errUnknown);
        });

    EXPECT_CALL(stBootloaderCommunicator, ExtendedSpecial(0x53, testing::_, testing::_, testing::_, testing::_)).WillOnce([this](auto subcommand, auto txData1, auto txData2, auto& rxData, const auto& onDone)
        {
            this->FillRange(rxData, 0x01, hal::FirmwareUpgradeService::State::error, hal::FirmwareUpgradeService::ErrorCode::errUnknown);
            onDone();
        });
    fusOnStBootloaderCommunicator.Upgrade(onDone);
}

TEST_F(FusOnStBootloaderCommunicatorTest, start_wireless_stack_passes)
{
    infra::VerifyingInvoker<void(infra::Optional<hal::FirmwareUpgradeService::StateWithErrorCode>)> onDone([](auto stateWithErrorCode)
        {
            EXPECT_EQ(stateWithErrorCode, infra::none);
        });

    EXPECT_CALL(stBootloaderCommunicator, ExtendedSpecial(0x5A, testing::_, testing::_, testing::_, testing::_)).WillOnce([this](auto subcommand, auto txData1, auto txData2, auto& rxData, const auto& onDone)
        {
            this->FillRange(rxData, 0x00);
            onDone();
        });
    fusOnStBootloaderCommunicator.StartWirelessStack(onDone);
}

TEST_F(FusOnStBootloaderCommunicatorTest, start_wireless_stack_fails)
{
    infra::VerifyingInvoker<void(infra::Optional<hal::FirmwareUpgradeService::StateWithErrorCode>)> onDone([](auto stateWithErrorCode)
        {
            EXPECT_EQ(stateWithErrorCode->state, hal::FirmwareUpgradeService::State::error);
            EXPECT_EQ(stateWithErrorCode->errorCode, hal::FirmwareUpgradeService::ErrorCode::errUnknown);
        });

    EXPECT_CALL(stBootloaderCommunicator, ExtendedSpecial(0x5A, testing::_, testing::_, testing::_, testing::_)).WillOnce([this](auto subcommand, auto txData1, auto txData2, auto& rxData, const auto& onDone)
        {
            this->FillRange(rxData, 0x01, hal::FirmwareUpgradeService::State::error, hal::FirmwareUpgradeService::ErrorCode::errUnknown);
            onDone();
        });
    fusOnStBootloaderCommunicator.StartWirelessStack(onDone);
}

TEST_F(FusOnStBootloaderCommunicatorTest, delete_stack_passes)
{
    infra::VerifyingInvoker<void(infra::Optional<hal::FirmwareUpgradeService::StateWithErrorCode>)> onDone([](auto stateWithErrorCode)
        {
            EXPECT_EQ(stateWithErrorCode, infra::none);
        });

    EXPECT_CALL(stBootloaderCommunicator, ExtendedSpecial(0x52, testing::_, testing::_, testing::_, testing::_)).WillOnce([this](auto subcommand, auto txData1, auto txData2, auto& rxData, const auto& onDone)
        {
            this->FillRange(rxData, 0x00);
            onDone();
        });
    fusOnStBootloaderCommunicator.DeleteWirelessStack(onDone);
}

TEST_F(FusOnStBootloaderCommunicatorTest, delete_stack_fails)
{
    infra::VerifyingInvoker<void(infra::Optional<hal::FirmwareUpgradeService::StateWithErrorCode>)> onDone([](auto stateWithErrorCode)
        {
            EXPECT_EQ(stateWithErrorCode->state, hal::FirmwareUpgradeService::State::error);
            EXPECT_EQ(stateWithErrorCode->errorCode, hal::FirmwareUpgradeService::ErrorCode::errUnknown);
        });

    EXPECT_CALL(stBootloaderCommunicator, ExtendedSpecial(0x52, testing::_, testing::_, testing::_, testing::_)).WillOnce([this](auto subcommand, auto txData1, auto txData2, auto& rxData, const auto& onDone)
        {
            this->FillRange(rxData, 0x01, hal::FirmwareUpgradeService::State::error, hal::FirmwareUpgradeService::ErrorCode::errUnknown);
            onDone();
        });
    fusOnStBootloaderCommunicator.DeleteWirelessStack(onDone);
}
