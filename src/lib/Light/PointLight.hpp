#pragma once

#include "glm/glm.hpp"
#include "lib/RenderObject.hpp"
#include "lib/moveable.hpp"

class PointLight : public Moveable {
  friend class Renderer;

public:
  PointLight(std::string name = "pointLight");

  /**
   * Moves the point light in the direction given by `direction`
   */
  void move(glm::vec3 direction, uint index);

  /**
   * Sets the point lights position
   */
  void setPosition(glm::vec3 position);

  /**
   * Sets the point lights color
   */
  void setColor(glm::vec3 color);

  /**
   * Adds the shader program to the point light. The point light will be used in
   * this shader program
   */
  void addShaderProgram(std::shared_ptr<ShaderProgram> sp);

  /**
   * Gets the point light render object, a cube by default
   */
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
