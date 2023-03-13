#pragma once

#include <glm/glm.hpp>

#include "fwd.hpp"
#include "lib/Light/Light.hpp"

class SpotLight : public Light
{
public:
    SpotLight() = default;
    SpotLight(const SpotLight&) = delete;

    virtual ~SpotLight() = default;

    virtual void setShaderUniforms(ShaderProgram &shader) override;

public:
    glm::vec3 m_position = {0.0f, 0.0f, 0.0f};

    glm::vec3 m_direction = {0.0f, -1.0f, 0.0f};
    float m_cutOffAngle = M_PI / 5.0f;
    float m_outerCutOffAngle = M_PI / 4.0f;

    float m_constant = 1.0f;
    float m_linear = 0.09f;
    float m_quadratic = 0.032f;
};
