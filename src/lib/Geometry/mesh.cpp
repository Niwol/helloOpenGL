#include "mesh.hpp"

Mesh::Mesh() 
{
  glGenBuffers(1, &m_VBO);
  glGenBuffers(1, &m_EBO);
  glGenBuffers(1, &m_NBO);
  glGenBuffers(1, &m_UVBO);
  glGenBuffers(1, &m_CBO);

  glGenVertexArrays(1, &m_VAO);

  m_nbIndices = 0;
  m_mode = 0;
}

Mesh::~Mesh() 
{
  glDeleteBuffers(1, &m_VBO);
  glDeleteBuffers(1, &m_NBO);
  glDeleteBuffers(1, &m_EBO);
  glDeleteBuffers(1, &m_UVBO);
  glDeleteBuffers(1, &m_CBO);

  glDeleteVertexArrays(1, &m_VAO);
}

void Mesh::to_cube() 
{

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

  m_uvCoords = {
    // Front face
    0.0f, 1.0f,   // top left
    0.0f, 0.0f,   // bottom left
    1.0f, 1.0f,   // top rigth
    1.0f, 0.0f,   // bottom rigth

    // Back face
    0.0f, 1.0f,   // top left
    0.0f, 0.0f,   // bottom left
    1.0f, 1.0f,   // top rigth
    1.0f, 0.0f,   // bottom rigth
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

void Mesh::to_sharp_cube() 
{

  // clang-format off
  m_vertices = {
    // Front face
    -0.5f, -0.5f,  0.5f,    // Bottom left
    -0.5f,  0.5f,  0.5f,    // Top left
     0.5f,  0.5f,  0.5f,    // top rigth
     0.5f, -0.5f,  0.5f,    // bottom rigth

    // Right face
     0.5f, -0.5f,  0.5f,    // Bottom left
     0.5f,  0.5f,  0.5f,    // Top left
     0.5f,  0.5f, -0.5f,    // Top right
     0.5f, -0.5f, -0.5f,    // Bottom rigth

    // Back face
     0.5f, -0.5f, -0.5f,    // Bottom left
     0.5f,  0.5f, -0.5f,    // Top left
    -0.5f,  0.5f, -0.5f,    // Top right
    -0.5f, -0.5f, -0.5f,    // Bottom right

    // Left face
    -0.5f, -0.5f, -0.5f,    // Bottom left
    -0.5f,  0.5f, -0.5f,    // Top left
    -0.5f,  0.5f,  0.5f,    // Top right
    -0.5f, -0.5f,  0.5f,    // Bottom right

    // Top face
    -0.5f,  0.5f,  0.5f,    // Bottom left
    -0.5f,  0.5f, -0.5f,    // Top left
     0.5f,  0.5f, -0.5f,    // Top right
     0.5f,  0.5f,  0.5f,    // Bottom right
    
    // Bottom face
    -0.5f, -0.5f, -0.5f,    // Bottom left
    -0.5f, -0.5f,  0.5f,    // Top left
     0.5f, -0.5f,  0.5f,    // Top right
     0.5f, -0.5f, -0.5f     // Bottom right
  };

  glm::vec3 normal = glm::vec3(1.0f);
  normal = glm::normalize(normal);

  m_normals = {
    // Front face
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

    // Right face
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    // Back face
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,

    // Left face
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,

    // Top face
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    // Bottom face
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f
  };

  m_uvCoords = {
    // Front face
    0.0f, 0.0f,   // bottom left
    0.0f, 1.0f,   // top left
    1.0f, 1.0f,   // top rigth
    1.0f, 0.0f,   // bottom rigth
    
    // Right face
    0.0f, 0.0f,   // bottom left
    0.0f, 1.0f,   // top left
    1.0f, 1.0f,   // top rigth
    1.0f, 0.0f,   // bottom rigth

    // Back face
    0.0f, 0.0f,   // bottom left
    0.0f, 1.0f,   // top left
    1.0f, 1.0f,   // top rigth
    1.0f, 0.0f,   // bottom rigth

    // Left face
    0.0f, 0.0f,   // bottom left
    0.0f, 1.0f,   // top left
    1.0f, 1.0f,   // top rigth
    1.0f, 0.0f,   // bottom rigth
    
    // Top face
    0.0f, 0.0f,   // bottom left
    0.0f, 1.0f,   // top left
    1.0f, 1.0f,   // top rigth
    1.0f, 0.0f,   // bottom rigth
    
    // Bottom face
    0.0f, 0.0f,   // bottom left
    0.0f, 1.0f,   // top left
    1.0f, 1.0f,   // top rigth
    1.0f, 0.0f,   // bottom rigth
  };

  m_indices = {
    // Front face
    0, 2, 1,
    0, 3, 2,

    // Right face
    4, 6, 5,
    4, 7, 6,

    // Back face
    8, 10, 9,
    8, 11, 10,

    // Left face
    12, 14, 13,
    12, 15, 14,

    // Top face
    16, 18, 17,
    16, 19, 18,

    // Bottom face
    20, 22, 21,
    20, 23, 22
  };
  // clang-format on

  m_nbIndices = m_indices.size();
  m_mode = GL_TRIANGLES;

  commit();
}

void Mesh::to_cube2()
{
  m_mesh.clean();  

  MyMesh::VertexHandle vhandels[8];

  vhandels[0] = m_mesh.add_vertex(MyMesh::Point(-1, -1,  1));
  vhandels[1] = m_mesh.add_vertex(MyMesh::Point(-1,  1,  1));
  vhandels[2] = m_mesh.add_vertex(MyMesh::Point( 1,  1,  1));
  vhandels[3] = m_mesh.add_vertex(MyMesh::Point( 1, -1,  1));
  vhandels[4] = m_mesh.add_vertex(MyMesh::Point(-1, -1, -1));
  vhandels[5] = m_mesh.add_vertex(MyMesh::Point(-1,  1, -1));
  vhandels[6] = m_mesh.add_vertex(MyMesh::Point( 1,  1, -1));
  vhandels[7] = m_mesh.add_vertex(MyMesh::Point( 1, -1, -1));

  std::vector<MyMesh::VertexHandle> face;

  // Front
  face.clear();
  face.push_back(vhandels[0]);
  face.push_back(vhandels[1]);
  face.push_back(vhandels[2]);
  face.push_back(vhandels[3]);
  m_mesh.add_face(face);

  // Top
  face.clear();
  face.push_back(vhandels[1]);
  face.push_back(vhandels[5]);
  face.push_back(vhandels[6]);
  face.push_back(vhandels[2]);
  m_mesh.add_face(face);
  
  // Back
  face.clear();
  face.push_back(vhandels[5]);
  face.push_back(vhandels[4]);
  face.push_back(vhandels[7]);
  face.push_back(vhandels[6]);
  m_mesh.add_face(face);

  // Bottom
  face.clear();
  face.push_back(vhandels[4]);
  face.push_back(vhandels[0]);
  face.push_back(vhandels[3]);
  face.push_back(vhandels[7]);
  m_mesh.add_face(face);

  // Left
  face.clear();
  face.push_back(vhandels[4]);
  face.push_back(vhandels[5]);
  face.push_back(vhandels[1]);
  face.push_back(vhandels[0]);
  m_mesh.add_face(face);
  
  // Right
  face.clear();
  face.push_back(vhandels[6]);
  face.push_back(vhandels[7]);
  face.push_back(vhandels[3]);
  face.push_back(vhandels[2]);
  m_mesh.add_face(face);

  m_mesh.request_vertex_normals();
  m_mesh.request_face_normals();

  m_mesh.update_normals();

  m_mesh.release_face_normals();

  m_mode = GL_TRIANGLES;
  commit2();
}

void Mesh::to_square() 
{
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

void Mesh::to_line() 
{
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

void Mesh::commit() 
{
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

  // UVBO
  glBindBuffer(GL_ARRAY_BUFFER, m_UVBO);
  glBufferData(GL_ARRAY_BUFFER, m_uvCoords.size() * sizeof(GLfloat),
               m_uvCoords.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
                        (GLvoid *)0);
  glEnableVertexAttribArray(2);

  // EBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint),
               m_indices.data(), GL_STATIC_DRAW);

  glBindVertexArray(0);
  // **************  BUFFERS  **************
}

void Mesh::commit2() 
{
  glBindVertexArray(m_VAO);

  // ***************************** VBO *****************************
  std::vector<GLfloat> vertices;
  std::vector<GLfloat> normals;

  for(auto v_iter = m_mesh.vertices().begin(); 
           v_iter != m_mesh.vertices().end(); 
           v_iter++)
  {
    auto p = m_mesh.point(*v_iter);
    vertices.push_back(p.data()[0]);
    vertices.push_back(p.data()[1]);
    vertices.push_back(p.data()[2]);

//    if(m_mesh.has_vertex_normals())
//    {
//      auto n = m_mesh.normal(*v_iter);
//      normals.push_back(n.data()[0]);
//      normals.push_back(n.data()[1]);
//      normals.push_back(n.data()[2]);
//
////      int i = normals.size() - 3;
////      float norm = glm::length(glm::vec3(normals[i], normals[i+1], normals[i+2]));
////      std::cout << normals[i + 0] << " " 
////                << normals[i + 1] << " " 
////                << normals[i + 2] << std::endl;
////      std::cout << norm << std::endl;
//    }
  }

  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
               vertices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                        (GLvoid *)0);
  glEnableVertexAttribArray(0);


  // ***************************** EBO *****************************
  std::vector<GLuint> indices;

  for(auto& f : m_mesh.faces())
  {
    auto verts = f.vertices_cw().begin();

    int idx0 = verts->idx();
    int idx1 = (++verts)->idx();
    int idx2 = (++verts)->idx();

    while(verts != f.vertices_cw().end())
    {
      indices.push_back(idx0);
      indices.push_back(idx1);
      indices.push_back(idx2);

      idx1 = idx2;
      idx2 = (++verts)->idx();
    }
  }

  m_nbIndices = indices.size();
  if(m_mesh.is_trimesh() || m_mesh.is_polymesh())
    m_mode = GL_TRIANGLES;

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
               indices.data(), GL_STATIC_DRAW);



  // ***************************** NBO *****************************
  if(m_mesh.has_vertex_normals())
  {
    std::vector<GLfloat> normals;
    for(auto v_iter = m_mesh.vertices_begin();
             v_iter != m_mesh.vertices_end();
             ++v_iter)
    {
      normals.push_back(m_mesh.normal(*v_iter).data()[0]);
      normals.push_back(m_mesh.normal(*v_iter).data()[1]);
      normals.push_back(m_mesh.normal(*v_iter).data()[2]);
    }


    glBindBuffer(GL_ARRAY_BUFFER, m_NBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat),
                 normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                          (GLvoid *)0);
    glEnableVertexAttribArray(1);
  }


  // ***************************** CBO *****************************
  if(m_mesh.has_vertex_colors())
  {
    std::vector<GLfloat> colors;
    for(auto v_iter = m_mesh.vertices().begin();
             v_iter != m_mesh.vertices().end();
             v_iter++)
    {
      MyMesh::Color color = m_mesh.color(*v_iter);
      MyColor fColor = color_uintToFloat(color);

      colors.push_back(fColor[0]);
      colors.push_back(fColor[1]);
      colors.push_back(fColor[2]);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_CBO);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), 
                 colors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                         (GLvoid*)0);
    glEnableVertexAttribArray(3);
  }


  glBindVertexArray(0);
}

void Mesh::set_vertices(std::vector<GLfloat> vertices) 
{
  m_vertices = vertices;
  commit();
}

void Mesh::set_normals(std::vector<GLfloat> normals) 
{
  m_normals = normals;
  commit();
}

void Mesh::set_indices(std::vector<GLuint> indices, int mode) 
{
  m_indices = indices;
  m_nbIndices = indices.size();
  m_mode = mode;
  commit();
}

void Mesh::set_uvCoords(std::vector<GLfloat> uvCoords) 
{
  m_uvCoords = uvCoords;
  commit();
}
