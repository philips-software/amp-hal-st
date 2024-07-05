@device:stm32f767
Feature: UART
  Test the UART functionality

  Background:
    Given uart peripherals are enabled

  Scenario: Send data over UART
    When the tested sends UART data
    Then the tester sees UART data

  Scenario: Receive data over UART
    When the tester sends UART data
    Then the tested sees UART data
