#pragma once

#include <glm/glm.hpp>

#include "fwd.hpp"
#include "lib/Light/Light.hpp"

class PointLight : public Light
{
public:
    PointLight() = default;
    PointLight(const PointLight&) = delete;

    virtual ~PointLight() = default;

    virtual void setShaderUniforms(ShaderProgram &shader) override;

public:
    glm::vec3 m_color = {1.0f, 1.0f, 1.0f};
    glm::vec3 m_position = {0.0f, 0.0f, 0.0f};

    float m_constant = 1.0f;
    float m_linear = 0.7f;
    float m_quadratic = 0.5f;
};
