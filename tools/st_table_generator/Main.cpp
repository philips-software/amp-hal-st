#include "args.hxx"
#include "google/protobuf/compiler/code_generator.h"
#include "google/protobuf/compiler/cpp/helpers.h"
#include "google/protobuf/compiler/plugin.h"
#include "google/protobuf/io/printer.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/stubs/strutil.h"
#include "hal/generic/FileSystemGeneric.hpp"
#include "infra/xml/XmlNavigator.hpp"
#include "infra/syntax/CppFormatter.hpp"

template<class T>
std::vector<T> Compact(const std::vector<infra::Optional<T>>& v)
{
    std::vector<T> result;

    for (auto& x : v)
        if (x != infra::none)
            result.push_back(*x);

    return result;
}

struct PeripheralCandidate
{
    std::string name;
    std::string type;
    std::vector<std::string> ipNames;
    infra::Optional<std::string> prefix;
    infra::Optional<std::string> base;
};

struct Peripheral
{
    std::string name;
    int position;
    infra::Optional<std::string> clockEnable;
    infra::Optional<std::string> base;
};

struct PeripheralGroup
{
    std::string groupName;
    std::vector<Peripheral> peripherals;
};

class TableGenerator
{
public:
    TableGenerator(hal::FileSystem& fileSystem, const std::filesystem::path& inputPath, const std::vector<PeripheralCandidate>& candidates);

    void Write(const std::filesystem::path& outputDirectory);

private:
    void ReadCandidates(const std::vector<PeripheralCandidate>& candidates);
    void ReadCandidate(const PeripheralCandidate& candidate);

    static std::string Concatenate(const std::vector<std::string>& lines);

private:
    std::filesystem::path inputDirectory;
    std::string input;
    infra::XmlNodeNavigator navigator;
    std::vector<PeripheralCandidate> candidates;
    std::vector<PeripheralGroup> peripheralGroups;
};

TableGenerator::TableGenerator(hal::FileSystem& fileSystem, const std::filesystem::path& inputPath, const std::vector<PeripheralCandidate>& candidates)
    : inputDirectory(inputPath.parent_path())
    , input(Concatenate(fileSystem.ReadFile(inputPath)))
    , navigator(input)
{
    ReadCandidates(candidates);
}

void TableGenerator::Write(const std::filesystem::path& outputDirectory)
{
    for (const auto& peripheralGroup : peripheralGroups)
    {
        std::cout << "Group: " << peripheralGroup.groupName << std::endl;
        for (const auto& peripheral : peripheralGroup.peripherals)
        {
            std::cout << "    Peripheral " << peripheral.name << " pos " << peripheral.position;
            if (peripheral.clockEnable != infra::none)
                std::cout << " Clock Enable: " << *peripheral.clockEnable;
            std::cout << std::endl;
        }
    }

    std::unique_ptr<google::protobuf::io::ZeroCopyOutputStream> stream = std::make_unique<google::protobuf::io::OstreamOutputStream>(&std::cout);
    google::protobuf::io::Printer printer(stream.get(), '$', nullptr);
    application::Entities formatter(true);

    auto includesByHeader = std::make_shared<application::IncludesByHeader>();
    includesByHeader->Path("infra/util/MemoryRange.hpp");
    includesByHeader->PathMacro("DEVICE_HEADER");
    formatter.Add(includesByHeader);

    auto includesBySource = std::make_shared<application::IncludesBySource>();
    includesBySource->Path("generated/stm32fxxx/PeripheralTable.hpp");
    includesBySource->PathSystem("<array>");
    includesBySource->PathSystem("<cstdlib>");
    formatter.Add(includesBySource);

    auto halNamespace = std::make_shared<application::Namespace>("hal");

    for (const auto& group : peripheralGroups)
    {
        if (group.peripherals.empty())
        {
             
        }
    }

    std::cout << "============================" << std::endl;
    formatter.PrintHeader(printer);
    std::cout << "============================" << std::endl;
    formatter.PrintSource(printer, "");
    std::cout << "============================" << std::endl;
}

