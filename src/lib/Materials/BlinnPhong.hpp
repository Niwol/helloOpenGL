#pragma once

#include <glm/glm.hpp>

#include "lib/Materials/Material.hpp"

class BlinnPhong : public Material
{
public:
    BlinnPhong() = default;
    BlinnPhong(const BlinnPhong&) = delete;

    virtual ~BlinnPhong() = default;

    virtual void setShaderUinforms(std::shared_ptr<ShaderProgram> shader) override;

private:

};

