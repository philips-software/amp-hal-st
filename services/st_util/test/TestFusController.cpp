#include "infra/util/test_helper/MockCallback.hpp"
#include "services/st_util/FusController.hpp"
#include "gmock/gmock.h"
#include <gtest/gtest.h>

class FirmwareUpgradeServiceMock
    : public hal::FirmwareUpgradeService
{
public:
    MOCK_METHOD(void, GetState, (hal::FirmwareUpgradeService::OnDone onDone), (override));
    MOCK_METHOD(void, Upgrade, (hal::FirmwareUpgradeService::OnDone onDone), (override));
    MOCK_METHOD(void, StartWirelessStack, (hal::FirmwareUpgradeService::OnDone onDone), (override));
    MOCK_METHOD(void, DeleteWirelessStack, (hal::FirmwareUpgradeService::OnDone onDone), (override));
};

class StBootloaderCommunicatorInitializerMock
    : public services::StBootloaderCommunicatorInitializer
{
public:
    MOCK_METHOD(void, Initialize, (), (override));
};

class FusControllerTest
    : public testing::Test
{
public:
    FusControllerTest()
        : controller(stBootloaderCommunicatorInitializer)
    {
        stBootloaderCommunicatorInitializer.NotifyObservers([this](auto& o)
            {
                o.Initialized(fus);
            });
    }

    void ExpectFusActivation()
    {
        EXPECT_CALL(fus, GetState(testing::_)).Times(2).WillOnce(testing::Invoke([](auto onDone)
                                                                     {
                                                                         hal::FirmwareUpgradeService::StateWithErrorCode stateWithErrorCode;
                                                                         stateWithErrorCode.state = hal::FirmwareUpgradeService::State::error;
                                                                         stateWithErrorCode.errorCode = hal::FirmwareUpgradeService::ErrorCode::stateNotRunning;
                                                                         onDone(infra::MakeOptional(stateWithErrorCode));
                                                                     }))
            .WillOnce(testing::Invoke([](auto onDone)
                {
                    hal::FirmwareUpgradeService::StateWithErrorCode stateWithErrorCode;
                    stateWithErrorCode.state = hal::FirmwareUpgradeService::State::idle;
                    stateWithErrorCode.errorCode = hal::FirmwareUpgradeService::ErrorCode::noError;
                    onDone(infra::MakeOptional(stateWithErrorCode));
                }));
    }

    testing::StrictMock<FirmwareUpgradeServiceMock> fus;
    testing::StrictMock<StBootloaderCommunicatorInitializerMock> stBootloaderCommunicatorInitializer;
    services::FusControllerImpl controller;
};

TEST_F(FusControllerTest, trigger_stack_upgrade)
{
    infra::VerifyingFunction<void()> onDone;

    ExpectFusActivation();
    EXPECT_CALL(fus, Upgrade(testing::_)).WillOnce(testing::Invoke([](auto onDone)
        {
            onDone(infra::none);
        }));
    EXPECT_CALL(stBootloaderCommunicatorInitializer, Initialize());
    controller.Upgrade(onDone);

    EXPECT_CALL(fus, GetState(testing::_)).WillOnce(testing::Invoke([](auto onDone)
        {
            hal::FirmwareUpgradeService::StateWithErrorCode stateWithErrorCode;
            stateWithErrorCode.state = hal::FirmwareUpgradeService::State::idle;
            stateWithErrorCode.errorCode = hal::FirmwareUpgradeService::ErrorCode::noError;
            onDone(infra::MakeOptional(stateWithErrorCode));
        }));
    stBootloaderCommunicatorInitializer.NotifyObservers([this](auto& o)
        {
            o.Initialized(fus);
        });
}

TEST_F(FusControllerTest, start_wireless_stack)
{
    infra::VerifyingFunction<void()> onDone;

    EXPECT_CALL(fus, StartWirelessStack(testing::_)).WillOnce(testing::Invoke([](auto onDone)
        {
            onDone(infra::none);
        }));
    EXPECT_CALL(stBootloaderCommunicatorInitializer, Initialize());
    controller.StartWirelessStack(onDone);

    EXPECT_CALL(fus, GetState(testing::_)).WillOnce(testing::Invoke([](auto onDone)
        {
            hal::FirmwareUpgradeService::StateWithErrorCode stateWithErrorCode;
            stateWithErrorCode.state = hal::FirmwareUpgradeService::State::error;
            stateWithErrorCode.errorCode = hal::FirmwareUpgradeService::ErrorCode::stateNotRunning;
            onDone(infra::MakeOptional(stateWithErrorCode));
        }));
    stBootloaderCommunicatorInitializer.NotifyObservers([this](auto& o)
        {
            o.Initialized(fus);
        });
}

TEST_F(FusControllerTest, delete_wireless_stack)
{
    infra::VerifyingFunction<void()> onDone;

    ExpectFusActivation();
    EXPECT_CALL(fus, DeleteWirelessStack(testing::_)).WillOnce(testing::Invoke([](auto onDone)
        {
            onDone(infra::none);
        }));
    EXPECT_CALL(stBootloaderCommunicatorInitializer, Initialize());
    controller.DeleteWirelessStack(onDone);

    EXPECT_CALL(fus, GetState(testing::_)).WillOnce(testing::Invoke([](auto onDone)
        {
            hal::FirmwareUpgradeService::StateWithErrorCode stateWithErrorCode;
            stateWithErrorCode.state = hal::FirmwareUpgradeService::State::idle;
            stateWithErrorCode.errorCode = hal::FirmwareUpgradeService::ErrorCode::noError;
            onDone(infra::MakeOptional(stateWithErrorCode));
        }));
    stBootloaderCommunicatorInitializer.NotifyObservers([this](auto& o)
        {
            o.Initialized(fus);
        });
}

TEST_F(FusControllerTest, delete_wireless_stack_while_no_stack_present)
{
    infra::VerifyingFunction<void()> onDone;

    ExpectFusActivation();
    EXPECT_CALL(fus, DeleteWirelessStack(testing::_)).WillOnce(testing::Invoke([](auto onDone)
        {
            hal::FirmwareUpgradeService::StateWithErrorCode stateWithErrorCode;
            stateWithErrorCode.state = hal::FirmwareUpgradeService::State::error;
            stateWithErrorCode.errorCode = hal::FirmwareUpgradeService::ErrorCode::imgNotFound;
            onDone(infra::MakeOptional(stateWithErrorCode));
        }));
    controller.DeleteWirelessStack(onDone);
}
