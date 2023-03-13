#include "lib/Light/DirectionalLight.hpp"
#include "lib/Light/Light.hpp"

void DirectionalLight::setShaderUniforms(ShaderProgram &shader)
{
    shader.setVec3("lightColor", m_color);
    shader.setVec3("directionalLight.direction", m_direction);
    shader.setInt("lightType", LightType::DIRECTION);
}
