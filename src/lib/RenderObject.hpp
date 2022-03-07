#pragma once

#include "mesh.hpp"
#include "opengl_stuff.h"
#include <glm.hpp>

class RenderObject {
public:
  RenderObject();
  ~RenderObject();

  /**
   * This function takes ownership of the mesh
   */
  void setMesh(Mesh *mesh);
  Mesh *getMesh();
  glm::mat4 getModelMatrix();

private:
  Mesh *m_mesh;
  glm::mat4 m_modelMatix;
};
