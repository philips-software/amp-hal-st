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
        : public services::BondStorageAbsolute
    {
    public:
        explicit BondStorageSt(uint32_t maxNumberOfBonds);

        // Implementation of BondStorage
        void BondStorageSynchronizerCreated(services::BondStorageSynchronizer& manager) override;
        void RemoveBond(const services::GapAddress& address) override;
        void RemoveAllBonds() override;
        void RemoveBondIf(const infra::Function<bool(const services::GapAddress&)>& onAddress) override;
        uint32_t GetNumberOfBonds() const override;
        uint32_t GetMaxNumberOfBonds() const override;
        bool IsBondStored(const services::GapAddress& address) const override;
        void IterateBondedDevices(const infra::Function<void(const services::GapAddress&)>& onBond) override;

    private:
        using BondStorageInternal = Bonded_Device_Entry_t[((BLE_EVT_MAX_PARAM_LEN - 3) - 2) / sizeof(Bonded_Device_Entry_t)];

    private:
        uint32_t maxNumberOfBonds;
    };
}

#endif
