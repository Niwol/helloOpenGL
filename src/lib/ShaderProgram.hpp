#pragma once

#include <string>

#include <src/opengl_stuff.h>

class ShaderProgram {
public:
  ShaderProgram(std::string vertexPath = "", 
                std::string geometryPath = "",
                std::string fragmentPath = "");
  ~ShaderProgram();

  /**
   * Creates a shader program with the 2 shaders given in the path
   *
   * @return true on success and false otherwise
   */
  bool createProgram(bool useGeometry = false);

  /**
   * Tels opengl to use this shader program
   */
  void use();

  /**
   * Sets a boolean value in the shader (the shader program needs to be used
   * first)
   */
  inline void setBool(const std::string &name, bool value) {
    glUniform1i(glGetUniformLocation(m_programId, name.c_str()), (int)value);
  }
  // ------------------------------------------------------------------------
  /**
   * Sets a integer value in the shader (the shader program needs to be used
   * first)
   */
  inline void setInt(const std::string &name, int value) {
    glUniform1i(glGetUniformLocation(m_programId, name.c_str()), value);
  }
  // ------------------------------------------------------------------------
  /**
   * Sets a float value in the shader (the shader program needs to be used
   * first)
   */
  inline void setFloat(const std::string &name, float value) {
    glUniform1f(glGetUniformLocation(m_programId, name.c_str()), value);
  }
  // ------------------------------------------------------------------------
  /**
   * Sets a glm::vec2 value in the shader (the shader program needs to be used
   * first)
   */
  inline void setVec2(const std::string &name, const glm::vec2 &value) {
    glUniform2fv(glGetUniformLocation(m_programId, name.c_str()), 1, &value[0]);
  }
  /**
   * Sets a glm::vec2 value in the shader (the shader program needs to be used
   * first)
   */
  inline void setVec2(const std::string &name, float x, float y) {
    glUniform2f(glGetUniformLocation(m_programId, name.c_str()), x, y);
  }
  // ------------------------------------------------------------------------
  /**
   * Sets a glm::vec3 value in the shader (the shader program needs to be used
   * first)
   */
  inline void setVec3(const std::string &name, const glm::vec3 &value) {
    glUniform3fv(glGetUniformLocation(m_programId, name.c_str()), 1, &value[0]);
  }
  /**
   * Sets a glm::vec3 value in the shader (the shader program needs to be used
   * first)
   */
  inline void setVec3(const std::string &name, float x, float y, float z) {
    glUniform3f(glGetUniformLocation(m_programId, name.c_str()), x, y, z);
  }
  // ------------------------------------------------------------------------
  /**
   * Sets a glm::vec4 value in the shader (the shader program needs to be used
   * first)
   */
  inline void setVec4(const std::string &name, const glm::vec4 &value) {
    glUniform4fv(glGetUniformLocation(m_programId, name.c_str()), 1, &value[0]);
  }
  /**
   * Sets a glm::vec4 value in the shader (the shader program needs to be used
   * first)
   */
  inline void setVec4(const std::string &name, float x, float y, float z,
                      float w) {
    glUniform4f(glGetUniformLocation(m_programId, name.c_str()), x, y, z, w);
  }
  // ------------------------------------------------------------------------
  /**
   * Sets a glm::mat2 value in the shader (the shader program needs to be used
   * first)
   */
  inline void setMat2(const std::string &name, const glm::mat2 &mat) {
    glUniformMatrix2fv(glGetUniformLocation(m_programId, name.c_str()), 1,
                       GL_FALSE, &mat[0][0]);
  }
  // ------------------------------------------------------------------------
  /**
   * Sets a glm::mat3 value in the shader (the shader program needs to be used
   * first)
   */
  inline void setMat3(const std::string &name, const glm::mat3 &mat) {
    glUniformMatrix3fv(glGetUniformLocation(m_programId, name.c_str()), 1,
                       GL_FALSE, &mat[0][0]);
  }
  // ------------------------------------------------------------------------
  /**
   * Sets a glm::mat4 value in the shader (the shader program needs to be used
   * first)
   */
  inline void setMat4(const std::string &name, const glm::mat4 &mat) {
    glUniformMatrix4fv(glGetUniformLocation(m_programId, name.c_str()), 1,
                       GL_FALSE, &mat[0][0]);
  }

  inline void setVertexShader(const std::string path)
  {
      m_vertexPath = path;
  }

  inline void setGeometryShader(const std::string path)
  {
      m_geometryPath = path;
  }
  
  inline void setFragmentShader(const std::string path)
  {
      m_fragmentPath = path;
  }

private:
  /**
   * Checks for compile errors that might have occured
   * @return true if no errors have occured and false otherwise
   */
  bool checkCompileErrors(uint id, std::string type);

  bool parseSourceCode(std::string path, std::string& code);

private:
  uint m_programId = 0;

  std::string m_vertexPath = "";
  std::string m_geometryPath = "";
  std::string m_fragmentPath = "";
};
