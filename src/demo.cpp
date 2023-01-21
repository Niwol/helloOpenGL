#include "demo.hpp"
#include "lib/RenderObject.hpp"
#include "lib/ShaderProgram.hpp"
#include <memory>

Demo::Demo() {}
Demo::~Demo() {}

bool Demo::onCreate()
{
    auto sp = std::make_shared<ShaderProgram>();
    if(sp->createProgram("../src/shaders/simpleShader.vs", 
                         "../src/shaders/simpleShader.fs"));


    auto object = std::make_shared<RenderObject>();
    
    return true;
}

bool Demo::onUpdate(Application::AppUtils &appUtils)
{

    return true;
}

void Demo::handleInput(GLFWwindow *window)
{

}

