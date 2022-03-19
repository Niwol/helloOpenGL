#pragma once

#include "Geometry/mesh.hpp"
#include "opengl_stuff.h"
#include <glm.hpp>

#include <memory>

class RenderObject {
public:
  RenderObject();
  ~RenderObject();

  /**
   * Applyes the transform matrix to the current modle matrix
   */
  void transform(glm::mat4 transform);

  /**
   * Resets the model matrix to an identity matrix
   */
  void resetModel();

  /**
   * This function takes ownership of the mesh and deletes it when it is
   * destroyed or if an other mesh is set
   */
  void setMesh(std::unique_ptr<Mesh> mesh);

  /**
   * Returns a raw pointer to the mesh owned by the render object
   */
  Mesh *getMesh();
  glm::mat4 getModelMatrix();

private:
  std::unique_ptr<Mesh> m_mesh;
  glm::mat4 m_modelMatix;
};
