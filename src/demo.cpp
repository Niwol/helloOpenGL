#include "demo.hpp"
#include "ext/matrix_transform.hpp"
#include "fwd.hpp"
#include "lib/Animation/Particle.hpp"
#include "lib/Animation/ParticleSpawner.hpp"
#include "lib/Camera.hpp"
#include "lib/EventHandler.hpp"
#include "lib/Light/DirectionalLight.hpp"
#include "lib/Light/Light.hpp"
#include "lib/Light/PointLight.hpp"
#include "lib/Light/SpotLight.hpp"
#include "lib/Materials/BlinnPhong.hpp"
#include "lib/Materials/MetallicRought.hpp"
#include "lib/RenderObject.hpp"
#include "lib/ShaderProgram.hpp"
#include "lib/Transform.hpp"
#include "lib/renderer.hpp"
#include <GLFW/glfw3.h>
#include <memory>
#include <optional>

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
    initParticleSystems();

    setCallbacks(appUtils.window);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    return true;
}

bool Demo::onUpdate(Application::AppUtils &appUtils)
{
    handleInput(*(appUtils.eventHandler), appUtils.delatTime);

    updateParticles(appUtils.delatTime);

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

    if(event.getMouseButtonState(GLFW_MOUSE_BUTTON_2) & PRESSED)
    {
        event.getMouseState(&mouseX, &mouseY);
    }

    if(event.getMouseButtonState(GLFW_MOUSE_BUTTON_2) & HELD)
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


    // Particles
    if(event.getMouseButtonState(GLFW_MOUSE_BUTTON_1) & PRESSED)
    {
        auto particle = std::make_shared<Particle>(m_camera.getPosition() + m_camera.getFront(),
                                                   m_camera.getFront() * 50.0f,
                                                   1.0f,
                                                   20.0f);

        m_particles.insert(particle);
        m_scene.objects.insert(particle->getRenderObject());
    }


    // Scene
    if(event.getKeyState(GLFW_KEY_KP_1) & PRESSED)
    {
        scene1();
        initParticleSystems();
    }
    if(event.getKeyState(GLFW_KEY_KP_2) & PRESSED)
    {
        scene2();
        initParticleSystems();
    }
    if(event.getKeyState(GLFW_KEY_KP_3) & PRESSED)
    {
        scene3();
        initParticleSystems();
    }
    if(event.getKeyState(GLFW_KEY_KP_4) & PRESSED)
    {
        scene4();
        initParticleSystems();
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

void Demo::initParticleSystems()
{
    m_particles.clear();

    m_spawner1 = std::make_shared<ParticleSpawner>(glm::vec3(1.0f, 1.0f, 3.0f), 0.05f, 1.0f);
    m_spawner2 = std::make_shared<ParticleSpawner>(glm::vec3(5.0f, 1.0f, 3.0f), 0.05f, 100.0f);



    auto particle1 = std::make_shared<Particle>(glm::vec3(0.0f, 10.0f, -4.0f),
                                               glm::vec3(0.0f),
                                               1.0f,
                                               std::optional<float>());

    particle1->lock();
    m_particles.insert(particle1);

    auto particle2 = std::make_shared<Particle>(glm::vec3(-3.0f, 3.0f, -4.0f),
                                               glm::vec3(0.0f),
                                               0.1f,
                                               std::optional<float>());

    //particle2->lock();
    m_particles.insert(particle2);

    auto particle3 = std::make_shared<Particle>(glm::vec3(0.0f, 3.0f, -4.0f),
                                               glm::vec3(0.0f),
                                               0.2f,
                                               std::optional<float>());

    m_particles.insert(particle3);

    m_spring1 = Spring(0.2f, particle1, particle3);
    m_spring2 = Spring(1.0f, particle2, particle3);

    for(auto& p : m_particles)
        m_scene.objects.insert(p->getRenderObject());
}

void Demo::updateParticles(float dt)
{
    if(auto p = m_spawner1->spawnIfCan(); p.has_value())
    {
        m_particles.insert(p.value());
        m_scene.objects.insert(p.value()->getRenderObject());
    }

    if(auto p = m_spawner2->spawnIfCan(); p.has_value())
    {
        m_particles.insert(p.value());
        m_scene.objects.insert(p.value()->getRenderObject());
    }

    std::vector<std::shared_ptr<Particle>> deadParticles;

    m_spring1.applyConstrains();
    m_spring2.applyConstrains();

    for(auto& p : m_particles)
    {
        p->update(EulerMethode::SemiImplicite, dt);

        if(p->isDead())
        {
            deadParticles.push_back(p);
        }
    }

    for(auto& p : deadParticles)
    {
        m_scene.objects.erase(p->getRenderObject());
        m_particles.erase(p);
    }
}

void Demo::scene1()
{
    m_scene.objects.clear();
    m_scene.lights.clear();
    
    {
        auto obj = std::make_shared<RenderObject>();
        obj->getMesh()->to_sphere();
        m_scene.objects.insert(obj);
    }

    {
        auto obj = std::make_shared<RenderObject>();
        obj->getMesh()->to_sharp_cube();

        auto t = glm::mat4(1.0);
        t = glm::translate(t, glm::vec3(6.0f, -4.0f, -2.0f));
        t = glm::scale(t, glm::vec3(8.0f));

        Transform transform;
        transform.translate({6.0f, -4.0f, -2.0f});
        transform.scale(glm::vec3(8.0f));

        obj->setTransform(transform);

        auto material = std::make_shared<BlinnPhong>();
        
        material->m_diffuseTexture.load("../textures/container2.png");
        material->m_specularTexture.load("../textures/container2_specular.png");
        material->m_hasTexture = true;
        material->m_shininess = 128;

        obj->setMaterial(material);

        m_scene.objects.insert(obj);
    }
    
    {
        auto obj = std::make_shared<RenderObject>();
        obj->getMesh()->to_sphere();

        Transform t;
        t.translate({10.0f, 0.0f, 5.0f});
        t.scale(glm::vec3(3.0f));

        obj->setTransform(t);

        auto material = std::make_shared<MetallicRoughtness>();
        
        material->m_albedo = {1.0f, 0.0f, 1.0f};
        material->m_roughtness = 0.8f;
        material->m_metallic = 0.0;

        obj->setMaterial(material);

        m_selectedObj = obj;

        m_scene.objects.insert(obj);
    }

    {
        auto obj = std::make_shared<RenderObject>();
        obj->getMesh()->to_square();
        
        Transform transform;

        transform.rotate({1.0f, 0.0f, 0.0f}, -M_PI / 2.0f);
        transform.scale(glm::vec3(500.0f));
        transform.translate({0.0f, -2.0f, 0.0f});

        obj->setTransform(transform);

        m_scene.objects.insert(obj);
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

        Transform t;
        t.translate({0.0f, -2.0f, 0.0f});
        t.rotate({1.0f, 0.0f, 0.0f}, -M_PI / 2.0f);
        t.scale(glm::vec3(500.0f));

        obj->setTransform(t);

        m_scene.objects.insert(obj);
    }

    int nbRows = 10;
    int nbCols = 10;

    for(int i = 0; i < nbRows; i++)
    {
        for(int j = 0; j < nbCols; j++)
        {
            auto obj = std::make_shared<RenderObject>();
            obj->getMesh()->to_sphere();

            auto material = std::make_shared<MetallicRoughtness>();
            material->m_metallic = float(i) / float(nbRows - 1);
            material->m_roughtness = float(j) / float(nbCols - 1);
            material->m_albedo = {1.0f, 0.0f, 1.0f};
            obj->setMaterial(material);

            Transform t;
            t.translate({i * 2.2f - nbCols, j * 2.0f + 1.0f, -3.0f});
            t.scale(glm::vec3(1.5f));

            obj->setTransform(t);

            m_scene.objects.insert(obj);
        }
    }

    {
        auto light = std::make_shared<DirectionalLight>();
        light->m_direction = {0.2f, -1.0f, -0.3f};
        m_scene.lights.push_back(light);
    }
}

void Demo::scene3()
{
    m_scene.objects.clear();
    m_scene.lights.clear();

    // Floor
    {
        auto obj = std::make_shared<RenderObject>();
        obj->getMesh()->to_square();
        
        Transform t;
        t.translate({0.0f, -2.0f, 0.0f});
        t.rotate({1.0f, 0.0f, 0.0f}, -M_PI / 2.0f);
        t.scale(glm::vec3(500.0f));

        obj->setTransform(t);

        m_scene.objects.insert(obj);
    }

    {
        auto obj = std::make_shared<RenderObject>();
        obj->getMesh()->to_sphere();

        auto material = std::make_shared<BlinnPhong>();
        material->m_diffuseTexture.load("../textures/world.jpg");
        material->m_hasTexture = true;
        obj->setMaterial(material);

        Transform t;
        t.translate({0.0f, 2.0f, 0.0f});
        t.scale(glm::vec3(3.0f));

        obj->setTransform(t);

        m_scene.objects.insert(obj);
    }

//    {
//        auto light = std::make_shared<PointLight>();
//        light->m_position = {-2.0f, 5.0f, 2.0f};
//        m_scene.lights.push_back(light);
//    }

    {
        auto light = std::make_shared<SpotLight>();
        light->m_position = {2.0f, 5.0f, 0.0f};
        light->m_direction = {-0.2f, -1.0f, 0.0f};
        m_scene.lights.push_back(light);
    }

    {
        auto light = std::make_shared<SpotLight>();
        light->m_position = {-1.0f, 4.0f, 2.0f};
        light->m_direction = {0.2f, -1.0f, -0.7f};
        m_scene.lights.push_back(light);
    }

    {
        auto light = std::make_shared<SpotLight>();
        light->m_position = {-1.0f, 6.0f, -3.0f};
        light->m_direction = {0.3f, -1.0f, 0.5f};
        m_scene.lights.push_back(light);
    }
}

void Demo::scene4()
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

    std::vector<GLfloat> uv =
    {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.5f, 0.8f,
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
        mesh->set_uvCoords(uv);
        mesh->set_indices(indices, GL_TRIANGLES);

        Transform t;
        t.rotate({1.0f, 0.0f, 0.0f}, -M_PI / 2.0f);

        obj->setTransform(t);
        m_scene.objects.insert(obj);
    }

    {
        auto obj = std::make_shared<RenderObject>();
        obj->getMesh()->to_square();

        Transform t;
        t.translate({0.0f, -2.0f, 0.0f});
        t.rotate({1.0f, 0.0f, 0.0f}, -M_PI / 2.0f);
        t.scale(glm::vec3(10.0f));
        
        obj->setTransform(t);

        m_scene.objects.insert(obj);
    }

    {
        auto light = std::make_shared<PointLight>();
        light->m_position = {0.0f, 2.0f, 0.0f};
        m_scene.lights.push_back(light);
    }
}

