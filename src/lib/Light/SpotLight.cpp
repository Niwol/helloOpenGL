#include "lib/Light/SpotLight.hpp"
#include "lib/Light/Light.hpp"

void SpotLight::setShaderUniforms(ShaderProgram &shader)
{
    shader.setVec3("lightColor", m_color);
    shader.setVec3("spotLight.position", m_position);
    shader.setVec3("spotLight.direction", m_direction);
    shader.setFloat("spotLight.cutOffAngle", std::cos(m_cutOffAngle));
    shader.setFloat("spotLight.outerCutOffAngle", std::cos(m_outerCutOffAngle));
    shader.setFloat("spotLight.constant", m_constant);
    shader.setFloat("spotLight.linear", m_linear);
    shader.setFloat("spotLight.quadratic", m_quadratic);
    shader.setInt("lightType", LightType::SPOT);
}
