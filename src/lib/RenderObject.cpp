#include "RenderObject.hpp"

RenderObject::RenderObject() : m_mesh{nullptr} {
  m_modelMatix = glm::mat4(1.0f);
}
RenderObject::~RenderObject() {}

void RenderObject::transform(glm::mat4 transform) { m_modelMatix *= transform; }
void RenderObject::resetModel() { m_modelMatix = glm::mat4(1.0f); }

void RenderObject::setMesh(std::unique_ptr<Mesh> mesh) {
  m_mesh = std::move(mesh);
}

Mesh *RenderObject::getMesh() { return m_mesh.get(); }
glm::mat4 RenderObject::getModelMatrix() { return m_modelMatix; }
