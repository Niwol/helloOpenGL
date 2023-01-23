#pragma once

#include <memory>

#include "lib/ShaderProgram.hpp"
#include "opengl_stuff.h"

class Material
{
public:
    Material() = default;
    Material(const Material&) = delete;

    virtual ~Material() = default;

    virtual void setShaderUinforms(ShaderProgram& shader) = 0;
};

//struct Material {
//  // Basic lightning
//  glm::vec3 diffuse = glm::vec3(0.0f);
//  glm::vec3 specular = glm::vec3(0.0f);
//
//  // BRDF lightning
//  float roughness = 0.0f;
//  float metallic = 0.0f;
//
//  // Texture lightning
//  bool isTextureMaterial = false;
//  std::shared_ptr<Texture> diffuseTexture;
//  std::shared_ptr<Texture> specularTexture;
//
//  int shininess = 32;
//  bool useVertexColor = false;
//};
