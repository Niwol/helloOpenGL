#include "RenderObject.hpp"
#include "lib/Materials/BlinnPhong.hpp"
#include "lib/Materials/SimpleMaterial.hpp"

RenderObject::RenderObject() : m_rendererID{0} {
    m_mesh = std::make_unique<Mesh>();

    m_material = std::make_shared<SimpleMaterial>();
}

RenderObject::~RenderObject() {}

void RenderObject::transform(glm::mat4 transform) { m_modelMatix *= transform; }
void RenderObject::resetModel() { m_modelMatix = glm::mat4(1.0f); }

void RenderObject::setMesh(std::unique_ptr<Mesh> mesh) {
    m_mesh = std::move(mesh);
}

Mesh *RenderObject::getMesh() { return m_mesh.get(); }
glm::mat4 RenderObject::getModelMatrix() { return m_modelMatix; }
