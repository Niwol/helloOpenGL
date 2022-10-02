#include "Camera.hpp"
#include <cstdio>

Camera::Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch)
  : m_position(position), m_worldUp(worldUp), m_yaw(yaw), m_pitch(pitch)
{
  updateCameraVectors();
}

void Camera::move(CameraMovement direction, float dt)
{
  float vel = m_speed * dt;
  switch (direction) 
  {
    case FORWARD:
      m_position +=
        glm::normalize(glm::vec3(m_direction.x, 0.0f, m_direction.z)) * vel;
      break;

    case BACKWARD:
      m_position -=
        glm::normalize(glm::vec3(m_direction.x, 0.0f, m_direction.z)) * vel;
      break;

    case LEFT:
      m_position -= m_right * vel;
      break;

    case RIGHT:
      m_position += m_right * vel;
      break;

    case UP:
      m_position += m_worldUp * vel;
      break;

    case DOWN:
      m_position -= m_worldUp * vel;
      break;
    }
}

void Camera::updateCameraVectors() 
{
  glm::vec3 front;
  front.x = std::cos(m_yaw) * std::cos(m_pitch);
  front.y = std::sin(m_pitch);
  front.z = std::sin(m_yaw) * std::cos(m_pitch);
  m_direction = glm::normalize(front);

  m_right = glm::normalize(glm::cross(m_direction, m_worldUp));
  m_up    = glm::normalize(glm::cross(m_right, m_direction));
}

