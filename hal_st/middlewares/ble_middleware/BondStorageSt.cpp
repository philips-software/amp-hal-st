#include "hal_st/middlewares/ble_middleware/BondStorageSt.hpp"
#include "infra/util/LogAndAbort.hpp"
#include "services/ble/Gap.hpp"

namespace
{
    services::GapDeviceAddressType ToGapAddressType(uint8_t addressType)
    {
        switch (addressType)
        {
            case 0x00:
                return services::GapDeviceAddressType::publicAddress;
            case 0x01:
                return services::GapDeviceAddressType::randomAddress;
            default:
                LOG_AND_ABORT_ENUM(addressType);
        }
    }

    bool AddressMatchesBond(const services::GapAddress& address, const Bonded_Device_Entry_t& bondEntry)
    {
        // Note: specifically not checking the address type, since it's not stored in shadow storage currently.
        return infra::ContentsEqual(infra::MakeRange(address.address), infra::MakeRange(bondEntry.Address));
    }

    services::GapAddress ToGapAddress(const Bonded_Device_Entry_t& bondEntry)
    {
        services::GapAddress address;
        std::copy(bondEntry.Address, bondEntry.Address + sizeof(bondEntry.Address), address.address.begin());
        address.type = ToGapAddressType(bondEntry.Address_Type);
        return address;
    }
}

namespace hal
{
    BondStorageSt::BondStorageSt(uint32_t maxNumberOfBonds)
        : maxNumberOfBonds(maxNumberOfBonds)
    {}

    void BondStorageSt::BondStorageSynchronizerCreated(services::BondStorageSynchronizer& manager)
    {}

    void BondStorageSt::RemoveBond(const services::GapAddress& address)
    {
        uint8_t numberOfBonds = 0;
        BondStorageInternal storage;
        aci_gap_get_bonded_devices(&numberOfBonds, storage);

        for (auto i = 0; i != numberOfBonds; ++i)
            if (AddressMatchesBond(address, storage[i]))
                aci_gap_remove_bonded_device(storage[i].Address_Type, storage[i].Address);
    }

    void BondStorageSt::RemoveAllBonds()
    {
        uint8_t numberOfBonds = 0;
        BondStorageInternal storage;
        aci_gap_get_bonded_devices(&numberOfBonds, storage);

        for (auto i = 0; i != numberOfBonds; ++i)
            aci_gap_remove_bonded_device(storage[i].Address_Type, storage[i].Address);
    }

    void BondStorageSt::RemoveBondIf(const infra::Function<bool(const services::GapAddress&)>& onAddress)
    {
        LOG_AND_ABORT_NOT_IMPLEMENTED();
    }

    uint32_t BondStorageSt::GetNumberOfBonds() const
    {
        uint8_t numberOfBonds = 0;
        BondStorageInternal storage;
        aci_gap_get_bonded_devices(&numberOfBonds, storage);

        return numberOfBonds;
    }

    uint32_t BondStorageSt::GetMaxNumberOfBonds() const
    {
        return maxNumberOfBonds;
    }

    bool BondStorageSt::IsBondStored(const services::GapAddress& address) const
    {
        uint8_t numberOfBonds = 0;
        BondStorageInternal storage;
        aci_gap_get_bonded_devices(&numberOfBonds, storage);

        for (auto i = 0; i != numberOfBonds; ++i)
            if (AddressMatchesBond(address, storage[i]))
                return true;

        return false;
    }

    void BondStorageSt::IterateBondedDevices(const infra::Function<void(const services::GapAddress&)>& onBond)
    {
        uint8_t numberOfBonds = 0;
        BondStorageInternal storage;
        aci_gap_get_bonded_devices(&numberOfBonds, storage);

        for (auto i = 0; i != numberOfBonds; ++i)
            onBond(ToGapAddress(storage[i]));
    }
}
