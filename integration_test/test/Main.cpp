#include "cucumber-cpp/CucumberRunner.hpp"
#include "cucumber-cpp/Hooks.hpp"
#include "cucumber-cpp/Matchers.hpp"
#include "cucumber-cpp/Steps.hpp"
#include "cucumber-cpp/report/JunitReport.hpp"
#include "cucumber-cpp/report/StdOutReport.hpp"
#include <gherkin/app.hpp>
#include <gherkin/file.hpp>
#include <iostream>

int main(int argc, char** argv)
{
    nlohmann::json root;
    root["tagexpression"] = argv[1];
    {
        cucumber_cpp::Hooks hooks;
        cucumber_cpp::StepRepository stepRepository;
        cucumber_cpp::CucumberRunner cucumberRunner{ hooks, stepRepository, argv[1] };

        gherkin::app app;
        gherkin::app::callbacks cbs{
            .pickle = [&](const cucumber::messages::pickle& pickle)
            {
                nlohmann::json j;

                pickle.to_json(j);

                cucumberRunner.Run(j);

                root["scenarios"].push_back(j);
                // std::cout << j << "\n";
            },
            .error = [&](const auto& m)
            {
                std::cout << m.to_json() << std::endl;
            }
        };

        app.include_source(false);
        app.include_ast(false);
        app.include_pickles(true);

        for (auto i = 2; i < argc; ++i)
        {
            app.parse(gherkin::file{ argv[i] }, cbs);
        }
    }

    std::cout << "\n\n\n========================\n\n\n";

    cucumber_cpp::report::StdOutReport report;
    report.GenerateReport(root);

    cucumber_cpp::report::JunitReport junitReport;
    junitReport.GenerateReport(root);

    return 0;
}
