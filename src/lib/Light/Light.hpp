#pragma once

#include "lib/ShaderProgram.hpp"

enum LightType 
{
    DIRECTION   = 0,
    POINT       = 1,
    SPOT        = 2,
};

class Light
{
public:
    Light() = default;
    Light(const Light&) = delete;

    virtual ~Light() = default;

    virtual void setShaderUniforms(ShaderProgram& shader) = 0;

public:
    glm::vec3 m_color = {1.0f, 1.0f, 1.0f};
};
