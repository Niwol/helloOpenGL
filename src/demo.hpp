#pragma once

#include "Application.hpp"
#include "lib/Animation/Particle.hpp"
#include "lib/Animation/ParticleSpawner.hpp"
#include "lib/Animation/Spring.hpp"
#include "lib/Camera.hpp"
#include "lib/EventHandler.hpp"
#include "lib/RenderObject.hpp"
#include "lib/Scene.hpp"
#include "lib/ShaderProgram.hpp"
#include "lib/renderer.hpp"

#include <GLFW/glfw3.h>
#include <memory>
#include <set>
#include <vector>

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

    void initParticleSystems();
    void updateParticles(float dt);

    void scene1();
    void scene2();
    void scene3();
    void scene4();

private:
    Scene m_scene;
    Camera m_camera;

    bool m_drawLines = false;

    RenderMode m_renderMode = RenderMode::Default;

    std::shared_ptr<ParticleSpawner> m_spawner1;
    std::shared_ptr<ParticleSpawner> m_spawner2;
    std::set<std::shared_ptr<Particle>> m_particles;

    Spring m_spring1;
    Spring m_spring2;

    bool m_quit = false;
};
