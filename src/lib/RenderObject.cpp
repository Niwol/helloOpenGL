#include "RenderObject.hpp"

RenderObject::RenderObject() : m_rendererID{0} {
  m_mesh = std::make_unique<Mesh>();
  m_modelMatix = glm::mat4(1.0f);

  m_material = std::make_shared<Material>();
  m_material->diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
  m_material->specular = glm::vec3(0.0f);
  m_material->shininess = 0;
  m_material->isTextureMaterial = false;
}
RenderObject::~RenderObject() {}

void RenderObject::transform(glm::mat4 transform) { m_modelMatix *= transform; }
void RenderObject::resetModel() { m_modelMatix = glm::mat4(1.0f); }

void RenderObject::setMesh(std::unique_ptr<Mesh> mesh) {
  m_mesh = std::move(mesh);
}

Mesh *RenderObject::getMesh() { return m_mesh.get(); }
glm::mat4 RenderObject::getModelMatrix() { return m_modelMatix; }
