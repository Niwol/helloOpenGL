#pragma once

#include "Application.hpp"
#include "lib/Camera.hpp"
#include "lib/EventHandler.hpp"
#include "lib/RenderObject.hpp"
#include "lib/Scene.hpp"
#include "lib/ShaderProgram.hpp"
#include "lib/renderer.hpp"

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

    RenderMode m_renderMode = RenderMode::Default;


    std::shared_ptr<RenderObject> m_selectedObj;
    float m_metallic = 0.0f;
    float m_roughtness = 0.0f;

    bool m_quit = false;
};
