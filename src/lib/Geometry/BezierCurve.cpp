#include "BezierCurve.hpp"

BezierCurve::BezierCurve(bool isSegementLegthCurve, float segmentLength,
                         float step, uint nbPoints)
    : m_nbControlPoints{0}, m_nbPoints{nbPoints},
      m_isSegmentLengthCurve{isSegementLegthCurve},
      m_segmentLength{segmentLength}, m_step{step} {
  m_RO_curve = std::make_shared<RenderObject>();
  m_RO_controlPolynom = std::make_shared<RenderObject>();
}

BezierCurve::~BezierCurve() {}

void BezierCurve::transform(glm::mat4 transform) {
  m_RO_curve->transform(transform);
  m_RO_controlPolynom->transform(transform);
}

void BezierCurve::addPoint(glm::vec3 point, bool bGenCurve) {
  m_controlPoints.push_back(point);
  m_nbControlPoints += 1;
  genBinomialCoeff();

  if (bGenCurve)
    genCurve();
}

void BezierCurve::setPoint(glm::vec3 point, uint index) {
  if (index < m_nbControlPoints) {
    m_controlPoints[index] = point;
  } else {
    std::cout << "WARNING: BezierCurve::setPoint: index out of range"
              << std::endl;
  }

  genCurve();
}

void BezierCurve::move(glm::vec3 point, uint index) {
  if (index < m_nbControlPoints) {
    m_controlPoints[index] += point;
  } else {
    std::cout << "WARNING: BezierCurve::setPoint: index out of range"
              << std::endl;
  }

  genCurve();
}

void BezierCurve::genCurve(bool printWarning) {
  if (m_isSegmentLengthCurve) {
    genCurve_segmentLength(printWarning);
  } else
    genCurve_normal(printWarning);
}

void BezierCurve::genCurve_normal(bool printWarning) {
  if (m_controlPoints.size() >= 2) {
    // Curve
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    Mesh *mesh = m_RO_curve->getMesh();

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

    mesh->set_vertices(vertices);
    mesh->set_indices(indices, GL_LINES);

    // Control polynom
    vertices.clear();
    indices.clear();

    mesh = m_RO_controlPolynom->getMesh();

    for (uint i = 0; i < m_nbControlPoints; i++) {
      vertices.push_back(m_controlPoints[i].x);
      vertices.push_back(m_controlPoints[i].y);
      vertices.push_back(m_controlPoints[i].z);

      if (i > 0) {
        indices.push_back(i - 1);
        indices.push_back(i);
      }
    }

    mesh->set_vertices(vertices);
    mesh->set_indices(indices, GL_LINES);

  } else if (printWarning) {
    std::cout << "WARNING: BezierCurve::genCurve: not enought control points"
              << std::endl;
  }
}

void BezierCurve::genCurve_segmentLength(bool printWarning) {
  if (m_controlPoints.size() >= 2) {
    // Curve
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    Mesh *mesh = m_RO_curve->getMesh();

    float u = 0.0f;

    // First point
    glm::vec3 p_prev = glm::vec3(0.0f);
    for (uint j = 0; j < m_nbControlPoints; j++) {
      p_prev += N(j, u) * m_controlPoints[j];
    }

    vertices.push_back(p_prev.x);
    vertices.push_back(p_prev.y);
    vertices.push_back(p_prev.z);

    int i = 1;
    while (u < 1.0f) {
      glm::vec3 p = glm::vec3(0.0f, 0.0f, 0.0f);

      for (uint j = 0; j < m_nbControlPoints; j++) {
        p += N(j, u) * m_controlPoints[j];
      }

      u += m_step;
      if (glm::length(p_prev - p) < m_segmentLength) {
        continue;
      }

      p_prev = p;

      vertices.push_back(p.x);
      vertices.push_back(p.y);
      vertices.push_back(p.z);

      if (i > 0) {
        indices.push_back(i - 1);
        indices.push_back(i);
      }
      i++;
    }

    // Last point
    u = 1.0f;
    glm::vec3 p = glm::vec3(0.0f);
    for (uint j = 0; j < m_nbControlPoints; j++) {
      p += N(j, u) * m_controlPoints[j];
    }

    vertices.push_back(p.x);
    vertices.push_back(p.y);
    vertices.push_back(p.z);

    indices.push_back(i - 1);
    indices.push_back(i);

    // Mesh
    mesh->set_vertices(vertices);
    mesh->set_indices(indices, GL_LINES);

    // Control polynom
    vertices.clear();
    indices.clear();

    mesh = m_RO_controlPolynom->getMesh();

    for (uint i = 0; i < m_nbControlPoints; i++) {
      vertices.push_back(m_controlPoints[i].x);
      vertices.push_back(m_controlPoints[i].y);
      vertices.push_back(m_controlPoints[i].z);

      if (i > 0) {
        indices.push_back(i - 1);
        indices.push_back(i);
      }
    }

    mesh->set_vertices(vertices);
    mesh->set_indices(indices, GL_LINES);

  } else if (printWarning) {
    std::cout << "WARNING: BezierCurve::genCurve: not enought control points"
              << std::endl;
  }
}

float BezierCurve::N(int index, float u) {

  float res =
      std::pow(u, index) * std::pow(1 - u, m_nbControlPoints - index - 1);

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
