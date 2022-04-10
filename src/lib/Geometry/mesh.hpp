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

  /**
   * Transformes the mesh into a cube
   */
  void to_cube();

  /**
   * Transforms the mesh into a square
   */
  void to_square();

  /**
   * Transforms the mesh into a line
   */
  void to_line();

  /**
   * Sends all the mesh data to the GPU
   */
  void commit();

  /**
   * Sets the meshes vertices
   */
  void set_vertices(std::vector<GLfloat> vertices);

  /**
   * Sets the meshes normals
   */
  void set_normals(std::vector<GLfloat> normals);

  /**
   * Sets the meshes uv coordonates
   */
  void set_uvCoords(std::vector<GLfloat> uvCoords);

  /**
   * Sets the meshes indices
   */
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
