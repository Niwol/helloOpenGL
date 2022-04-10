#include "BezierSurface.hpp"

BezierSurface::BezierSurface(uint nbPoints_line, uint nbPoints_row)
    : m_nbLines{0}, m_nbRows{0}, m_nbPoints_line{nbPoints_line},
      m_nbPoints_row{nbPoints_row} {
  m_RO_surface = std::make_shared<RenderObject>();
}

BezierSurface::~BezierSurface() {}

void BezierSurface::transform(glm::mat4 transform) {
  m_RO_surface->transform(transform);

  for (auto &ro : m_ROs_controlPoints) {
    ro->transform(transform);
  }
}

void BezierSurface::addLine(std::vector<glm::vec3> points, bool bGenSurface) {

  if (m_controlPoints.size() == 0 || points.size() == m_nbRows) {

    auto m = std::make_shared<Material>();
    m->diffuse = glm::vec3(1.0f, 1.0f, 0.2f);
    m->specular = glm::vec3(0.5f, 0.5f, 0.5f);
    m->shininess = 32;
    m->isTextureMaterial = false;

    for (auto &p : points) {
      m_controlPoints.push_back(p);
      auto ro = std::make_shared<RenderObject>();
      ro->getMesh()->to_cube();
      ro->setMaterial(m);

      auto t = glm::mat4(1.0f);
      t = glm::translate(t, p);
      t = glm::scale(t, glm::vec3(0.05, 0.05, 0.05));
      ro->transform(t);
      m_ROs_controlPoints.push_back(ro);
    }

    m_nbLines += 1;
    m_nbRows = points.size();

    genBinomialCoeff();

    // Surface generation can be long if it has a high precision, this boolean
    // allows the user to generate the surface once all control point lines have
    // been added
    if (bGenSurface)
      genSurface();
  }
}

void BezierSurface::setPoint(glm::vec3 point, uint index) {
  if (index < m_nbRows * m_nbLines) {
    m_controlPoints[index] = point;

    auto t = glm::mat4(1.0f);
    t = glm::translate(t, point);
    t = glm::scale(t, glm::vec3(0.05f));
    m_ROs_controlPoints[index]->resetModel();
    m_ROs_controlPoints[index]->transform(t);
  } else {
    std::cout << "WARNING: BezierSurface::setPoint: index out of range"
              << std::endl;
  }

  genSurface();
}

void BezierSurface::move(glm::vec3 direction, uint index) {
  if (index < m_nbRows * m_nbLines) {
    m_controlPoints[index] += direction;

    glm::vec3 point = m_controlPoints[index];
    auto t = glm::mat4(1.0f);
    t = glm::translate(t, point);
    t = glm::scale(t, glm::vec3(0.05f));
    m_ROs_controlPoints[index]->resetModel();
    m_ROs_controlPoints[index]->transform(t);

  } else {
    std::cout << "WARNING: BezierSurface::setPoint: index out of range"
              << std::endl;
  }

  genSurface();
}

