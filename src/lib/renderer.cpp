#include "renderer.hpp"

Renderer::Renderer(int width, int height)
    : m_width{width}, m_height{height}, m_nextID{1} {}

Renderer::~Renderer() {}

void Renderer::addRenderObject(std::shared_ptr<RenderObject> ro,
                               bool printWarning) {
  try {
    m_renderObjects.at(ro->m_rendererID);

    if (printWarning)
      std::cout << "WARNING: Renderer::addRenderObject: renderer already "
                   "contains the render object"
                << std::endl;
  } catch (std::out_of_range &e) {
    ro->m_rendererID = m_nextID;
    m_renderObjects.insert(
        std::pair<uint, std::shared_ptr<RenderObject>>(m_nextID, ro));
    m_nextID++;
  }
}

void Renderer::removeRenderObject(std::shared_ptr<RenderObject> ro,
                                  bool printWarning) {
  if (m_renderObjects.erase(ro->m_rendererID) == 0 && printWarning)
    std::cout << "WARNING: Renderer::removeRenderObject: renderer didn't "
                 "contain the render object"
              << std::endl;
}

void Renderer::setShaderProgram(std::shared_ptr<ShaderProgram> sp) {
  m_shaderProgram = sp;
}
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

    // std::cout << "Renderer::draw: drawing " << m_renderObjects.size()
    //           << " objects" << std::endl;
    for (auto &pair : m_renderObjects) {
      auto ro = pair.second;
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