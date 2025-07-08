#include "services/st_util/FusFirmwareUpgradeController.hpp"
#include "services/st_util/FusWirelessStackUpgrade.hpp"
#include <cstdlib>

namespace services
{
    FusFirmwareUpgradeController::FusFirmwareUpgradeController(hal::FusWirelessStackUpgrade& fus, StBootloaderCommunicatorInitializer& stBootloaderCommunicatorInitializer)
        : StBootloaderCommunicatorInitializerObserver(stBootloaderCommunicatorInitializer)
        , fus(fus)
    {}

    void FusFirmwareUpgradeController::DeleteWirelessStack(infra::Function<void()> onDone)
    {
        this->onDone = onDone;

        ActivateFus([this](auto stateWithError)
            {
                fus.DeleteWirelessStack([this](auto stateWithError)
                    {
                        // Handle if (stateWithError)
                        Subject().Initialize();
                    });
            });
    }

    void FusFirmwareUpgradeController::Initialized()
    {
        ActivateFus([this](auto stateWithError)
            {
                this->onDone();
            });
    }

    void FusFirmwareUpgradeController::ActivateFus(hal::FusWirelessStackUpgrade::OnDone fusActivated)
    {
        this->fusActivated = fusActivated;

        // AN5185, section 1.3 How to activate FUS
        fus.GetFusState([this](auto stateWithError)
            {
                if (stateWithError->errorCode == hal::FusWirelessStackUpgrade::ErrorCode::stateNotRunning)
                {
                    fus.GetFusState([this](auto stateWithError)
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
}
