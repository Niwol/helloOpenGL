#include "lib/Materials/SimpleMaterial.hpp"
#include "lib/ShaderManager.hpp"

SimpleMaterial::SimpleMaterial()
{
    m_shaderID = DefaultShaders::Shader_Simple;
}

bool SimpleMaterial::usesVertexColor() 
{
    return false;
}

void SimpleMaterial::setShaderUniforms(ShaderProgram &shader) 
{
    shader.setVec3("color", m_color);
}

std::string SimpleMaterial::getShaderName()
{
    return "Simple";
}

