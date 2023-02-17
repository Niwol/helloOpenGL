#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "lib/ShaderProgram.hpp"

ShaderProgram::ShaderProgram(std::string vertexPath,
                             std::string geometryPath,
                             std::string fragmentPath)
    : m_vertexPath{vertexPath},
      m_geometryPath{geometryPath},
      m_fragmentPath{fragmentPath}
{
    m_programId = glCreateProgram();

    if (m_vertexPath != "" && m_fragmentPath != "") 
    {
        createProgram();
    } 
    else if (vertexPath != "" && fragmentPath == "") 
    {
        std::cout << "WARNING: didn't create shader program, missing fragement "
            "shader path"
            << std::endl;
    }
}

ShaderProgram::~ShaderProgram() 
{
    if(m_programId)
        glDeleteProgram(m_programId); 
}

bool ShaderProgram::createProgram(bool useGeometry)
{
    std::string vertexCode;
    std::string geometryCode;
    std::string fragmentCode;

    // Read shaders
    std::ifstream vertexFile;
    std::ifstream geometryFile;
    std::ifstream fragmentFile;

    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    geometryFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try 
    {
        vertexFile.open(m_vertexPath);
        fragmentFile.open(m_fragmentPath);
        if(useGeometry)
            geometryFile.open(m_geometryPath);

        std::stringstream vStream, gStream, fStream;

        vStream << vertexFile.rdbuf();
        fStream << fragmentFile.rdbuf();
        if(useGeometry)
            gStream << geometryFile.rdbuf();

        vertexCode = vStream.str();
        fragmentCode = fStream.str();
        if(useGeometry)
            geometryCode = gStream.str();

        vertexFile.close();
        fragmentFile.close();
        if(useGeometry)
            geometryFile.close();
    } 
    catch (std::ifstream::failure &e) 
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what()
            << std::endl;
        return false;
    }

    const char *vertexSource = vertexCode.c_str();
    const char *fragmentSource = fragmentCode.c_str();
    const char *geometrySource = geometryCode.c_str();

    // Compiling shaders
    uint vertexShader, geometryShader, fragmentShader;

    // Vertex
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    if (!checkCompileErrors(vertexShader, "VERTEX"))
        return false;

    if(useGeometry)
    {
        // Geometry
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, &geometrySource, NULL);
        glCompileShader(geometryShader);
        if (!checkCompileErrors(geometryShader, "GEOMETRY"))
            return false;
    }

    // Fragment
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    if (!checkCompileErrors(fragmentShader, "FRAGMENT"))
        return false;

    // Shader program and linking
    glAttachShader(m_programId, vertexShader);
    if(useGeometry)
        glAttachShader(m_programId, geometryShader);
    glAttachShader(m_programId, fragmentShader);
    glLinkProgram(m_programId);
    if (!checkCompileErrors(m_programId, "PROGRAM"))
        return false;

    // Delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if(useGeometry)
        glDeleteShader(geometryShader);

    return true;
}

void ShaderProgram::use() { glUseProgram(m_programId); }

bool ShaderProgram::checkCompileErrors(uint id, std::string type) {
  int success;
  char infoLog[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(id, 1024, NULL, infoLog);
      std::cout
          << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
          << infoLog
          << "\n -- --------------------------------------------------- -- "
          << std::endl;
      return false;
    }
  } else {
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(id, 1024, NULL, infoLog);
      std::cout
          << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
          << infoLog
          << "\n -- --------------------------------------------------- -- "
          << std::endl;
      return false;
    }
  }

  return true;
}
