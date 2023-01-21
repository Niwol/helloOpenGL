#pragma once

#include "Application.hpp"
#include "lib/Scene.hpp"

#include <GLFW/glfw3.h>

class Demo : public Runable
{
public:
    Demo();
    Demo(const Demo&) = delete;
    virtual ~Demo() override;

    bool onCreate() override;
    bool onUpdate(Application::AppUtils &appUtils) override;

    void handleInput(GLFWwindow *window);

private:
    Scene m_scene;
};
