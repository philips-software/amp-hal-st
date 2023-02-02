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
        virtual void BondStorageSynchronizerCreated(services::BondStorageSynchronizer& manager) override;
        virtual void UpdateBondedDevice(hal::MacAddress address) override;
        virtual void RemoveBond(hal::MacAddress address) override;
        virtual void RemoveAllBonds() override;
        virtual void RemoveBondIf(const infra::Function<bool(hal::MacAddress)>& onAddress) override;
        virtual uint32_t GetMaxNumberOfBonds() const override;
        virtual bool IsBondStored(hal::MacAddress address) const override;
        virtual void IterateBondedDevices(const infra::Function<void(hal::MacAddress)>& onAddress) override;

    private:
        using BondStorageInternal = Bonded_Device_Entry_t[((BLE_EVT_MAX_PARAM_LEN - 3) - 2) / sizeof(Bonded_Device_Entry_t)];

    private:
        uint32_t maxNumberOfBonds;
    };
}

#endif
