@device:stm32f767
Feature: UART Duplex DMA
  Test the UART duplex DMA functionality

  Background:
    Given uart duplex dma peripherals are enabled

  Scenario: Send data over UART
    When the tested sends UART data
    Then the tester sees UART data

  Scenario: Receive data over UART
    When the tester sends UART data
    Then the tested sees UART data
