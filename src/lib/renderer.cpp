#include "renderer.hpp"

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::addMesh(Mesh *mesh) { m_meshes.push_back(mesh); }
void Renderer::setShaderProgram(ShaderProgram *sp) { m_shaderProgram = sp; }

void Renderer::draw() {
  if (m_shaderProgram) {
    m_shaderProgram->use();

    for (auto &m : m_meshes) {
      glBindVertexArray(m->m_VAO);
      glDrawElements(GL_TRIANGLES, m->m_nbIndices, GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
  } else {
    std::cout << "Renderer: WARNING: renderer has no shader program set"
              << std::endl;
  }
}