#pragma once

#include <glm.hpp>

class Moveable {
public:
  virtual void move(glm::vec3 direction, uint index) = 0;
};