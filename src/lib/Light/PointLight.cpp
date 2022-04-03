#include "PointLight.hpp"

PointLight::PointLight(std::string name) : m_name{name} {
  // Position
  m_position = glm::vec3(0.0f);

  // Light color
  m_ambient = glm::vec3(0.1f);
  m_diffuse = glm::vec3(0.5f);
  m_specular = glm::vec3(1.0f);

  // Render object
  m_renderObject = std::make_shared<RenderObject>();
  m_renderObject->getMesh()->to_cube();
  auto t = glm::mat4(1.0f);
  t = glm::scale(t, glm::vec3(0.1f, 0.1f, 0.1f));
  m_renderObject->transform(t);

  auto m = m_renderObject->getMaterial();
  m->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
}

void PointLight::move(glm::vec3 direction) {
  m_position += direction;
  updateTransform();
}

void PointLight::setPosition(glm::vec3 position) {
  m_position = position;
  updateTransform();
}

void PointLight::setColor(glm::vec3 color) {
  m_ambient = color * glm::vec3(0.1f);
  m_diffuse = color * glm::vec3(0.5f);
  m_specular = color * glm::vec3(1.0f);

  auto m = m_renderObject->getMaterial();
  m->diffuse = color;
}

void PointLight::addShaderProgram(std::shared_ptr<ShaderProgram> sp) {
  m_shaderPrograms.push_back(sp);
}

void PointLight::updateTransform() {
  auto t = glm::mat4(1.0f);
  t = glm::translate(t, m_position);
  t = glm::scale(t, glm::vec3(0.1f, 0.1f, 0.1f));

  m_renderObject->resetModel();
  m_renderObject->transform(t);
}
