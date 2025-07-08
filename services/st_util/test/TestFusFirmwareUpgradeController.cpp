#include "infra/util/test_helper/MockCallback.hpp"
#include "services/st_util/FusFirmwareUpgradeController.hpp"
#include "gmock/gmock.h"
#include <gtest/gtest.h>

class FusWirelessStackUpgradeMock
    : public hal::FusWirelessStackUpgrade
{
public:
    MOCK_METHOD(void, GetFusState, (hal::FusWirelessStackUpgrade::OnDone onDone), (override));
    MOCK_METHOD(void, FirmwareUpgrade, (hal::FusWirelessStackUpgrade::OnDone onDone), (override));
    MOCK_METHOD(void, StartWirelessStack, (hal::FusWirelessStackUpgrade::OnDone onDone), (override));
    MOCK_METHOD(void, DeleteWirelessStack, (hal::FusWirelessStackUpgrade::OnDone onDone), (override));
};

class StBootloaderCommunicatorInitializerMock
    : public services::StBootloaderCommunicatorInitializer
{
public:
    MOCK_METHOD(void, Initialize, (), (override));
};

class FusFirmwareUpgradeControllerTest
    : public testing::Test
{
public:
    testing::StrictMock<FusWirelessStackUpgradeMock> fus;
    testing::StrictMock<StBootloaderCommunicatorInitializerMock> stBootloaderCommunicatorInitializer;
    services::FusFirmwareUpgradeController controller{ fus, stBootloaderCommunicatorInitializer };
};

TEST_F(FusFirmwareUpgradeControllerTest, delete_wireless_stack)
{
    infra::VerifyingFunction<void()> onDone;

    EXPECT_CALL(fus, GetFusState(testing::_)).Times(2).WillOnce(testing::Invoke([](auto onDone)
                                                                    {
                                                                        hal::FusWirelessStackUpgrade::StateWithErrorCode stateWithErrorCode;
                                                                        stateWithErrorCode.state = hal::FusWirelessStackUpgrade::State::error;
                                                                        stateWithErrorCode.errorCode = hal::FusWirelessStackUpgrade::ErrorCode::stateNotRunning;
                                                                        onDone(infra::MakeOptional(stateWithErrorCode));
                                                                    }))
        .WillOnce(testing::Invoke([](auto onDone)
            {
                hal::FusWirelessStackUpgrade::StateWithErrorCode stateWithErrorCode;
                stateWithErrorCode.state = hal::FusWirelessStackUpgrade::State::idle;
                stateWithErrorCode.errorCode = hal::FusWirelessStackUpgrade::ErrorCode::noError;
                onDone(infra::MakeOptional(stateWithErrorCode));
            }));
    EXPECT_CALL(fus, DeleteWirelessStack(testing::_)).WillOnce(testing::Invoke([](auto onDone)
        {
            onDone(infra::none);
        }));
    EXPECT_CALL(stBootloaderCommunicatorInitializer, Initialize());
    controller.DeleteWirelessStack(onDone);

    EXPECT_CALL(fus, GetFusState(testing::_)).WillOnce(testing::Invoke([](auto onDone)
            {
                hal::FusWirelessStackUpgrade::StateWithErrorCode stateWithErrorCode;
                stateWithErrorCode.state = hal::FusWirelessStackUpgrade::State::idle;
                stateWithErrorCode.errorCode = hal::FusWirelessStackUpgrade::ErrorCode::noError;
                onDone(infra::MakeOptional(stateWithErrorCode));
            }));
    stBootloaderCommunicatorInitializer.NotifyObservers([](auto& o)
        {
            o.Initialized();
        });
}
