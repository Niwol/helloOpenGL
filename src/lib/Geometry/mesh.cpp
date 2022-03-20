#include "mesh.hpp"

Mesh::Mesh() {
  glGenBuffers(1, &m_VBO);
  glGenBuffers(1, &m_EBO);
  glGenBuffers(1, &m_NBO);

  glGenVertexArrays(1, &m_VAO);

  m_nbIndices = 0;
  m_mode = 0;
}

Mesh::~Mesh() {
  glDeleteBuffers(1, &m_VBO);
  glDeleteBuffers(1, &m_NBO);
  glDeleteBuffers(1, &m_EBO);

  glDeleteVertexArrays(1, &m_VAO);
}

void Mesh::to_cube() {

  // clang-format off
  m_vertices = {
    // Front face
    -0.5f,  0.5f,  0.5f,   // top left
    -0.5f, -0.5f,  0.5f,   // bottom left
     0.5f,  0.5f,  0.5f,   // top rigth
     0.5f, -0.5f,  0.5f,   // bottom rigth

    // Back face
    -0.5f,  0.5f, -0.5f,   // top left
    -0.5f, -0.5f, -0.5f,   // bottom left
     0.5f,  0.5f, -0.5f,   // top rigth
     0.5f, -0.5f, -0.5f    // bottom rigth
  };

  glm::vec3 normal = glm::vec3(1.0f);
  normal = glm::normalize(normal);

  m_normals = {
    // Front face
    -normal.x,  normal.y,  normal.z,   // top left
    -normal.x, -normal.y,  normal.z,   // bottom left
     normal.x,  normal.y,  normal.z,   // top rigth
     normal.x, -normal.y,  normal.z,   // bottom rigth

    // Back face
    -normal.x,  normal.y, -normal.z,   // top left
    -normal.x, -normal.y, -normal.z,   // bottom left
     normal.x,  normal.y, -normal.z,   // top rigth
     normal.x, -normal.y, -normal.z,   // bottom rigth
  };

  m_indices = {
    // Front face
    0, 1, 2,
    1, 3, 2,

    // Top face
    4, 0, 6,
    0, 2, 6,

    // Right face
    2, 3, 6,
    3, 7, 6,

    // Left face
    4, 5, 0,
    5, 1, 0,

    // Back face
    6, 7, 4,
    7, 5, 4,

    // Bottom face
    1, 5, 3,
    5, 7, 3
  };
  // clang-format on

  m_nbIndices = m_indices.size();
  m_mode = GL_TRIANGLES;

  commit();
}

void Mesh::to_square() {

  // clang-format off
  m_vertices = {
    -0.5f,  0.5f, 0.0f,   // top left
    -0.5f, -0.5f, 0.0f,   // bottom left
     0.5f,  0.5f, 0.0f,   // top rigth
     0.5f, -0.5f, 0.0f    // bottom rigth
  };

  m_indices = {
    0, 1, 2,
    1, 2, 3
  };
  // clang-format on

  m_nbIndices = m_indices.size();
  m_mode = GL_TRIANGLES;

  commit();
}

void Mesh::to_line() {

  // clang-format off
  m_vertices = {
    -0.5f,  -0.5f,  0.0f,   // 1
    -0.25f,  0.5f,  0.0f,   // 2
     0.0f,   0.20f, 0.0f,   // 3
     0.25f,  0.5f,  0.0f,   // 4
     0.5f,  -0.5f,  0.0f    // 5
  };

  m_indices = {
    0, 1, 
    1, 2, 
    2, 3,
    3, 4
  };
  // clang-format on

  m_nbIndices = m_indices.size();
  m_mode = GL_LINES;

  commit();
}

void Mesh::commit() {
  // **************  BUFFERS  **************
  glBindVertexArray(m_VAO);

  // VBO
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(GLfloat),
               m_vertices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                        (GLvoid *)0);
  glEnableVertexAttribArray(0);

  // NBO
  glBindBuffer(GL_ARRAY_BUFFER, m_NBO);
  glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(GLfloat),
               m_normals.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                        (GLvoid *)0);
  glEnableVertexAttribArray(1);

  // EBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint),
               m_indices.data(), GL_STATIC_DRAW);

  glBindVertexArray(0);
  // **************  BUFFERS  **************
}

void Mesh::set_vertices(std::vector<GLfloat> vertices) {
  m_vertices = vertices;
  commit();
}
void Mesh::set_normals(std::vector<GLfloat> normals) {
  m_normals = normals;
  commit();
}
void Mesh::set_indices(std::vector<GLuint> indices, int mode) {
  m_indices = indices;
  m_nbIndices = indices.size();
  m_mode = mode;
  commit();
}
