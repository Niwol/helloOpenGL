#pragma once

#include "texture.hpp"
#include <memory>

#include "opengl_stuff.h"

struct Material {
  glm::vec3 diffuse = glm::vec3(0.0f);
  glm::vec3 specular = glm::vec3(0.0f);

  bool isTextureMaterial = false;
  std::shared_ptr<Texture> diffuseTexture;
  std::shared_ptr<Texture> specularTexture;

  int shininess = 32;
};
