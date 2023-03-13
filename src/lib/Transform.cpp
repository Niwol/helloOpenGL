#include "lib/Transform.hpp"
#include "ext/matrix_transform.hpp"
#include "fwd.hpp"

Transform::Transform()
    : m_position{glm::vec3(0.0f)},
      m_scale{glm::vec3(1.0f)},
      m_rotation{glm::mat4(1.0f)},
      m_model{glm::mat4(1.0f)}
{}

Transform::Transform(const Transform& other)
    : m_position{other.m_position},
      m_scale{other.m_scale},
      m_rotation{other.m_rotation},
      m_model{other.m_model}
{}


void Transform::translate(glm::vec3 translation)
{
    m_position += translation;
    updateModel();
}

void Transform::scale(glm::vec3 scaler)
{
    m_scale *= scaler;
    updateModel();
}

void Transform::rotate(glm::vec3 axis, float theta)
{
    m_rotation = glm::rotate(m_rotation, theta, axis);
    updateModel();
}

void Transform::updateModel()
{
    m_model = glm::mat4(1.0f);
    m_model = glm::translate(m_model, m_position);
    m_model = m_model * m_rotation;
    m_model = glm::scale(m_model, m_scale);
}


Transform& Transform::operator=(const Transform& other)
{
    m_position = other.m_position;
    m_scale = other.m_scale;
    m_rotation = other.m_rotation;
    m_model = other.m_model;

    return *this;
}
