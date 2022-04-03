#pragma once

#include "glm/glm.hpp"
#include "lib/RenderObject.hpp"

class PointLight {
  friend class Renderer;

public:
  PointLight(std::string name = "pointLight");

  void move(glm::vec3 direction);
  void setPosition(glm::vec3 position);
  void setColor(glm::vec3 color);

  void addShaderProgram(std::shared_ptr<ShaderProgram> sp);

  inline std::shared_ptr<RenderObject> getRenderObject() {
    return m_renderObject;
  }

private:
  void updateTransform();

private:
  std::string m_name;
  glm::vec3 m_position;

  glm::vec3 m_ambient;
  glm::vec3 m_diffuse;
  glm::vec3 m_specular;
  std::shared_ptr<RenderObject> m_renderObject;

  // Shader programms that use this light
  std::vector<std::shared_ptr<ShaderProgram>> m_shaderPrograms;

  // ID used by the renderer to keep track of the light
  uint m_rendererID;
};
