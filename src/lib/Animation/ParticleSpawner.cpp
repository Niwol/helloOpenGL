#include "lib/Animation/ParticleSpawner.hpp"
#include "ext/vector_float3.hpp"
#include "fwd.hpp"
#include "lib/Animation/Particle.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdlib>
#include <memory>
#include <optional>

ParticleSpawner::ParticleSpawner(glm::vec3 position, 
                                 float spawnRate, 
                                 float particleMass)
    : m_position{position},
      m_spawnRate{spawnRate},
      m_particleMass{particleMass}
{}

std::optional<std::shared_ptr<Particle>> ParticleSpawner::spawnIfCan()
{
    if(canSpawn())
        return spawn();

    return {};
}

glm::vec3 randomUnit()
{
    float r1 = float(std::rand()) / float(RAND_MAX);
    float r2 = float(std::rand()) / float(RAND_MAX);

    float x = std::cos(2.0f * M_PI * r1) * 2.0f * std::sqrt(r2 * (1.0f - r2));
    float y = std::sin(2.0f * M_PI * r1) * 2.0f * std::sqrt(r2 * (1.0f - r2));
    float z = 1.0f - 2.0f * r2;

    return glm::vec3(x, y, z);
}

std::shared_ptr<Particle> ParticleSpawner::spawn()
{
    glm::vec3 velocity = {0.0f, 1.0f, 0.0f};
    velocity *= 2.0f;
    velocity += randomUnit() * 2.0f;

    auto particle = std::make_shared<Particle>(m_position, velocity, m_particleMass, 2.0f);

    m_previousSpawnTime = glfwGetTime();
    return particle;
}

bool ParticleSpawner::canSpawn()
{
    return glfwGetTime() - m_previousSpawnTime > m_spawnRate;
}

