#include "cucumber_cpp/library/Application.hpp"

int main(int argc, char** argv)
{
    cucumber_cpp::library::Application application{};

    application.CliParser().add_option("--target", *application.ProgramContext().EmplaceAt<std::string>("target"), "COM port or hostname (ws://<host>/path or tcp://<host>) of the amp-hal-st integration test board")->required();

    return application.Run(argc, argv);
}
