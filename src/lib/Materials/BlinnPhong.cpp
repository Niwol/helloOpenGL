#include "lib/Materials/BlinnPhong.hpp"
#include "lib/ShaderManager.hpp"
#include <GL/gl.h>
#include <cstdint>
#include <string>

BlinnPhong::BlinnPhong()
{
    m_shaderID = DefaultShaders::Shader_BlinnPhong;
}

bool BlinnPhong::usesVertexColor()
{
    return false;
}

void BlinnPhong::setShaderUniforms(ShaderProgram &shader)
{
    if(m_hasTexture)
    {
        m_diffuseTexture.bind(GL_TEXTURE0);
        shader.setInt("material.diffuseTexture", 0);

        m_specularTexture.bind(GL_TEXTURE1);
        shader.setInt("material.specularTexture", 1);
    }
    else
    {
        shader.setVec3("material.diffuse", m_diffuse);
        shader.setVec3("material.specular", m_specular);
    }

    shader.setBool("material.hasTexture", m_hasTexture);
    shader.setInt("material.shininess", m_shininess);
}

void BlinnPhong::setShaderAmbient(ShaderProgram &shader)
{
    if(m_hasTexture)
    {
        m_diffuseTexture.bind(GL_TEXTURE0);
        shader.setInt("tex", 0);
    }
    else
    {
        shader.setVec3("color", m_diffuse);
    }

    shader.setBool("hasTexture", m_hasTexture);
}

std::string BlinnPhong::getShaderName()
{
    return "BlinnPhong";
}

