#include "hal_st/middlewares/ble_middleware/BondStorageSt.hpp"

namespace hal
{
    BondStorageSt::BondStorageSt(uint32_t maxNumberOfBonds)
        : maxNumberOfBonds(maxNumberOfBonds)
    {}

    void BondStorageSt::BondStorageManagerCreated(services::BondStorageManager& manager)
    {}

    void BondStorageSt::UpdateBondedDevice(hal::MacAddress address)
    {
        // ST middleware handles updating of bonds list internally, so we don't have anything to update here.
    }

    void BondStorageSt::RemoveBond(hal::MacAddress address)
    {
        uint8_t numberOfBonds;
        BondStorageInternal storage;
        aci_gap_get_bonded_devices(&numberOfBonds, storage);

        for (auto i = 0; i != numberOfBonds; ++i)
            if (infra::ContentsEqual(infra::MakeRange(storage[i].Address), infra::MakeRange(address)))
                aci_gap_remove_bonded_device(storage[i].Address_Type, storage[i].Address);
    }

    void BondStorageSt::RemoveAllBonds()
    {
        uint8_t numberOfBonds;
        BondStorageInternal storage;
        aci_gap_get_bonded_devices(&numberOfBonds, storage);

        for (auto i = 0; i != numberOfBonds; ++i)
            aci_gap_remove_bonded_device(storage[i].Address_Type, storage[i].Address);
    }

    void BondStorageSt::RemoveBondIf(const infra::Function<bool(hal::MacAddress)>& onAddress)
    {}

    uint32_t BondStorageSt::GetMaxNumberOfBonds() const
    {
        return maxNumberOfBonds;
    }

    bool BondStorageSt::IsBondStored(hal::MacAddress address) const
    {
        uint8_t numberOfBonds;
        BondStorageInternal storage;
        aci_gap_get_bonded_devices(&numberOfBonds, storage);

        for (auto i = 0; i != numberOfBonds; ++i)
            if (infra::ContentsEqual(infra::MakeRange(storage[i].Address), infra::MakeRange(address)))
                return true;

        return false;
    }

    void BondStorageSt::IterateBondedDevices(const infra::Function<void(hal::MacAddress)>& onAddress)
    {
        uint8_t numberOfBonds;
        BondStorageInternal storage;
        aci_gap_get_bonded_devices(&numberOfBonds, storage);

        for (auto i = 0; i != numberOfBonds; ++i)
        {
            hal::MacAddress address;
            infra::Copy(infra::MakeRange(storage[i].Address), infra::MakeRange(address));
            onAddress(address);
        }
    }
}