void BezierSurface::genSurface(bool printWarning) {
  if (m_nbLines >= 2 && m_nbRows >= 2) {
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> uvCoords;

    Mesh *mesh = m_RO_surface->getMesh();

    // Vertices / UV coords
    for (uint j = 0; j < m_nbPoints_row; j++) {
      for (uint i = 0; i < m_nbPoints_line; i++) {
        glm::vec3 p = glm::vec3(0.0f);

        float u = float(i) / (m_nbPoints_line - 1);
        float v = float(j) / (m_nbPoints_row - 1);

        for (uint k = 0; k < m_nbRows; k++) {
          for (uint l = 0; l < m_nbLines; l++) {
            p += N(k, l, u, v) * m_controlPoints[l * m_nbRows + k];
          }
        }

        vertices.push_back(p.x);
        vertices.push_back(p.y);
        vertices.push_back(p.z);

        uvCoords.push_back(u);
        uvCoords.push_back(v);
      }
    }

    // Indices
    for (uint i = 0; i < m_nbPoints_line - 1; i++) {
      for (uint j = 0; j < m_nbPoints_row - 1; j++) {
        int idx = j * m_nbPoints_line + i;

        // Tri 1
        indices.push_back(idx + m_nbPoints_line);
        indices.push_back(idx);
        indices.push_back(idx + m_nbPoints_line + 1);

        // Tri 2
        indices.push_back(idx);
        indices.push_back(idx + 1);
        indices.push_back(idx + m_nbPoints_line + 1);
      }
    }

    // Normals
    normals.resize(vertices.size(), 0.0f);

    for (uint i = 0; i < indices.size(); i += 3) {

      glm::vec3 p1, p2, p3;
      p1.x = vertices[indices[i + 0] * 3 + 0];
      p1.y = vertices[indices[i + 0] * 3 + 1];
      p1.z = vertices[indices[i + 0] * 3 + 2];
      p2.x = vertices[indices[i + 1] * 3 + 0];
      p2.y = vertices[indices[i + 1] * 3 + 1];
      p2.z = vertices[indices[i + 1] * 3 + 2];
      p3.x = vertices[indices[i + 2] * 3 + 0];
      p3.y = vertices[indices[i + 2] * 3 + 1];
      p3.z = vertices[indices[i + 2] * 3 + 2];

      glm::vec3 v1, v2;
      v1 = p2 - p1;
      v2 = p3 - p1;

      glm::vec3 n = glm::cross(v1, v2);
      n = glm::normalize(n);

      normals[indices[i + 0] * 3 + 0] += n.x;
      normals[indices[i + 0] * 3 + 1] += n.y;
      normals[indices[i + 0] * 3 + 2] += n.z;
      normals[indices[i + 1] * 3 + 0] += n.x;
      normals[indices[i + 1] * 3 + 1] += n.y;
      normals[indices[i + 1] * 3 + 2] += n.z;
      normals[indices[i + 2] * 3 + 0] += n.x;
      normals[indices[i + 2] * 3 + 1] += n.y;
      normals[indices[i + 2] * 3 + 2] += n.z;
    }

    for (uint i = 0; i < normals.size(); i += 3) {
      glm::vec3 n;
      n.x = normals[i + 0];
      n.y = normals[i + 1];
      n.z = normals[i + 2];

      n = glm::normalize(n);
      normals[i + 0] = n.x;
      normals[i + 1] = n.y;
      normals[i + 2] = n.z;
    }

    mesh->set_vertices(vertices);
    mesh->set_indices(indices, GL_TRIANGLES);
    mesh->set_normals(normals);
    mesh->set_uvCoords(uvCoords);

  } else if (printWarning) {
    std::cout
        << "WARNING: BezierSurface::genSurface: not enought control points"
        << std::endl;
  }
}

float BezierSurface::N(int i, int j, float u, float v) {

  float res = m_binomialCoeff_line[i] * std::pow(u, i) *
              std::pow(1 - u, m_nbRows - i - 1);

  res *= m_binomialCoeff_row[j] * std::pow(v, j) *
         std::pow(1 - v, m_nbLines - j - 1);

  return res;
}

void BezierSurface::genBinomialCoeff() {
  // Lines
  m_binomialCoeff_line.clear();
  m_binomialCoeff_line.resize(m_nbRows, 0);
  m_binomialCoeff_line[0] = 1;

  for (uint i = 0; i < m_nbRows; i++) {
    for (uint j = i; j > 0; j--) {
      m_binomialCoeff_line[j] =
          m_binomialCoeff_line[j] + m_binomialCoeff_line[j - 1];
    }
  }

  // Rows
  m_binomialCoeff_row.clear();
  m_binomialCoeff_row.resize(m_nbLines, 0);
  m_binomialCoeff_row[0] = 1;

  for (uint i = 0; i < m_nbLines; i++) {
    for (uint j = i; j > 0; j--) {
      m_binomialCoeff_row[j] =
          m_binomialCoeff_row[j] + m_binomialCoeff_row[j - 1];
    }
  }
}
