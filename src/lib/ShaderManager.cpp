#include "lib/ShaderManager.hpp"
#include "lib/ShaderProgram.hpp"
#include <cstdint>
#include <cstdio>
#include <memory>
#include <optional>
#include <string>
#include <vector>

ShaderManager::ShaderManager()
{
    loadDefaultShaders();
}

ShaderManager::~ShaderManager() {}


uint32_t ShaderManager::addShaderProgram(std::shared_ptr<ShaderProgram> shaderProgram, std::string name)
{
    auto iterator = m_shaderNameToID.find(name);
    if(iterator != m_shaderNameToID.end())
    {
        std::cout << "[ShaderManager] Warning: the shader program " << name 
                  << " is already loaded" << std::endl;

        return iterator->second;
    }

    m_shaderPrograms[m_nextShaderID] = shaderProgram;
    m_shaderNameToID[name] = m_nextShaderID;
    m_shaderIDToName[m_nextShaderID] = name;

    return m_nextShaderID++;
}

void ShaderManager::removeShaderProgram(uint32_t shaderID)
{
    auto iterator = m_shaderIDToName.find(shaderID);
    if(iterator == m_shaderIDToName.end())
    {
        std::cout << "[ShaderProgram] Warning: The shader program with ID "
                  << shaderID << " didn't exist"
                  << std::endl;

        return;
    }

    m_shaderPrograms.erase(iterator->first);
    m_shaderNameToID.erase(iterator->second);
    m_shaderIDToName.erase(iterator);
}

std::optional<std::shared_ptr<ShaderProgram>> 
ShaderManager::getShader(uint32_t shaderID)
{
    auto iterator = m_shaderPrograms.find(shaderID);
    if(iterator == m_shaderPrograms.end())
    {
        return {};
    }

    return iterator->second;
}

std::optional<uint32_t> ShaderManager::getShaderID(std::string name) const
{
    auto iterator = m_shaderNameToID.find(name);
    if(iterator == m_shaderNameToID.end())
    {
        return {};
    }

    return iterator->second;
}


bool ShaderManager::loadDefaultShaders()
{
    std::vector<std::pair<std::string, std::string>> shaderNames;
    
    shaderNames.push_back(std::pair<std::string, std::string>
                         ("Utils/black", "Black"));
    shaderNames.push_back(std::pair<std::string, std::string>
                         ("Utils/normalShader", "Normal"));
    shaderNames.push_back(std::pair<std::string, std::string>
                         ("Utils/depthShader", "Depth"));
    shaderNames.push_back(std::pair<std::string, std::string>
                         ("Materials/simpleShader", "Simple"));
    shaderNames.push_back(std::pair<std::string, std::string>
                         ("Materials/blinnPhong", "BlinnPhong"));
    shaderNames.push_back(std::pair<std::string, std::string>
                         ("Materials/brdfShader", "MetallicRoughness"));
    shaderNames.push_back(std::pair<std::string, std::string>
                         ("Utils/ambient", "Ambient"));

    uint32_t currentShaderID = m_nextShaderID;
    m_nextShaderID = 1;

    for(auto& pair : shaderNames)
    {
        std::string pathName = "../src/shaders/" + pair.first;
        std::string name = pair.second;

        auto shader = std::make_shared<ShaderProgram>();
        shader->setVertexShader(pathName + ".vs");
        shader->setFragmentShader(pathName + ".fs");
        bool res = shader->createProgram();

    
        if(!res)
        {
            std::cout << "[ShaderManager] Error: Failed to load default shader:"
                      << " name = " << name
                      << std::endl;
        }

        addShaderProgram(shader, name);
    }

    m_nextShaderID = currentShaderID;

    return true;
}


