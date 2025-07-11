#include "services/st_util/FusController.hpp"
#include "infra/util/ReallyAssert.hpp"
#include "services/st_util/FirmwareUpgradeService.hpp"
#include <cstdlib>

namespace services
{
    FusControllerImpl::FusControllerImpl(StBootloaderCommunicatorInitializer& stBootloaderCommunicatorInitializer)
        : StBootloaderCommunicatorInitializerObserver(stBootloaderCommunicatorInitializer)
        , state(infra::InPlaceType<StateIdle>(), *this)
    {}

    void FusControllerImpl::Upgrade(infra::Function<void()> onDone)
    {
        really_assert(fus != nullptr);

        this->onDone = onDone;
        state.Emplace<StateUpgradeWirelessStack>(*this);
    }

    void FusControllerImpl::StartWirelessStack(infra::Function<void()> onDone)
    {
        really_assert(fus != nullptr);

        this->onDone = onDone;
        state.Emplace<StateStartWirelessStack>(*this);
    }

    void FusControllerImpl::DeleteWirelessStack(infra::Function<void()> onDone)
    {
        really_assert(fus != nullptr);

        this->onDone = onDone;
        state.Emplace<StateDeleteWirelessStack>(*this);
    }

    void FusControllerImpl::Initialized(hal::FirmwareUpgradeService& fus)
    {
        this->fus = &fus;
        state->Initialized();
    }

    // AN5185, section 1.3 How to activate FUS
    void FusControllerImpl::ActivateFus(hal::FirmwareUpgradeService::OnDone fusActivated)
    {
        this->fusActivated = fusActivated;

        fus->GetState([this](auto stateWithError)
            {
                if (stateWithError->errorCode == hal::FirmwareUpgradeService::ErrorCode::stateNotRunning)
                {
                    fus->GetState([this](auto stateWithError)
                        {
                            if (stateWithError->errorCode != hal::FirmwareUpgradeService::ErrorCode::stateNotRunning)
                                this->fusActivated(stateWithError);
                            else
                                std::abort();
                        });
                }
                else
                    this->fusActivated(stateWithError);
            });
    }

    FusControllerImpl::StateIdle::StateIdle(FusControllerImpl& controller)
        : controller(controller)
    {}

    void FusControllerImpl::StateIdle::Initialized()
    {}

    // AN5185, section 2.1 Wireless stack install and upgrade
    FusControllerImpl::StateUpgradeWirelessStack::StateUpgradeWirelessStack(FusControllerImpl& controller)
        : controller(controller)
    {
        controller.ActivateFus([this](auto stateWithError)
            {
                this->controller.fus->Upgrade([this](auto stateWithError)
                    {
                        if (stateWithError)
                            std::abort();
                        else
                            this->controller.Subject().Initialize();
                    });
            });
    }

    void FusControllerImpl::StateUpgradeWirelessStack::Initialized()
    {
        controller.ActivateFus([this](auto stateWithError)
            {
                WirelessStackUpgraded();
            });
    }

    void FusControllerImpl::StateUpgradeWirelessStack::WirelessStackUpgraded()
    {
        controller.onDone();
        controller.state.Emplace<StateIdle>(controller);
    }

    // AN5185, section 2.3 Wireless stack start
    FusControllerImpl::StateStartWirelessStack::StateStartWirelessStack(FusControllerImpl& controller)
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

    void FusControllerImpl::StateStartWirelessStack::Initialized()
    {
        this->controller.fus->GetState([this](auto stateWithError)
            {
                if (stateWithError && stateWithError->errorCode == hal::FirmwareUpgradeService::ErrorCode::stateNotRunning)
                    WirelessStackStarted();
                else
                    std::abort();
            });
    }

    void FusControllerImpl::StateStartWirelessStack::WirelessStackStarted()
    {
        controller.onDone();
        controller.state.Emplace<StateIdle>(controller);
    }

    // AN5185, section 2.2 Wireless stack delete
    FusControllerImpl::StateDeleteWirelessStack::StateDeleteWirelessStack(FusControllerImpl& controller)
        : controller(controller)
    {
        controller.ActivateFus([this](auto stateWithError)
            {
                this->controller.fus->DeleteWirelessStack([this](auto stateWithError)
                    {
                        if (stateWithError)
                        {
                            if (stateWithError->errorCode == hal::FirmwareUpgradeService::ErrorCode::imgNotFound)
                                WirelessStackDeleted();
                            else
                                std::abort();
                        }
                        else
                            this->controller.Subject().Initialize();
                    });
            });
    }

    void FusControllerImpl::StateDeleteWirelessStack::Initialized()
    {
        controller.ActivateFus([this](auto stateWithError)
            {
                WirelessStackDeleted();
            });
    }

    void FusControllerImpl::StateDeleteWirelessStack::WirelessStackDeleted()
    {
        controller.onDone();
        controller.state.Emplace<StateIdle>(controller);
    }
}
