#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <optional>

#include "lib/ShaderProgram.hpp"

class ShaderManager
{
public:
    ShaderManager();
    ShaderManager(const ShaderManager&) = delete;

    ~ShaderManager();

    uint32_t addShaderProgram(std::shared_ptr<ShaderProgram> shaderProgram,
                              std::string name);
    void removeShaderProgram(uint32_t shaderID);

    std::optional<std::shared_ptr<ShaderProgram>> getShader(uint32_t shaderID);
    std::optional<uint32_t> getShaderID(std::string name);

private:
    bool loadDefaultShaders();

private:
    std::map<uint32_t, std::shared_ptr<ShaderProgram>> m_shaderPrograms;
    std::map<std::string, uint32_t> m_shaderNameToID;
    std::map<uint32_t, std::string> m_shaderIDToName;

    uint32_t m_nextShaderID = 0;
};


/**
 *  Renderer -> Render()
 *  {
 *      loop lights
 *      {
 *          renderObject(object, light)
 *      }
 *  }
 *
 *  renderObject(object, light)
 *  {
 *      matID = object->getMat()
 *      mat = matManager->getMat(matID)
 *
 *      shderID = mat->getShader()
 *      shader = ShaderManager->getShader(shaderID)
 *
 *      setUniforms(shader, mat, light)
 *      glDraw()
 *  }
 *
 */
