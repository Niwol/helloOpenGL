#pragma once

#include "ShaderProgram.hpp"
#include "mesh.hpp"
#include "opengl_stuff.h"

#include <vector>

class Renderer {
public:
  Renderer();
  ~Renderer();

  void addMesh(Mesh *mesh);
  void setShaderProgram(ShaderProgram *sp);

  void draw();

private:
  std::vector<Mesh *> m_meshes;
  ShaderProgram *m_shaderProgram;
};
