# STM32WB55 Internal Flash Usage with BLE Stack 

## Scope

This document explains the limitation of using internal flash next to running a BLE stack in STM32WB55 series chips and the associated design decisions. The information on limitations is based on application note[1], experimentations and discussions with ST.

### Limitations on internal flash usage

Relevant part in application note[1] is section 4.7
CPU2, the radio cpu, cannot fetch and execute code from internal flash if CPU1, the application cpu, is writing to or erasing from flash. If CPU2 has time-critical code execution, for instance while advertising, scanning or connected, then CPU1 access to flash controlled. 

For this purpose CPU1 needs to follow an algorithm to gain access to flash[1].
In this algorithm Sem2 and Sem6 are only needed if the application is not running on bare metal. Therefore, we don't have to use these locks. We only need to use Sem7.

By Sem7, CPU2 protects its timing by restricting flash access of CPU1. It gives Sem7 only if it is guaranteed to perform 1 flash operation before the next radio event. Sem7 will be given access by CPU2 when:
- In Standby state: Always. There is no time-critical operation.
- In Advertising/Scanning/Connected states: 
  - For writing: If radio is idle and there is a ~4ms(time to write a chunk) buffer before the next radio event.
  - For erasing: If radio is idle and there is a ~25ms(time to erase a sector) buffer before the next radio event.

CPU1 must indicate before asking for Sem7 if it intends to write or erase. When CPU1 gets the sem7, it must:
- Not keep the sem7 for longer but immediately start the flash operation
- Only perform one flash operation and give back the semaphore at the completion

### Experiments

While connected or advertising, using the flash access algortihm[1], sem7 is requested repeatedly in quick successions to see the gaps.

1. While advertising: 
  - With 18.75ms or 25ms interval, sem7 cannot be obtained for erasing. This is expected as there is only <18.75ms or <25ms between two radio events, which is less than 25ms. 
  - Note that radio events also take time so between two events is less than advertising interval. One advertisement event is usually 3tx+3rx packets length with a slight dependency to advertisement data size for tx packets
  - With 31.25ms interval sem7 can be obtained. But we see a period of not exactly 31.25ms but longer. This is also expected since advertisement is always pseudo-randomly delayed by 0-10ms against air packet conflicts in the advertisement channels. 
2. While connected:
  - Results are similar to advertisement with one important difference. Radio events while connected do not have a fixed length. 
  - If neiter peers sends data, than connection event is just one empty tx and one empty rx packets long. - If peers are sending data, in theory the whole time between the two connection events can be occupied leaving no space for CPU1 flash operation. In practive this is only an issue between two embedded peers as with Phones as centrals, number of packets for a single connection event is limited. For iPhones it is at max 6, for Androids it can be 8, 10, 12, etc but it is always restricted.
  - For example, with a connection interval of 75ms, there should be always enough time for performing an erase.

### Design for modifying flash during radio operations

The idea to modify the flash during radio operations is that to increase the advertisement/scanning/connection interval sufficiently for the particular flash operation.
- Advertisement interval: Controlled by peripheral
- Scanning interval and window: Controlled by central
- Connection interval: Can be proposed by peripheral but eventually decided by central

From the central side, it is always possible to control all relevant properties.
From the peripheral side regarding connection interval, peripheral needs to propose a desired high enough connection interval and central decides the actual value. 
Practically all the phones as centrals conforms with peripheral suggested values as long as it is not too low.

Some guideline values are:

|             | Write                   | Erase                    |                                                                       |
|-------------|-------------------------|--------------------------|-----------------------------------------------------------------------|
| Advertising | interval > 10ms         | interval > 30ms          |                                                                       |
| Connected   | interval > 75ms         | interval > 75ms          | Assuming 20(10tx+10rx) full packets for a single connection interval. |
|             |                         |                          | Radio time = ~45ms                                                    | 
| Scanning    | interval - window > 5ms | interval - window > 25ms |                                                                       |
|             |                         |                          |                                                                       |


## References

[1] AN5289, How to build wireless applications with STM32WB MCUs, 18.0, https://www.st.com/resource/en/application_note/an5289-how-to-build-wireless-applications-with-stm32wb-mcus-stmicroelectronics.pdf
