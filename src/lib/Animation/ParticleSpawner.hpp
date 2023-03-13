#pragma once

#include "lib/Animation/Particle.hpp"
#include <memory>
#include <optional>
class ParticleSpawner
{
public:
    ParticleSpawner(glm::vec3 position = {0.0f, 0.0f, 0.0f},
                    float spawnRate = 1.0f,
                    float particleMass = 1.0f);
    ParticleSpawner(const ParticleSpawner&) = delete;

    ~ParticleSpawner() = default;

    std::optional<std::shared_ptr<Particle>> spawnIfCan();

    std::shared_ptr<Particle> spawn();
    bool canSpawn();

private:
    glm::vec3 m_position = {0.0f, 0.0f, 0.0f};
    float m_spawnRate = 1.0f;
    float m_previousSpawnTime = 0.0f;

    float m_particleMass = 1.0f;
};
