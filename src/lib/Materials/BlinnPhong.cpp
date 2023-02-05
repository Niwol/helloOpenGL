#include "lib/Materials/BlinnPhong.hpp"
#include "lib/ShaderManager.hpp"
#include <cstdint>
#include <string>

BlinnPhong::BlinnPhong()
{
    m_shaderID = DefaultShaders::BlinnPhong;
}

bool BlinnPhong::usesVertexColor()
{
    return false;
}

void BlinnPhong::setShaderUniforms(ShaderProgram &shader)
{

}

std::string BlinnPhong::getShaderName()
{
    return "BlinnPhong";
}

