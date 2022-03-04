#pragma once

#include "opengl_stuff.h"

#include <QOpenGLFunctions_4_1_Core>

#include <string>

class ShaderProgram : public QOpenGLFunctions_4_1_Core {
public:
  ShaderProgram(std::string vertexPath = "", std::string fragmentPath = "");
  ~ShaderProgram();

  bool createProgram(std::string vertexPath, std::string fragmentPath);

  void use();

  inline void setBool(const std::string &name, bool value) {
    glUniform1i(glGetUniformLocation(m_programId, name.c_str()), (int)value);
  }
  // ------------------------------------------------------------------------
  inline void setInt(const std::string &name, int value) {
    glUniform1i(glGetUniformLocation(m_programId, name.c_str()), value);
  }
  // ------------------------------------------------------------------------
  inline void setFloat(const std::string &name, float value) {
    glUniform1f(glGetUniformLocation(m_programId, name.c_str()), value);
  }
  // ------------------------------------------------------------------------
  inline void setVec2(const std::string &name, const glm::vec2 &value) {
    glUniform2fv(glGetUniformLocation(m_programId, name.c_str()), 1, &value[0]);
  }
  inline void setVec2(const std::string &name, float x, float y) {
    glUniform2f(glGetUniformLocation(m_programId, name.c_str()), x, y);
  }
  // ------------------------------------------------------------------------
  inline void setVec3(const std::string &name, const glm::vec3 &value) {
    glUniform3fv(glGetUniformLocation(m_programId, name.c_str()), 1, &value[0]);
  }
  inline void setVec3(const std::string &name, float x, float y, float z) {
    glUniform3f(glGetUniformLocation(m_programId, name.c_str()), x, y, z);
  }
  // ------------------------------------------------------------------------
  inline void setVec4(const std::string &name, const glm::vec4 &value) {
    glUniform4fv(glGetUniformLocation(m_programId, name.c_str()), 1, &value[0]);
  }
  inline void setVec4(const std::string &name, float x, float y, float z,
                      float w) {
    glUniform4f(glGetUniformLocation(m_programId, name.c_str()), x, y, z, w);
  }
  // ------------------------------------------------------------------------
  inline void setMat2(const std::string &name, const glm::mat2 &mat) {
    glUniformMatrix2fv(glGetUniformLocation(m_programId, name.c_str()), 1,
                       GL_FALSE, &mat[0][0]);
  }
  // ------------------------------------------------------------------------
  inline void setMat3(const std::string &name, const glm::mat3 &mat) {
    glUniformMatrix3fv(glGetUniformLocation(m_programId, name.c_str()), 1,
                       GL_FALSE, &mat[0][0]);
  }
  // ------------------------------------------------------------------------
  inline void setMat4(const std::string &name, const glm::mat4 &mat) {
    glUniformMatrix4fv(glGetUniformLocation(m_programId, name.c_str()), 1,
                       GL_FALSE, &mat[0][0]);
  }

private:
  bool checkCompileErrors(uint id, std::string type);

private:
  uint m_programId;
};