#pragma once

#include "fwd.hpp"
#include "lib/RenderObject.hpp"
#include "lib/Transform.hpp"
#include <memory>
#include <optional>

#define GRAVITY glm::vec3(0.0f, -9.81f, 0.0f)
#define ATTENUATION 0.999


enum class EulerMethode
{
    Explicite,
    SemiImplicite,
    Implicite,
};

class Particle
{
    friend class Spring;

public:
    Particle(glm::vec3 position = {0.0f, 0.0f, 0.0f},
             glm::vec3 velocity = {0.0f, 0.0f, 0.0f},
             float mass = 1.0f,
             std::optional<float> lifeTime = {});

    Particle(const Particle&) = delete;

    ~Particle() = default;


    void update(EulerMethode methode, float dt);

public:
    inline void setPosition(glm::vec3 position)
    {
        m_position = position;
        m_previousPosition = position;

        Transform t;
        t.translate(m_position);
        m_renderObject->setTransform(t);
    }

    inline void setVelocity(glm::vec3 velocity)
    {
        m_velocity = velocity;
        m_previousVelocity = velocity;
    }

    inline void addForce(glm::vec3 force)
    {
        m_acceleration += force;
    }

    inline std::shared_ptr<RenderObject> getRenderObject()
    {
        return m_renderObject;
    }

    inline void lock() { m_locked = true; }
    inline void unlock() { m_locked = false; }

    inline bool isDead()
    {
        if(m_lifeTime.has_value())
            return m_lifeTime.value() <= 0.0f;

        return false;
    }

private:
    void updateExplicite(float dt);
    void updateSemiImplicite(float dt);
    void updateImplicite(float dt);

private:
    std::shared_ptr<RenderObject> m_renderObject;

    glm::vec3 m_position = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_previousPosition = {0.0f, 0.0f, 0.0f};

    glm::vec3 m_velocity = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_previousVelocity = {0.0f, 0.0f, 0.0f};

    glm::vec3 m_acceleration = {0.0f, 0.0f, 0.0f};

    float m_mass = 1.0f;

    std::optional<float> m_lifeTime = {};
    bool m_locked = false;
};
