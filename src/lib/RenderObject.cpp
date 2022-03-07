#include "RenderObject.hpp"

RenderObject::RenderObject() { m_modelMatix = glm::mat4(1.0f); }
RenderObject::~RenderObject() {
  if (m_mesh)
    delete m_mesh;
}

void RenderObject::setMesh(Mesh *mesh) { m_mesh = mesh; }
Mesh *RenderObject::getMesh() { return m_mesh; }
glm::mat4 RenderObject::getModelMatrix() { return m_modelMatix; }
