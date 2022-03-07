#include <fstream>
#include <sstream>

#include "lib/ShaderProgram.hpp"

ShaderProgram::ShaderProgram(std::string vertexPath, std::string fragmentPath) {

  initializeOpenGLFunctions();

  if (vertexPath != "" && fragmentPath != "") {
    createProgram(vertexPath, fragmentPath);
  } else if (vertexPath != "" && fragmentPath == "") {
    std::cout << "WARNING: didn't create shader program, missing fragement "
                 "shader path"
              << std::endl;
  }
}

ShaderProgram::~ShaderProgram() { glDeleteProgram(m_programId); }

bool ShaderProgram::createProgram(std::string vertexPath,
                                  std::string fragmentPath) {
  std::string vertexCode;
  std::string fragmentCode;

  // Read shaders
  std::ifstream vertexFile;
  std::ifstream fragmentFile;

  vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    vertexFile.open(vertexPath);
    fragmentFile.open(fragmentPath);

    std::stringstream vStream, fStream;

    vStream << vertexFile.rdbuf();
    fStream << fragmentFile.rdbuf();

    vertexCode = vStream.str();
    fragmentCode = fStream.str();

    vertexFile.close();
    fragmentFile.close();
  } catch (std::ifstream::failure &e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what()
              << std::endl;
    return false;
  }

  const char *vertexSource = vertexCode.c_str();
  const char *fragmentSource = fragmentCode.c_str();

  // Compiling shaders
  uint vertexShader, fragmentShader;

  // Vertex
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);
  if (!checkCompileErrors(vertexShader, "VERTEX"))
    return false;

  // Fragment
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);
  if (!checkCompileErrors(fragmentShader, "FRAGMENT"))
    return false;

  // Shader program and linking
  m_programId = glCreateProgram();
  glAttachShader(m_programId, vertexShader);
  glAttachShader(m_programId, fragmentShader);
  glLinkProgram(m_programId);
  if (!checkCompileErrors(m_programId, "PROGRAM"))
    return false;

  // Delete shaders
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

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
