#include "lib/Materials/Material.hpp"
#include <ostream>
#include <string>

void Material::selectShader(const ShaderManager &shaderManager)
{
    std::string shaderName = getShaderName();

    auto shaderID = shaderManager.getShaderID(shaderName);

    if(shaderID.has_value())
    {
        m_shaderID = shaderID.value();
    }
    else
    {
        std::cout << "[Material] Warning: " << shaderName 
                  << "is not registered in the shader manager, "
                  << "using simple shader"
                  << std::endl;

        shaderID = shaderManager.getShaderID("SimpleShader");
        if(shaderID.has_value())
        {
            m_shaderID = shaderID.value();
        }
        else
        {
            std::cout << "[Material] Error: SimpleShader is not registered, "
                      << "the shader manager seems not to be correctly "
                      << "initialized"
                      << std::endl;
        }
    }
}

