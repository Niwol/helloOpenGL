#include "lib/Animation/Spring.hpp"
#include "ext/quaternion_geometric.hpp"
#include "lib/Animation/Particle.hpp"
#include <memory>
#include <optional>

Spring::Spring(float restDistance,
               std::optional<std::shared_ptr<Particle>> particle1,
               std::optional<std::shared_ptr<Particle>> particle2)
    : m_particle1{particle1},
      m_particle2{particle2},
      m_restDistance{restDistance}
{}

void Spring::applyConstrains()
{
    if((m_particle1.has_value() && m_particle2.has_value()) == false)
        return;

    const glm::vec3 p1 = m_particle1.value()->m_position;
    const glm::vec3 p2 = m_particle2.value()->m_position;

    const glm::vec3 p1p2 = p2 - p1;
    const float offDistance = glm::length(p1p2) - m_restDistance;
    const glm::vec3 p1p2Normalized = glm::normalize(p1p2);
    const glm::vec3 force = p1p2Normalized * m_springForce * offDistance;

    m_particle1.value()->addForce(force);
    m_particle2.value()->addForce(-force);
}

