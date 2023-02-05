#include "PointLight.hpp"
#include "lib/Light/Light.hpp"

void PointLight::setShaderUniforms(ShaderProgram &shader)
{
    shader.setVec3("lightColor", m_color);
    shader.setVec3("pointLight.position", m_position);
    shader.setFloat("pointLight.constant", m_constant);
    shader.setFloat("pointLight.linear", m_linear);
    shader.setFloat("pointLight.quadratic", m_quadratic);
    shader.setInt("lightType", LightType::POINT);
}
