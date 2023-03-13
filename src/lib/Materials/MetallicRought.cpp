#include "lib/Materials/MetallicRought.hpp"
#include "lib/ShaderManager.hpp"
#include <string>

MetallicRoughtness::MetallicRoughtness()
{
    m_shaderID = DefaultShaders::Shader_MetallicRoughness;
}

bool MetallicRoughtness::usesVertexColor()
{
    return false;
}

void MetallicRoughtness::setShaderUniforms(ShaderProgram &shader)
{
    shader.setVec3("material.albedo", m_albedo);
    shader.setFloat("material.metallic", m_metallic);
    shader.setFloat("material.roughtness", m_roughtness);
}

void MetallicRoughtness::setShaderAmbient(ShaderProgram &shader)
{
    shader.setVec3("color", m_albedo);
    shader.setBool("hasTexture", false);
}

std::string MetallicRoughtness::getShaderName()
{
    return "MetallicRoughtness";
}

