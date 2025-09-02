# Pair BLE using OOB (Out Of Band)

## Scope
This document explains how to pair devices over BLE using Out Of Band (OOB) on the STM32WBA65 MCU. It outlines scenarios for pairing between Central and Peripheral devices, as well as pairing with an Android device.

## Pairing between two embedded sides

In this case, the Central and Peripheral are both using the OOB implementation. It's selected by the proper hal::GapSt::Security in the hal::GapCentralSt parameter. For OOB, it's necessary to select the Security Level 4.

### Privacy disabled

#### Central and Peripheral generating OOB credentials

The sequence for this configuration is provided by ST: first Central em Peripheral generate the OOB. The data is exchanged and the values are stored on each side. Now the normal process is followed, with the Peripheral advertising and Central requesting the connection. The pairing will be handled using the data from OOB.

#### Only Central generating OOB credentials

The sequence for this configuration is the same as when both are generating the OOB, but just the Central generates the OOB data and send it to Peripheral, that configure it on the stack. Now the normal process is followed, with the Peripheral advertising and Central requesting the connection. The pairing will be handled using the data from OOB.

#### Only Peripheral generating OOB credentials

Same as for just Central, but in reverse order.
 
### Privacy enabled

#### Central and Peripheral generating OOB credentials

This configuration works the same way as when privacy is disabled.

#### Only Central generating OOB credentials

This configuration works the same way as when privacy is disabled.

#### Only Peripheral generating OOB credentials

In this configuration, since the Peripheral has an RPA (Resolvable Private Address) for it's advertising when privacy is enabled, the process is different. The key could be generated at the beginning and exchange before the connection, but the Central just set the OOB data after the Connection and before Pairing phase. After Connection, Central knows the Peripheral's Identity and is able to pair.
 
## Pairing with an Android device 

This case is to cover the situation where a Peripheral wants to connect and pair with an Android device. The core of this connection is the NDEF: it is important to use the correct information in the Tag to be recognized by the Android. There is a script to generate the NDEF data in the format Android is expecting.

### Privacy disabled

A Connection Handover Record is required, otherwise only a few Android devices will respond to a tap. The TK (Temporary Key) is required in the BLE OOB record. During the tests, when the TK was not provided, Android asked for a PIN. The TK that is to be provided in the BLE OOB record must equal the OOB random data. 

### Privacy enabled

To use a Peripheral with privacy enabled to connect to Android device, the only difference from the configuration above is the address added to the NDEF. In this case, the address has to be the RPA, which the Peripheral is using to advertise.

### Script python to generate NDEF
```
import ndef
import io
import pprint

# from nordic example
#hexstr = '91020d487315c102000000046163010130001a205d016170706c69636174696f6e2f766e642e626c7565746f6f74682e6c652e6f6f6230081b4a412f65b3ef01021c0011108af8b8669bd32f077cd89bc5be59730f11229236d1a0aced56b5a2785d2599f287611123fe1d3d37e89d9775308ce389da9e13a30319000002010413094e6f726469635f4e46435f70616972696e6751021a5470101375726e3a6e66633a736e3a68616e646f766572002c020400'
#octets = bytearray.fromhex(hexstr)
#decoder = ndef.message_decoder(octets)
#records = list(decoder)
#pprint.pp(records)

address = 'B0:C8:98:21:00:24'
random = 'd82d87aba8d2707a4387d1335cb1a49d'
confirm = 'dc449ec12c2a1a13b5aff703eef81c8b'
tk = 'd82d87aba8d2707a4387d1335cb1a49d'

print("Connection Handover Record")
connection_handover_record = ndef.HandoverSelectRecord('1.5', None)
connection_handover_record.add_alternative_carrier('active', '0')
pprint.pp(connection_handover_record)

print("\n")
print("Bluetooth Low Energy Record")
ble_oob_record = ndef.BluetoothLowEnergyRecord()
ble_oob_record.device_name = 'peer_node'
ble_oob_record.role_capabilities = "Peripheral"
ble_oob_record.flags = 4
print(ble_oob_record.device_name)
print(ble_oob_record.role_capabilities)
print(ble_oob_record.flags)

ble_oob_record.device_address = address
print(ble_oob_record.device_address.addr)
print(ble_oob_record.device_address.type)

ble_oob_record.secure_connections_random_value = int.from_bytes(bytearray.fromhex(random), byteorder='little', signed=False)
ble_oob_record.secure_connections_confirmation_value = int.from_bytes(bytearray.fromhex(confirm), byteorder='little', signed=False)
ble_oob_record.security_manager_tk_value = int.from_bytes(bytearray.fromhex(tk), byteorder='little', signed=False)

pprint.pp(ble_oob_record)

for k in ble_oob_record.attribute_names:
    print('key {} value {}'.format(k, ble_oob_record.get(k)))

new_records = [connection_handover_record, ble_oob_record]
buf = b"".join((ndef.message_encoder(new_records)))
print("\n\n")
print("Encoded NDEF records: {}".format(buf.hex()))
```

## References

[1] STM32WB-WBA BLE security - Out Of Band pairing, https://wiki.st.com/stm32mcu/wiki/Connectivity:STM32WB-WBA_BLE_security_-_out_of_band_pairing
[2] AN5270 - Introduction to STM32WB Bluetooth® Low Energy wireless interface, https://www.st.com/resource/en/application_note/an5270-stm32wb-bluetooth-low-energy-wireless-interface-stmicroelectronics.pdf
[3] STM32WBA BLE Wireless Interface,  https://htmlpreview.github.io/?https://github.com/STMicroelectronics/STM32CubeWBA/blob/main/Middlewares/ST/STM32_WPAN/ble/stack/doc/STM32WBA_BLE_Wireless_Interface.html#ACI_ATT_EXCHANGE_MTU_RESP_EVENT_anchor
