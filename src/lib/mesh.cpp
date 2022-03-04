#include "mesh.hpp"

Mesh::Mesh() {
  glGenBuffers(1, &m_VBO);
  glGenBuffers(1, &m_EBO);
  glGenBuffers(1, &m_NBO);

  glGenVertexArrays(1, &m_VAO);

  m_nbIndices = 0;
  m_type = 0;
}

Mesh::~Mesh() {
  glDeleteBuffers(1, &m_VBO);
  glDeleteBuffers(1, &m_NBO);
  glDeleteBuffers(1, &m_EBO);

  glDeleteVertexArrays(1, &m_VAO);
}

void Mesh::to_square() {

  std::vector<GLfloat> vertices;
  std::vector<GLuint> indices;

  // clang-format off
  vertices = {
    -0.5f,  0.5f, 0.0f,   // top left
    -0.5f, -0.5f, 0.0f,   // bottom left
     0.5f,  0.5f, 0.0f,   // top rigth
     0.5f, -0.5f, 0.0f    // bottom rigth
  };

  indices = {
    0, 1, 2,
    1, 2, 3
  };
  m_nbIndices = indices.size();
  // clang-format on

  // **************  BUFFERS  **************
  glBindVertexArray(m_VAO);

  // VBO
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
               vertices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                        (GLvoid *)0);
  glEnableVertexAttribArray(0);

  // EBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
               indices.data(), GL_STATIC_DRAW);

  glBindVertexArray(0);
  // **************  BUFFERS  **************
}

void Mesh::use() { glBindVertexArray(m_VAO); }