#include "lib/Animation/Particle.hpp"
#include "ext/vector_float3.hpp"
#include "fwd.hpp"
#include "lib/RenderObject.hpp"
#include "lib/Transform.hpp"
#include <memory>

Particle::Particle(glm::vec3 position, 
                   glm::vec3 velocity,
                   float mass,
                   std::optional<float> lifeTime)
    : m_position{position},
      m_previousPosition{position},
      m_velocity{velocity},
      m_previousVelocity{velocity},
      m_mass{mass},
      m_lifeTime{lifeTime}
{
    m_renderObject = std::make_shared<RenderObject>();
    m_renderObject->getMesh()->to_sphere(10, 10);

    auto& t = m_renderObject->getTransform();
    t.translate(m_position);
    t.scale(glm::vec3(0.3f));
}

void Particle::update(EulerMethode methode, float dt)
{
    if(m_locked)
    {
        m_acceleration = glm::vec3(0.0f);
        return;
    }

    addForce(GRAVITY * m_mass);

    switch (methode) 
    {
        case EulerMethode::Explicite:       updateExplicite(dt);        break;
        case EulerMethode::SemiImplicite:   updateSemiImplicite(dt);    break;
        case EulerMethode::Implicite:       updateImplicite(dt);        break;
    }

    m_velocity *= ATTENUATION;

    m_acceleration = glm::vec3(0.0f);

    if(m_lifeTime.has_value())
        m_lifeTime.value() -= dt;
}

void Particle::updateExplicite(float dt)
{
    m_previousVelocity = m_velocity;
    m_previousPosition = m_position;

    m_velocity = m_previousVelocity + (m_acceleration / m_mass) * dt;
    m_position = m_previousPosition + m_previousVelocity * dt;

    m_renderObject->getTransform().translate(m_position - m_previousPosition);
}

void Particle::updateSemiImplicite(float dt)
{
    m_previousVelocity = m_velocity;
    m_previousPosition = m_position;

    m_velocity = m_previousVelocity + (m_acceleration / m_mass) * dt;
    m_position = m_previousPosition + m_velocity * dt;

    m_renderObject->getTransform().translate(m_position - m_previousPosition);
}

void Particle::updateImplicite(float dt)
{
    (void)dt;
}
