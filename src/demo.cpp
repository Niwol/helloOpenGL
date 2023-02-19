#include "demo.hpp"
#include "ext/matrix_transform.hpp"
#include "ext/scalar_constants.hpp"
#include "fwd.hpp"
#include "lib/Camera.hpp"
#include "lib/EventHandler.hpp"
#include "lib/Light/Light.hpp"
#include "lib/Light/PointLight.hpp"
#include "lib/Materials/BlinnPhong.hpp"
#include "lib/Materials/MetallicRought.hpp"
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
    m_camera = Camera({0.0f, 0.0f, 5.0f});
    int width, height;
    glfwGetWindowSize(appUtils.window, &width, &height);
    m_camera.setWidth(width);
    m_camera.setHeight(height);

    scene1();

    setCallbacks(appUtils.window);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    return true;
}

bool Demo::onUpdate(Application::AppUtils &appUtils)
{
    handleInput(*(appUtils.eventHandler), appUtils.delatTime);

    appUtils.renderer->render(m_scene, m_camera, m_renderMode);
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
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            //glDisable(GL_CULL_FACE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            //glEnable(GL_CULL_FACE);
        }
    }

    if(event.getKeyState(GLFW_KEY_N) & PRESSED)
        m_renderMode = RenderMode::Normal;

    if(event.getKeyState(GLFW_KEY_B) & PRESSED)
        m_renderMode = RenderMode::Depth;

    if(event.getKeyState(GLFW_KEY_V) & PRESSED)
        m_renderMode = RenderMode::Default;


    // object color
    if(event.getKeyState(GLFW_KEY_P) & PRESSED)
    {
        auto mat = static_cast<MetallicRoughtness*>(m_selectedObj->getMaterial().get());
        if(mat->m_metallic < 1.0f)
            mat->m_metallic += 0.1f;
    }
    if(event.getKeyState(GLFW_KEY_SEMICOLON) & PRESSED)
    {
        auto mat = static_cast<MetallicRoughtness*>(m_selectedObj->getMaterial().get());
        if(mat->m_metallic > 0.0f)
            mat->m_metallic -= 0.1f;
    }
    if(event.getKeyState(GLFW_KEY_O) & PRESSED)
    {
        auto mat = static_cast<MetallicRoughtness*>(m_selectedObj->getMaterial().get());
        if(mat->m_roughtness < 1.0f)
            mat->m_roughtness += 0.1f;
    }
    if(event.getKeyState(GLFW_KEY_L) & PRESSED)
    {
        auto mat = static_cast<MetallicRoughtness*>(m_selectedObj->getMaterial().get());
        if(mat->m_roughtness > 0.0f)
            mat->m_roughtness -= 0.1f;
    }


    // Scene
    if(event.getKeyState(GLFW_KEY_KP_1) & PRESSED)
        scene1();
    if(event.getKeyState(GLFW_KEY_KP_2) & PRESSED)
        scene2();
    if(event.getKeyState(GLFW_KEY_KP_3) & PRESSED)
        scene3();
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

void Demo::scene1()
{
    m_scene.objects.clear();
    m_scene.lights.clear();
    
    {
        auto obj = std::make_shared<RenderObject>();
        obj->getMesh()->to_sharp_cube();
        m_scene.objects.push_back(obj);
    }

    {
        auto obj = std::make_shared<RenderObject>();
        obj->getMesh()->to_sharp_cube();

        auto t = glm::mat4(1.0);
        t = glm::translate(t, glm::vec3(6.0f, -4.0f, -2.0f));
        t = glm::scale(t, glm::vec3(8.0f));

        obj->setTransform(t);

        auto material = std::make_shared<BlinnPhong>();
        
        material->m_diffuseTexture.load("../textures/container2.png");
        material->m_specularTexture.load("../textures/container2_specular.png");
        material->m_hasTexture = true;
        material->m_shininess = 128;

        obj->setMaterial(material);

        m_scene.objects.push_back(obj);
    }
    
    {
        auto obj = std::make_shared<RenderObject>();
        obj->getMesh()->to_sharp_cube();

        auto t = glm::mat4(1.0);
        t = glm::translate(t, glm::vec3(10.0f, 0.0f, 5.0f));
        t = glm::scale(t, glm::vec3(3.0f));

        obj->setTransform(t);

        auto material = std::make_shared<MetallicRoughtness>();
        
        material->m_albedo = {1.0f, 0.0f, 1.0f};
        material->m_roughtness = 0.8f;
        material->m_metallic = 0.0;

        obj->setMaterial(material);

        m_selectedObj = obj;

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
        light->m_position = {5.0f, 5.0f, 2.0f};
        light->m_color = {1.0f, 1.0f, 1.0f};
        lights.push_back(light);
    }

    {
        auto light = std::make_shared<PointLight>();
        light->m_position = {6.0f, 3.0f, 0.0f};
        light->m_color = {1.0f, 1.0f, 1.0f};
        lights.push_back(light);
    }

    {
        auto light = std::make_shared<PointLight>();
        light->m_position = {1.5f, 3.0f, 0.0f};
        light->m_color = {1.0f, 0.0f, 0.0f};
        lights.push_back(light);
    }
    
    {
        auto light = std::make_shared<PointLight>();
        light->m_position = {10.0f, 1.0f, 10.0f};
        light->m_color = {0.0f, 1.0f, 0.0f};
        lights.push_back(light);
    }

    {
        auto light = std::make_shared<PointLight>();
        light->m_position = {20.0f, 1.0f, 1.0f};
        light->m_color = {0.0f, 0.0f, 1.0f};
        lights.push_back(light);
    }

    for(auto& light : lights)
        m_scene.lights.push_back(light);

}

void Demo::scene2()
{
    m_scene.objects.clear();
    m_scene.lights.clear();

    {
        auto obj = std::make_shared<RenderObject>();
        obj->getMesh()->to_square();

        auto t = glm::mat4(1.0f);
        t = glm::rotate(t, -glm::pi<float>() / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));

        obj->setTransform(t);
        m_scene.objects.push_back(obj);
    }

    {
        auto light = std::make_shared<PointLight>();
        light->m_position = {0.0f, 2.0f, 0.0f};
        m_scene.lights.push_back(light);
    }
}

void Demo::scene3()
{
    m_scene.objects.clear();
    m_scene.lights.clear();

    std::vector<GLfloat> vertices = 
    {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.3f, 0.0f,
    };

    std::vector<GLfloat> normals = 
    {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
    };

    std::vector<GLuint> indices =
    {
        0, 1, 2,
    };

    {
        auto obj = std::make_shared<RenderObject>();
        auto mesh = obj->getMesh();
        mesh->set_vertices(vertices);
        mesh->set_normals(normals);
        mesh->set_indices(indices, GL_TRIANGLES);

        auto t = glm::mat4(1.0f);
        t = glm::rotate(t, -glm::pi<float>() / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));

        obj->setTransform(t);
        m_scene.objects.push_back(obj);
    }

    {
        auto obj = std::make_shared<RenderObject>();
        obj->getMesh()->to_square();
        
        auto t = glm::mat4(1.0f);
        t = glm::translate(t, glm::vec3(0.0f, -2.0f, 0.0f));
        t = glm::rotate(t, -glm::pi<float>() / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        t = glm::scale(t, glm::vec3(10.0f));

        obj->setTransform(t);

        m_scene.objects.push_back(obj);
    }

    {
        auto light = std::make_shared<PointLight>();
        light->m_position = {0.0f, 2.0f, 0.0f};
        m_scene.lights.push_back(light);
    }
}
