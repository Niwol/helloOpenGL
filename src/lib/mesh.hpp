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

  void to_square();
  void use();                                        // temporary
  inline uint getNbIndices() { return m_nbIndices; } // temporary

private:
  friend class Renderer;

  GLuint m_VAO;
  GLuint m_VBO;
  GLuint m_EBO;
  GLuint m_NBO;

  uint m_nbIndices;
  int m_type;
};
