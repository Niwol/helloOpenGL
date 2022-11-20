#pragma once

#include <iostream>
#include <vector>

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include "opengl_stuff.h"

typedef OpenMesh::PolyMesh_ArrayKernelT<> MyMesh;

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
  void to_cube2();

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
  void commit2();

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

public:
  MyMesh m_mesh;

private:
  friend class Renderer;

  GLuint m_VAO;
  GLuint m_VBO;
  GLuint m_NBO;
  GLuint m_UVBO;
  GLuint m_CBO;
  GLuint m_EBO;

  std::vector<GLfloat> m_vertices;
  std::vector<GLfloat> m_normals;
  std::vector<GLfloat> m_uvCoords;
  std::vector<GLuint> m_indices;

  uint m_nbIndices;
  int m_mode;
};
