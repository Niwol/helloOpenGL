#pragma once

#include <glm/glm.hpp>

#include "fwd.hpp"
#include "lib/Materials/Material.hpp"
#include "lib/ShaderProgram.hpp"
#include "lib/texture.hpp"

class BlinnPhong : public Material
{
public:
    BlinnPhong();
    BlinnPhong(const BlinnPhong&) = delete;

    virtual ~BlinnPhong() = default;

    virtual bool usesVertexColor() override;

    virtual void setShaderUniforms(ShaderProgram& shader) override;

    virtual std::string getShaderName() override;

public:
    glm::vec3 m_diffuse = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_specular = {0.0f, 0.0f, 0.0f};

    int m_shininess = 32;

    bool m_hasTexture = false;
    Texture m_diffuseTexture;
    Texture m_specularTexture;
};

