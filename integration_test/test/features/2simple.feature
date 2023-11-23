@device:CORTEX-M4
Feature: Simple feature file
  This is a Simple feature file

  Background:
    Given a background step

  # Scenario: scenario 01
  #   Given a simple data table
  #     | foo | bar |
  #     | boz | boo |
  #   And a data table with a single cell
  #     | foo |
  #   And a data table with different fromatting
  #     | foo | bar | boz |
  #   And a data table with an empty cell
  #     | foo |  | boz |
  #   And a data table with comments and newlines inside
  #     | foo | bar |

  #     | boz  | boo  |
  #     # this is a comment
  #     | boz2 | boo2 |

  @hierookeentag
  Scenario Outline: Can substract
    Given there are <x> cucumbers
    When I eat <y> cucumbers
    Given I eat <y> cucumbers
    Then I should have <z> cucumbers

    Examples:
      | x  | y | z |
      | 10 | 5 | 5 |
      | 11 | 3 | 8 |

  @dingus
  Scenario Outline: Is a dingus
    Given there are <x> cucumbers
    When I eat <y> cucumbers
    Then I should have <z> cucumbers
    Then I should have <z> cucumbers
    And this step should be skipped

    Examples:
      | x  | y | z |
      | 10 | 4 | 5 |
      | 11 | 3 | 6 |

  Scenario: a scenario with a missing step
    Given there are <x> cucumbers
    When I eat <y> cucumbers
    Then I should have <z> cucumbers left
    And this step should be skipped
    Examples:
      | x  | y | z |
      | 10 | 5 | 5 |
      | 11 | 3 | 8 |
