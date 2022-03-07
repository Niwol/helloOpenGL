#include "renderer.hpp"

Renderer::Renderer(int width, int height) : m_width{width}, m_height{height} {}

Renderer::~Renderer() {}

void Renderer::addRenderObject(RenderObject *ro) {
  m_renderObjects.push_back(ro);
}
void Renderer::setShaderProgram(ShaderProgram *sp) { m_shaderProgram = sp; }
void Renderer::setCamera(std::shared_ptr<Camera> cam) { m_camera = cam; }
void Renderer::setDimension(int width, int height) {
  m_width = width;
  m_height = height;
}

void Renderer::draw() {
  if (m_shaderProgram) {

    m_shaderProgram->setMat4("view", m_camera->viewmatrix());
    m_shaderProgram->setMat4("projection",
                             glm::perspective(glm::radians(45.0f),
                                              float(m_width) / float(m_height),
                                              0.1f, 100.0f));

    m_shaderProgram->use();

    for (auto &ro : m_renderObjects) {
      Mesh *mesh = ro->getMesh();

      m_shaderProgram->setMat4("model", ro->getModelMatrix());
      glBindVertexArray(mesh->m_VAO);
      glDrawElements(mesh->m_mode, mesh->m_nbIndices, GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
  } else {
    std::cout << "Renderer: WARNING: renderer has no shader program set"
              << std::endl;
  }
}