#include "demo.hpp"
#include "ext/matrix_transform.hpp"
#include "ext/scalar_constants.hpp"
#include "fwd.hpp"
#include "lib/Camera.hpp"
#include "lib/EventHandler.hpp"
#include "lib/Light/Light.hpp"
#include "lib/Light/PointLight.hpp"
#include "lib/RenderObject.hpp"
#include "lib/ShaderProgram.hpp"
#include "lib/renderer.hpp"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <memory>

Demo::Demo() {}
Demo::~Demo() {}

bool Demo::onCreate(Application::AppUtils& appUtils)
{
    // Camera
    m_camera = Camera({0.0f, 0.0f, -5.0f});
    int width, height;
    glfwGetWindowSize(appUtils.window, &width, &height);
    m_camera.setWidth(width);
    m_camera.setHeight(height);

    // Scene
    {
        auto obj = std::make_shared<RenderObject>();
        obj->getMesh()->to_sharp_cube();
        m_scene.objects.push_back(obj);
    }

    {
        auto obj = std::make_shared<RenderObject>();
        obj->getMesh()->to_square();
        
        auto t = glm::mat4(1.0f);
        t = glm::translate(t, glm::vec3(0.0f, -2.0f, 0.0f));
        t = glm::rotate(t, -glm::pi<float>() / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        t = glm::scale(t, glm::vec3(500.0f));

        obj->setTransform(t);

        m_scene.objects.push_back(obj);
    }





    std::vector<std::shared_ptr<Light>> lights;
    
    {
        auto light = std::make_shared<PointLight>();
        light->m_position = {1.5f, 3.0f, 0.0f};
        lights.push_back(light);
    }
    
    {
        auto light = std::make_shared<PointLight>();
        light->m_position = {20.0f, 1.0f, 1.0f};
        lights.push_back(light);
    }

    for(auto& light : lights)
        m_scene.lights.push_back(light);

    setCallbacks(appUtils.window);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    return true;
}

bool Demo::onUpdate(Application::AppUtils &appUtils)
{
    handleInput(*(appUtils.eventHandler), appUtils.delatTime);


    appUtils.renderer->render(m_scene, m_camera, RenderMode::Default);
    glfwSwapBuffers(appUtils.window);

    return !m_quit;
}

void Demo::handleInput(EventHandler& event, float dt)
{
    if(event.getKeyState(GLFW_KEY_ESCAPE) & PRESSED)
        m_quit = true;

    if(event.getKeyState(GLFW_KEY_W) & (HELD | PRESSED))
    {
        m_camera.move(CameraMovement::FORWARD_XZ, dt);
    }

    if(event.getKeyState(GLFW_KEY_S) & (HELD | PRESSED))
    {
        m_camera.move(CameraMovement::BACKWARD_XZ, dt);
    }

    if(event.getKeyState(GLFW_KEY_A) & (HELD | PRESSED))
    {
        m_camera.move(CameraMovement::LEFT, dt);
    }

    if(event.getKeyState(GLFW_KEY_D) & (HELD | PRESSED))
    {
        m_camera.move(CameraMovement::RIGHT, dt);
    }

    if(event.getKeyState(GLFW_KEY_SPACE) & (HELD | PRESSED))
    {
        m_camera.move(CameraMovement::UP, dt);
    }

    if(event.getKeyState(GLFW_KEY_LEFT_CONTROL) & (HELD | PRESSED))
    {
        m_camera.move(CameraMovement::DOWN, dt);
    }

    // Rotation
    static double mouseX = 0.0;
    static double mouseY = 0.0;

    if(event.getMouseButtonState(GLFW_MOUSE_BUTTON_1) & PRESSED)
    {
        event.getMouseState(&mouseX, &mouseY);
    }

    if(event.getMouseButtonState(GLFW_MOUSE_BUTTON_1) & HELD)
    {
        double x, y;
        
        event.getMouseState(&x, &y);

        int dx = int(x - mouseX);
        int dy = int(y - mouseY);

        m_camera.handleMouseMovement(dx, dy);

        mouseX = x;
        mouseY = y;
    }


    // Render mode
    if(event.getKeyState(GLFW_KEY_Z) & PRESSED)
    {
        m_drawLines = !m_drawLines;
        
        if(m_drawLines)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void Demo::setCallbacks(GLFWwindow* window)
{
    glfwSetFramebufferSizeCallback(window, 
                    [](GLFWwindow* window, int width, int height)
    {
        Demo* self = static_cast<Demo*>(glfwGetWindowUserPointer(window));

        glViewport(0, 0, width, height);
        
        self->m_camera.setWidth(width);
        self->m_camera.setHeight(height);
    });
}

