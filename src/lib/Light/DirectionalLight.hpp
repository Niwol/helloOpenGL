#pragma once

#include <glm/glm.hpp>

#include "fwd.hpp"
#include "lib/Light/Light.hpp"

class DirectionalLight : public Light
{
public:
    DirectionalLight() = default;
    DirectionalLight(const DirectionalLight&) = delete;

    virtual ~DirectionalLight() = default;

    virtual void setShaderUniforms(ShaderProgram &shader) override;

public:
    glm::vec3 m_direction = {0.0f, -1.0f, 0.0f};
};

