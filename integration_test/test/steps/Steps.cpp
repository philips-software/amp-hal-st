#include "cucumber-cpp/Steps.hpp"
#include "cucumber-cpp-example/fixtures/Fixture.hpp"
#include "cucumber-cpp/Context.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <any>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>

STEP("a background step")
{
    std::cout << "this is a background step\n";
}

STEP("a simple data table")
{
    auto fixture = context.get<NordicBleFixture>();
}

STEP("there are ([0-9]+) cucumbers")
{
    const auto num = std::stoi(parameters[0].get<std::string>());
    context.set("cucumbers_before", std::make_shared<int>(num));
}

STEP("I eat ([0-9]+) cucumbers")
{
    const auto num = std::stoi(parameters[0].get<std::string>());
    context.set("cucumbers_eaten", std::make_shared<int>(num));
}

STEP("^I should have ([0-9]+) cucumbers$")
{
    const auto num = std::stoi(parameters[0].get<std::string>());

    const auto before = context.get<int>("cucumbers_before");
    const auto eaten = context.get<int>("cucumbers_eaten");

    const auto actual = *before - *eaten;

    ASSERT_THAT(actual, testing::Eq(num));
}

STEP("I should have ([0-9]+) cucumbers left")
{
    const auto num = std::stoi(parameters[0].get<std::string>());

    const auto before = *context.get<int>("cucumbers_before");
    const auto eaten = *context.get<int>("cucumbers_eaten");

    const auto actual = before - eaten;

    ASSERT_THAT(actual, testing::Eq(num));
}

STEP("a data table with a single cell")
{}

STEP("a data table with different fromatting")
{}

STEP("a data table with an empty cell")
{}

STEP("a data table with comments and newlines inside")
{}
