#pragma once

#include <cstdint>
#include <memory>

#include <glm/glm.hpp>

#include "lib/ShaderManager.hpp"
#include "lib/ShaderProgram.hpp"
#include "opengl_stuff.h"

class Material
{
public:
    Material() = default;
    Material(const Material&) = delete;

    virtual ~Material() = default;

    void selectShader(const ShaderManager& shaderManager);

    virtual bool usesVertexColor() = 0;

    virtual void setShaderUniforms(ShaderProgram& shader) = 0;
    virtual void setShaderAmbient(ShaderProgram& shader) = 0;

    virtual std::string getShaderName() = 0;

    inline uint32_t getShaderID()
    {
        return m_shaderID;
    }

protected:
    uint32_t m_shaderID = -1;
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
