#pragma once

#include "Application.hpp"
#include "lib/Camera.hpp"
#include "lib/EventHandler.hpp"
#include "lib/Scene.hpp"
#include "lib/ShaderProgram.hpp"

#include <GLFW/glfw3.h>
#include <memory>

class Demo : public Runable
{
public:
    Demo();
    Demo(const Demo&) = delete;
    virtual ~Demo() override;

    bool onCreate(Application::AppUtils& appUtils) override;
    bool onUpdate(Application::AppUtils &appUtils) override;

    void handleInput(EventHandler& event, float dt);
    void setCallbacks(GLFWwindow* window);

private:
    Scene m_scene;
    Camera m_camera;

    bool m_drawLines = false;

    bool m_quit = false;
};
