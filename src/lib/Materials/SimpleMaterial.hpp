#pragma once

#include "lib/Materials/Material.hpp"

class SimpleMaterial : public Material
{
public:
    SimpleMaterial();
    SimpleMaterial(const SimpleMaterial&) = delete;

    virtual ~SimpleMaterial() = default;

    virtual bool usesVertexColor() override;    

    virtual void setShaderUniforms(ShaderProgram &shader) override;
    virtual void setShaderAmbient(ShaderProgram &shader) override;

    virtual std::string getShaderName() override;

public:
    glm::vec3 m_color = {0.3f, 0.3f, 0.3f};
};
