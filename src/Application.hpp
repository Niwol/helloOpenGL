#pragma once

#include <iostream>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "lib/renderer.hpp"
#include "lib/Camera.hpp"

class Runable;

class Application
{
public:
    struct Options
    {
        int width = 0;
        int height = 0;
    };

    struct AppUtils
    {
        float delatTime = 0.0f;
        GLFWwindow *window = nullptr;

        std::shared_ptr<Renderer> renderer;
        std::shared_ptr<Camera> camera;
    };

public:
    Application();
    Application(const Application&) = delete;

    ~Application();

    bool construct(std::unique_ptr<Runable> runable, const Application::Options& options);

    void run();

private:
    GLFWwindow *m_window = nullptr; 
    std::unique_ptr<Runable> m_runable;
    std::shared_ptr<Renderer> m_renderer;
    std::shared_ptr<Camera> m_camera;

    AppUtils m_appUtils;
};

class Runable
{
public:
    Runable() = default;
    virtual ~Runable() = default;

    virtual bool onCreate() = 0;
    virtual bool onUpdate(Application::AppUtils& appUtils) = 0;
};
