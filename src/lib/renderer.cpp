#include "renderer.hpp"

Renderer::Renderer(int width, int height)
    : m_width{width}, m_height{height}, m_nextID{1} {}

Renderer::~Renderer() {}

void Renderer::addRenderObject(std::shared_ptr<RenderObject> ro,
                               bool printWarning) {

  static uint nextID = 1;
  try {
    m_renderObjects.at(ro->m_rendererID);

    if (printWarning)
      std::cout << "WARNING: Renderer::addRenderObject: renderer already "
                   "contains the render object"
                << std::endl;
  } catch (std::out_of_range &e) {
    ro->m_rendererID = nextID;
    m_renderObjects.insert(
        std::pair<uint, std::shared_ptr<RenderObject>>(nextID, ro));
    nextID++;
  }
}

void Renderer::removeRenderObject(std::shared_ptr<RenderObject> ro,
                                  bool printWarning) {
  if (m_renderObjects.erase(ro->m_rendererID) == 0 && printWarning)
    std::cout << "WARNING: Renderer::removeRenderObject: renderer didn't "
                 "contain the render object"
              << std::endl;
}

void Renderer::addPointLight(std::shared_ptr<PointLight> pointLight,
                             bool printWarning) {

  static uint nextID = 1;
  try {
    m_pointLights.at(pointLight->m_rendererID);

    if (printWarning)
      std::cout << "WARNING: Renderer::addRenderObject: renderer already "
                   "contains the point light"
                << std::endl;
  } catch (std::out_of_range &e) {
    pointLight->m_rendererID = nextID;
    m_pointLights.insert(
        std::pair<uint, std::shared_ptr<PointLight>>(nextID, pointLight));
    nextID++;
  }
}

void Renderer::removePointLight(std::shared_ptr<PointLight> pointLight,
                                bool printWarning) {
  if (m_pointLights.erase(pointLight->m_rendererID) == 0 && printWarning)
    std::cout << "WARNING: Renderer::removeRenderObject: renderer didn't "
                 "contain the render object"
              << std::endl;
}

void Renderer::setCamera(std::shared_ptr<Camera> cam) { m_camera = cam; }
void Renderer::setDimension(int width, int height) {
  m_width = width;
  m_height = height;
}

void Renderer::draw() {

  glm::mat4 perspective = glm::perspective(
      glm::radians(45.0f), float(m_width) / float(m_height), 0.1f, 100.0f);

  // Setting lights in their shader programs
  int i = 0;
  for (auto &pair : m_pointLights) {
    auto pl = pair.second;
    for (auto &sp : pl->m_shaderPrograms) {
      sp->use();
      sp->setVec3("pointLights[" + std::to_string(i) + "].position",
                  pl->m_position);
      sp->setVec3("pointLights[" + std::to_string(i) + "].ambient",
                  pl->m_ambient);
      sp->setVec3("pointLights[" + std::to_string(i) + "].diffuse",
                  pl->m_diffuse);
      sp->setVec3("pointLights[" + std::to_string(i) + "].specular",
                  pl->m_specular);
      sp->setInt("nbPointLights", i + 1);
    }
    i++;
  }

  // Drawing render objects
  for (auto &pair : m_renderObjects) {
    auto ro = pair.second;

    auto shaderProgram = ro->getShaderProgram();
    Mesh *mesh = ro->getMesh();

    if (shaderProgram) {
      shaderProgram->use();
      shaderProgram->setMat4("model", ro->getModelMatrix());
      shaderProgram->setMat4("view", m_camera->viewmatrix());
      shaderProgram->setMat4("projection", perspective);
      shaderProgram->setVec3("viewPos", m_camera->position());

      std::shared_ptr<Material> m;
      if (m = ro->getMaterial()) {

        // Set shader material
        shaderProgram->setInt("material.shininess", m->shininess);

        if (m->isTextureMaterial) {
          glActiveTexture(GL_TEXTURE0);
          glBindTexture(GL_TEXTURE_2D, m->diffuseTexture->m_textureID);
          shaderProgram->setInt("material.diffuse", 0);

          glActiveTexture(GL_TEXTURE1);
          glBindTexture(GL_TEXTURE_2D, m->specularTexture->m_textureID);
          shaderProgram->setInt("material.specular", 1);

        } else {
          shaderProgram->setVec3("material.diffuse", m->diffuse);
          shaderProgram->setVec3("material.specular", m->specular);
          shaderProgram->setFloat("material.roughness", m->roughness);
          shaderProgram->setFloat("material.metallic", m->metallic);
        }
      }

      glBindVertexArray(mesh->m_VAO);
      glDrawElements(mesh->m_mode, mesh->m_nbIndices, GL_UNSIGNED_INT, 0);
    } else {
      std::cout
          << "Renderer::draw: WARNING: render object has no shader program set"
          << std::endl;
    }
  }
  glBindVertexArray(0);
}