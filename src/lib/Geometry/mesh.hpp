#pragma once

#include "opengl_stuff.h"

#include <iostream>
#include <vector>

class Mesh {
public:
  Mesh();
  Mesh(const Mesh &) = delete;
  Mesh &operator=(const Mesh &) = delete;

  ~Mesh();

  void to_cube();
  void to_square();
  void to_line();
  void commit();

  void set_vertices(std::vector<GLfloat> vertices);
  void set_normals(std::vector<GLfloat> normals);
  void set_uvCoords(std::vector<GLfloat> uvCoords);
  void set_indices(std::vector<GLuint> indices, int mode);

private:
  friend class Renderer;

  GLuint m_VAO;
  GLuint m_VBO;
  GLuint m_NBO;
  GLuint m_UVBO;
  GLuint m_EBO;

  std::vector<GLfloat> m_vertices;
  std::vector<GLfloat> m_normals;
  std::vector<GLfloat> m_uvCoords;
  std::vector<GLuint> m_indices;

  uint m_nbIndices;
  int m_mode;
};
