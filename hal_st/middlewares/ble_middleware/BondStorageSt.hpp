#ifndef HAL_ST_BOND_STORAGE_ST_HPP
#define HAL_ST_BOND_STORAGE_ST_HPP

#include "services/ble/BondStorageSynchronizer.hpp"

extern "C"
{
#include "ble_gap_aci.h"
}

namespace hal
{
    class BondStorageSt
        : public services::BondStorage
    {
    public:
        explicit BondStorageSt(uint32_t maxNumberOfBonds);

        // Implementation of BondStorage
        void BondStorageSynchronizerCreated(services::BondStorageSynchronizer& manager) override;
        void UpdateBondedDevice(hal::MacAddress address) override;
        void RemoveBond(hal::MacAddress address) override;
        void RemoveAllBonds() override;
        void RemoveBondIf(const infra::Function<bool(hal::MacAddress)>& onAddress) override;
        uint32_t GetMaxNumberOfBonds() const override;
        bool IsBondStored(hal::MacAddress address) const override;
        void IterateBondedDevices(const infra::Function<void(hal::MacAddress)>& onAddress) override;

    private:
        using BondStorageInternal = Bonded_Device_Entry_t[((BLE_EVT_MAX_PARAM_LEN - 3) - 2) / sizeof(Bonded_Device_Entry_t)];

    private:
        uint32_t maxNumberOfBonds;
    };
}

#endif
