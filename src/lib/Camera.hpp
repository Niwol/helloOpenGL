#pragma once

#include "ext/matrix_transform.hpp"
#include <cmath>
#include <glm.hpp>

enum CameraMovement
{
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  UP,
  DOWN
};

class Camera {
public:
  Camera(glm::vec3 position  = {0.0f, 0.0f,  0.0f},
         glm::vec3 wordlUp   = {0.0f, 1.0f,  0.0f},
         float yaw = M_PI / 2.0f,
         float pitch = 0.0f);

  inline glm::mat4 getViewMatrix() const {
    return glm::lookAt(m_position, m_position + m_direction, m_up);
  }

  inline glm::vec3 getPosition() const {
    return m_position;
  }

  void move(CameraMovement direction, float dt);

private:
  void updateCameraVectors();

private:
  glm::vec3 m_position = {0.0f, 0.0f, 0.0f};

  glm::vec3 m_direction = {0.0f, 0.0f, -1.0f};
  glm::vec3 m_up = {0.0f, 1.0f, 0.0f};
  glm::vec3 m_right = {1.0f, 0.0f, 0.0f};

  glm::vec3 m_worldUp = {0.0f, 1.0f, 0.0f};

  float m_yaw = M_PI / 2.0f;
  float m_pitch = 0.0f;

  float m_speed = 10.0f;
};