void TableGenerator::ReadCandidates(const std::vector<PeripheralCandidate>& candidates)
{
    for (const auto& candidate : candidates)
        ReadCandidate(candidate);
}

void TableGenerator::ReadCandidate(const PeripheralCandidate& candidate)
{
    const infra::XmlNodeNavigatorToken mcu{ "Mcu" };
    const infra::XmlTransformArrayNavigatorToken<infra::Optional<Peripheral>> ip{ "IP", [this, &candidate](const infra::XmlNodeNavigator& navigator)
        {
            static const infra::XmlStringAttributeNavigatorToken name{ "Name" };
            static const infra::XmlStringAttributeNavigatorToken instance_name{ "InstanceName" };
            static const infra::XmlOptionalStringAttributeNavigatorToken clock_enable_mode{ "ClockEnableMode" };

            auto ipName = navigator / name;
            for (auto& candidateIpName : candidate.ipNames)
                if (ipName == candidateIpName)
                {
                    Peripheral result;

                    result.name = navigator / instance_name;

                    if (candidate.prefix != infra::none)
                        result.position = std::stoi(result.name.substr(candidate.prefix->size()));
                    else if (result.name == ipName)
                        result.position = 1;
                    else
                        result.position = std::stoi(result.name.substr(ipName.size()));

                    auto clockEnable = navigator / clock_enable_mode;
                    if (clockEnable != infra::none)
                        result.clockEnable = clockEnable;

                    result.base = candidate.base;

                    return infra::MakeOptional(result);
                }

            return infra::Optional<Peripheral>();
        } };

    peripheralGroups.push_back(PeripheralGroup{ candidate.name, Compact(navigator / mcu / ip) });
}

std::string TableGenerator::Concatenate(const std::vector<std::string>& lines)
{
    std::string result;

    for (const auto& line : lines)
        result += line;

    return result;
}

int main(int argc, const char* argv[], const char* env[])
{
    args::ArgumentParser parser("Generate tables for ST microcontrollers");
    args::Group positionals(parser, "Positional arguments:");
    args::Positional<std::string> inputArgument(positionals, "input", "input xml file", args::Options::Required);
    args::Positional<std::string> directoryArgument(positionals, "directory", "directory for generated files", args::Options::Required);
    args::HelpFlag help(parser, "help", "display this help menu.", { 'h', "help" });

    try
    {
        parser.ParseCLI(argc, argv);

        hal::FileSystemGeneric fileSystem;

        std::vector<PeripheralCandidate> candidates{
            PeripheralCandidate{ "Uart", "USART_TypeDef*", { "UART", "USART" }, infra::none },
            PeripheralCandidate{ "Spi", "SPI_TypeDef*", { "SPI" }, infra::none },
            PeripheralCandidate{ "Timer", "TIM_TypeDef*", { "TIM1_8", "TIM6_7", "TIM1_8F7", "TIM6_7F7", "TIM1_8F37", "TIM6_7F37", "TIM1_8F77", "TIM6_7F77" }, infra::MakeOptional<std::string>("TIM") },
            PeripheralCandidate{ "Adc", "ADC_TypeDef*", { "ADC" }, infra::none },
            PeripheralCandidate{ "Rtc", "RTC_TypeDef*", { "RTC" }, infra::none },
        };

        TableGenerator tableGenerator(fileSystem, std::filesystem::path(args::get(inputArgument)), candidates);

        const auto outputDirectory = args::get(directoryArgument);
        tableGenerator.Write(outputDirectory);
    }
    catch (const args::Help&)
    {
        std::cout << parser;
        return 1;
    }
    //catch (const std::exception& ex)
    //{
    //    std::cout << ex.what() << std::endl;
    //    return 1;
    //}

    return 0;
}
