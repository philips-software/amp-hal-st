#include "services/st_util/FusFirmwareUpgradeController.hpp"
#include "infra/util/ReallyAssert.hpp"
#include "services/st_util/FusWirelessStackUpgrade.hpp"
#include <cstdlib>

namespace services
{
    FusFirmwareUpgradeControllerImpl::FusFirmwareUpgradeControllerImpl(StBootloaderCommunicatorInitializer& stBootloaderCommunicatorInitializer)
        : StBootloaderCommunicatorInitializerObserver(stBootloaderCommunicatorInitializer)
        , state(infra::InPlaceType<StateIdle>(), *this)
    {}

    void FusFirmwareUpgradeControllerImpl::DeleteWirelessStack(infra::Function<void()> onDone)
    {
        really_assert(fus != nullptr);

        this->onDone = onDone;
        state.Emplace<StateDeleteWirelessStack>(*this);
    }

    void FusFirmwareUpgradeControllerImpl::Upgrade(infra::Function<void()> onDone)
    {
        really_assert(fus != nullptr);

        this->onDone = onDone;
        state.Emplace<StateUpgradeWirelessStack>(*this);
    }

    void FusFirmwareUpgradeControllerImpl::StartWirelessStack(infra::Function<void()> onDone)
    {
        really_assert(fus != nullptr);

        this->onDone = onDone;
        state.Emplace<StateStartWirelessStack>(*this);
    }

    void FusFirmwareUpgradeControllerImpl::Initialized(hal::FusWirelessStackUpgrade& fus)
    {
        this->fus = &fus;
        state->Initialized();
    }

    // AN5185, section 1.3 How to activate FUS
    void FusFirmwareUpgradeControllerImpl::ActivateFus(hal::FusWirelessStackUpgrade::OnDone fusActivated)
    {
        this->fusActivated = fusActivated;

        fus->GetFusState([this](auto stateWithError)
            {
                if (stateWithError->errorCode == hal::FusWirelessStackUpgrade::ErrorCode::stateNotRunning)
                {
                    fus->GetFusState([this](auto stateWithError)
                        {
                            if (stateWithError->errorCode != hal::FusWirelessStackUpgrade::ErrorCode::stateNotRunning)
                                this->fusActivated(stateWithError);
                            else
                                std::abort();
                        });
                }
                else
                    this->fusActivated(stateWithError);
            });
    }

    FusFirmwareUpgradeControllerImpl::StateIdle::StateIdle(FusFirmwareUpgradeControllerImpl& controller)
        : controller(controller)
    {}

    void FusFirmwareUpgradeControllerImpl::StateIdle::Initialized()
    {}

    FusFirmwareUpgradeControllerImpl::StateDeleteWirelessStack::StateDeleteWirelessStack(FusFirmwareUpgradeControllerImpl& controller)
        : controller(controller)
    {
        controller.ActivateFus([this](auto stateWithError)
            {
                this->controller.fus->DeleteWirelessStack([this](auto stateWithError)
                    {
                        if (stateWithError)
                        {
                            if (stateWithError->errorCode == hal::FusWirelessStackUpgrade::ErrorCode::imgNotFound)
                                WirelessStackDeleted();
                            else
                                std::abort();
                        }
                        else
                            this->controller.Subject().Initialize();
                    });
            });
    }

    void FusFirmwareUpgradeControllerImpl::StateDeleteWirelessStack::Initialized()
    {
        controller.ActivateFus([this](auto stateWithError)
            {
                WirelessStackDeleted();
            });
    }

    void FusFirmwareUpgradeControllerImpl::StateDeleteWirelessStack::WirelessStackDeleted()
    {
        controller.onDone();
        controller.state.Emplace<StateIdle>(controller);
    }

    FusFirmwareUpgradeControllerImpl::StateUpgradeWirelessStack::StateUpgradeWirelessStack(FusFirmwareUpgradeControllerImpl& controller)
        : controller(controller)
    {
        controller.ActivateFus([this](auto stateWithError)
            {
                this->controller.fus->FirmwareUpgrade([this](auto stateWithError)
                    {
                        if (stateWithError)
                            std::abort();
                        else
                            this->controller.Subject().Initialize();
                    });
            });
    }

    void FusFirmwareUpgradeControllerImpl::StateUpgradeWirelessStack::Initialized()
    {
        controller.ActivateFus([this](auto stateWithError)
            {
                WirelessStackUpgraded();
            });
    }

    void FusFirmwareUpgradeControllerImpl::StateUpgradeWirelessStack::WirelessStackUpgraded()
    {
        controller.onDone();
        controller.state.Emplace<StateIdle>(controller);
    }

    FusFirmwareUpgradeControllerImpl::StateStartWirelessStack::StateStartWirelessStack(FusFirmwareUpgradeControllerImpl& controller)
        : controller(controller)
    {
        this->controller.fus->StartWirelessStack([this](auto stateWithError)
            {
                if (stateWithError)
                    std::abort();
                else
                    this->controller.Subject().Initialize();
            });
    }

    void FusFirmwareUpgradeControllerImpl::StateStartWirelessStack::Initialized()
    {
        this->controller.fus->GetFusState([this](auto stateWithError)
            {
                if (stateWithError && stateWithError->errorCode == hal::FusWirelessStackUpgrade::ErrorCode::stateNotRunning)
                    WirelessStackStarted();
                else
                    std::abort();
            });
    }

    void FusFirmwareUpgradeControllerImpl::StateStartWirelessStack::WirelessStackStarted()
    {
        controller.onDone();
        controller.state.Emplace<StateIdle>(controller);
    }
}
