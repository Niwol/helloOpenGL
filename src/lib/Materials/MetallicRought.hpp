#pragma once

#include "lib/Materials/Material.hpp"

class MetallicRoughtness : public Material
{
public:
    MetallicRoughtness();
    MetallicRoughtness(const MetallicRoughtness&) = delete;

    virtual ~MetallicRoughtness() = default;

    virtual bool usesVertexColor() override;

    virtual void setShaderUniforms(ShaderProgram& shader) override;
    virtual void setShaderAmbient(ShaderProgram &shader) override;

    virtual std::string getShaderName() override;

public:
    glm::vec3 m_albedo = {0.0f, 0.0f, 0.0f};

    float m_metallic = 0.0f;
    float m_roughtness = 0.0f;
};

