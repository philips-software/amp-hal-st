#include "args.hxx"
#include "google/protobuf/compiler/code_generator.h"
#include "google/protobuf/compiler/cpp/helpers.h"
#include "google/protobuf/compiler/plugin.h"
#include "google/protobuf/io/printer.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/stubs/strutil.h"
#include "hal/generic/FileSystemGeneric.hpp"
#include "infra/syntax/CppFormatter.hpp"
#include "infra/syntax/XmlNavigator.hpp"
#include <fstream>
#include <ostream>
#include <sstream>

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
    std::string type;
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

    std::string MakeArrayType(const std::string& type, int size) const;
    std::string PeripheralInitializer(const std::vector<Peripheral>& peripherals) const;
    std::string IrqInitializer(const std::vector<Peripheral>& peripherals) const;
    std::string EnableClockBody(const std::vector<Peripheral>& peripherals) const;
    std::string DisableClockBody(const std::vector<Peripheral>& peripherals) const;

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

std::string ToUpper(const std::string& s)
{
    std::string result(s.size(), ' ');
    std::transform(s.begin(), s.end(), result.begin(), ::toupper);
    return result;
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

    application::IncludeGuard formatter("include_guard_hpp");

    auto includesByHeader = std::make_shared<application::IncludesByHeader>();
    includesByHeader->Path("infra/util/MemoryRange.hpp");
    includesByHeader->PathMacro("DEVICE_HEADER");
    formatter.Add(includesByHeader);

    auto includesBySource = std::make_shared<application::IncludesBySource>();
    includesBySource->Path("generated/stm32fxxx/PeripheralTable.hpp");
    includesBySource->PathSystem("array");
    includesBySource->PathSystem("cstdlib");
    formatter.Add(includesBySource);

    auto halNamespace = std::make_shared<application::Namespace>("hal");
    formatter.Add(halNamespace);

    for (const auto& group : peripheralGroups)
    {
        auto definitionName = "HAS_PERIPHERAL_" + ToUpper(group.groupName);
        auto variableName = "peripheral" + group.groupName;

        if (group.peripherals.empty())
            halNamespace->Add(std::make_shared<application::Undef>(definitionName));
        else
        {
            halNamespace->Add(std::make_shared<application::Define>(definitionName));
            halNamespace->Add(std::make_shared<application::SourceLocalVariable>(variableName + "Array", MakeArrayType("unsigned_int", group.peripherals.size()), PeripheralInitializer(group.peripherals)));
            halNamespace->Add(std::make_shared<application::ExternVariable>(variableName, "const infra::MemoryRange<" + group.type + "* const>", "infra::ReinterpretCastMemoryRange<" + group.type + "* const>(infra::MakeRange(" + variableName + "Array)"));
            halNamespace->Add(std::make_shared<application::SourceLocalVariable>(variableName + "IrqArray", MakeArrayType("IRQn_Type const", group.peripherals.size()), IrqInitializer(group.peripherals)));
            halNamespace->Add(std::make_shared<application::ExternVariable>(variableName + "Irq", "const infra::MemoryRange<IRQn_Type const>", "infra::MakeRange(" + variableName + "IrqArray"));
            auto EnableClock = std::make_shared<application::Function>("EnableClock" + group.groupName, EnableClockBody(group.peripherals), "void", 0);
            EnableClock->Parameter("std::size_t index");
            halNamespace->Add(EnableClock);
            auto DisableClock = std::make_shared<application::Function>("DisableClock" + group.groupName, DisableClockBody(group.peripherals), "void", 0);
            DisableClock->Parameter("std::size_t index");
            halNamespace->Add(DisableClock);
        }
    }

    std::ofstream header(outputDirectory / "PeripheralTable.chpp");
    std::unique_ptr<google::protobuf::io::ZeroCopyOutputStream> headerStream = std::make_unique<google::protobuf::io::OstreamOutputStream>(&header);
    google::protobuf::io::Printer headerPrinter(headerStream.get(), '$', nullptr);
    formatter.PrintHeader(headerPrinter);

    std::ofstream source(outputDirectory / "PeripheralTable.cpp");
    std::unique_ptr<google::protobuf::io::ZeroCopyOutputStream> sourceStream = std::make_unique<google::protobuf::io::OstreamOutputStream>(&source);
    google::protobuf::io::Printer sourcePrinter(sourceStream.get(), '$', nullptr);
    formatter.PrintSource(sourcePrinter, "");
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

    peripheralGroups.push_back(PeripheralGroup{ candidate.name, candidate.type, Compact(navigator / mcu / ip) });
}

std::string TableGenerator::MakeArrayType(const std::string& type, int size) const
{
    std::ostringstream stream;

    stream << "constexpr std::array<" << type << ", " << size << ">";

    return stream.str();
}

std::string TableGenerator::PeripheralInitializer(const std::vector<Peripheral>& peripherals) const
{
    std::ostringstream stream;

    stream << "\n  {{\n";
    for (auto& p : peripherals)
        stream << "    " << p.name << ",\n";
    stream << "  }}";

    return stream.str();
}

std::string TableGenerator::IrqInitializer(const std::vector<Peripheral>& peripherals) const
{
    std::ostringstream stream;

    stream << "\n  {{\n";
    for (auto& p : peripherals)
        stream << "    " << p.name << "_IRQn,\n";
    stream << "  }}";

    return stream.str();
}

std::string TableGenerator::EnableClockBody(const std::vector<Peripheral>& peripherals) const
{
    std::ostringstream stream;

    stream << "switch (index)\n{\n";

    for (auto& p : peripherals)
        if (p.clockEnable)
            stream << "  case " << std::distance(peripherals.data(), &p) << ": " << *p.clockEnable << "(); break;\n";
        else
            stream << "  case " << std::distance(peripherals.data(), &p) << ": __HAL_RCC_" << p.name << "_CLK_ENABLE(); break;\n";

    stream << "  default:\n    std::abort();\n}";

    return stream.str();
}

std::string TableGenerator::DisableClockBody(const std::vector<Peripheral>& peripherals) const
{
    std::ostringstream stream;

    stream << "switch (index)\n{\n";

    for (auto& p : peripherals)
        if (p.clockEnable)
            stream << "  case " << std::distance(peripherals.data(), &p) << ": " << p.clockEnable->substr(0, p.clockEnable->size() - 6)  << "DISABLE(); break;\n";
        else
            stream << "  case " << std::distance(peripherals.data(), &p) << ": __HAL_RCC_" << p.name << "_CLK_DISABLE(); break;\n";

    stream << "  default:\n    std::abort();\n}";

    return stream.str();
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
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
