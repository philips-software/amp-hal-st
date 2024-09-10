@device:stm32f767
Feature: GPIO
  Test the GPIO functionality

  Background:
    Given gpio peripherals are enabled

  Scenario: toggle an output pin
    When the tested sets its output pin high
    Then the tester sees a high value
    When the tested sets its output pin low
    Then the tester sees a low value

  Scenario: observe an input pin
    When the tester sets its output pin high
    Then the tested sees a high value
    When the tester sets its output pin low
    Then the tested sees a low value
