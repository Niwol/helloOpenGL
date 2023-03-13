#include "lib/Materials/SimpleMaterial.hpp"
#include "lib/ShaderManager.hpp"
#include "lib/ShaderProgram.hpp"

SimpleMaterial::SimpleMaterial()
{
    m_shaderID = DefaultShaders::Shader_Simple;
}

bool SimpleMaterial::usesVertexColor() 
{
    return false;
}

void SimpleMaterial::setShaderUniforms(ShaderProgram& shader) 
{
    shader.setVec3("color", m_color);
}

void SimpleMaterial::setShaderAmbient(ShaderProgram& shader)
{
    shader.setVec3("color", m_color);
    shader.setBool("hasTexture", false);
}

std::string SimpleMaterial::getShaderName()
{
    return "Simple";
}

