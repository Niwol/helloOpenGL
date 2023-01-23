#include "lib/ShaderManager.hpp"
#include "lib/ShaderProgram.hpp"
#include <cstdint>
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

std::optional<uint32_t> ShaderManager::getShaderID(std::string name)
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
                         ("normalShader", "Normal"));

    for(auto& pair : shaderNames)
    {
        std::string pathName = pair.first;
        std::string name = pair.second;

        auto shader = std::make_shared<ShaderProgram>();
        shader->createProgram("../src/shaders/" + pathName + ".vs",
                              "../src/shaders/" + pathName + ".fs");

        addShaderProgram(shader, name);
    }

    return true;
}


