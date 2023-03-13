#pragma once

#include "lib/Animation/Particle.hpp"
#include <memory>
#include <optional>

class Spring
{
public:
    Spring(float restDistance = 1.0f,
           std::optional<std::shared_ptr<Particle>> particle1 = {},
           std::optional<std::shared_ptr<Particle>> particle2 = {});
    
    Spring(const Spring&) = delete;

    ~Spring() = default;

    void applyConstrains();

    inline void setParticle1(std::shared_ptr<Particle> particle)
    {
        m_particle1 = particle;
    }

    inline void setParticle2(std::shared_ptr<Particle> particle)
    {
        m_particle2 = particle;
    }

    inline void removeParticle1() { m_particle1 = {}; }
    inline void removeParticle2() { m_particle2 = {}; }

private:
    std::optional<std::shared_ptr<Particle>> m_particle1 = {};
    std::optional<std::shared_ptr<Particle>> m_particle2 = {};

    float m_restDistance = 1.0f;
    float m_springForce = 1.0f;
};
