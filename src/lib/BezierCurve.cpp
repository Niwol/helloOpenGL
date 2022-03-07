#include "BezierCurve.hpp"

BezierCurve::BezierCurve(uint nbPoints)
    : m_nbControlPoints{0}, m_nbPoints{nbPoints} {}
BezierCurve::~BezierCurve() {}

void BezierCurve::addPoint(glm::vec3 point) {
  m_controlPoints.push_back(point);
  m_nbControlPoints += 1;
  genBinomialCoeff();
}

void BezierCurve::setPoint(glm::vec3 point, uint index) {
  if (index < m_nbControlPoints) {
    m_controlPoints[index] = point;
  } else {
    std::cout << "WARNING: BezierCurve::setPoint: index out of range"
              << std::endl;
  }
}

void BezierCurve::movePoint(glm::vec3 point, uint index) {
  if (index < m_nbControlPoints) {
    m_controlPoints[index] += point;
  } else {
    std::cout << "WARNING: BezierCurve::setPoint: index out of range"
              << std::endl;
  }
}

void BezierCurve::genCurve(std::vector<GLfloat> &vertices,
                           std::vector<GLuint> &indices) {
  if (m_controlPoints.size() >= 2) {
    vertices.clear();
    indices.clear();

    for (uint i = 0; i < m_nbPoints; i++) {
      glm::vec3 p = glm::vec3(0.0f, 0.0f, 0.0f);

      float u = float(i) / (m_nbPoints - 1);
      for (uint j = 0; j < m_nbControlPoints; j++) {
        p += N(j, u) * m_controlPoints[j];
      }

      vertices.push_back(p.x);
      vertices.push_back(p.y);
      vertices.push_back(p.z);

      if (i > 0) {
        indices.push_back(i - 1);
        indices.push_back(i);
      }
    }

  } else {
    std::cout << "WARNING: BezierCurve::genCurve: not enought control points"
              << std::endl;
  }
}

void BezierCurve::genControlPoints(std::vector<GLfloat> &vertices,
                                   std::vector<GLuint> &indices) {
  vertices.clear();
  indices.clear();

  for (uint i = 0; i < m_nbControlPoints; i++) {
    vertices.push_back(m_controlPoints[i].x);
    vertices.push_back(m_controlPoints[i].y);
    vertices.push_back(m_controlPoints[i].z);

    if (i > 0) {
      indices.push_back(i - 1);
      indices.push_back(i);
    }
  }
}

float BezierCurve::N(int index, float u) {

  float res = std::pow(u, index) * std::pow(1 - u, m_nbControlPoints - index);

  return m_binomialCoeff[index] * res;
}

void BezierCurve::genBinomialCoeff() {
  m_binomialCoeff.clear();
  m_binomialCoeff.resize(m_nbControlPoints, 0);
  m_binomialCoeff[0] = 1;

  for (uint i = 0; i < m_nbControlPoints; i++) {
    for (uint j = i; j > 0; j--) {
      m_binomialCoeff[j] = m_binomialCoeff[j] + m_binomialCoeff[j - 1];
    }
  }
}
