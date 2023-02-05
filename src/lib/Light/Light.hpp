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
};
