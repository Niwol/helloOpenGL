#include <iostream>
#include <memory>
#include <utility>

#include "Application.hpp"
#include "demo.hpp"

int main(int, char**) {

    std::cout << "Hello OpenGL!" << std::endl;

    Application app;
    std::unique_ptr<Demo> demo = std::make_unique<Demo>();

    Application::Options options;
    options.width = 800;
    options.height = 600;

    if(app.construct(std::move(demo), options))
        app.run();

    return EXIT_SUCCESS;
}

