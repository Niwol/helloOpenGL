#include "BezierSurface.hpp"

BezierSurface::BezierSurface(uint nbPoints)
    : m_nbRows{0}, m_nbLines{0}, m_nbPoints{nbPoints} {
  m_RO_surface = std::make_shared<RenderObject>();
}

BezierSurface::~BezierSurface() {}

void BezierSurface::transform(glm::mat4 transform) {
  m_RO_surface->transform(transform);

  for (auto &ro : m_ROs_controlPoints) {
    ro->transform(transform);
  }
}

void BezierSurface::addRow(std::vector<glm::vec3> points) {
  // TODO
}
void BezierSurface::addLine(std::vector<glm::vec3> points) {
  // TODO
}

void BezierSurface::setPoint(glm::vec3 point, uint index) {
  if (index < m_nbRows * m_nbLines) {
    m_controlPoints[index] = point;
  } else {
    std::cout << "WARNING: BezierSurface::setPoint: index out of range"
              << std::endl;
  }
}

void BezierSurface::movePoint(glm::vec3 point, uint index) {
  if (index < m_nbRows * m_nbLines) {
    m_controlPoints[index] += point;
  } else {
    std::cout << "WARNING: BezierSurface::setPoint: index out of range"
              << std::endl;
  }
}

void BezierSurface::genSurface() {
  // TODO
}

void BezierSurface::genControlPoints() {
  // TODO
}

float BezierSurface::N(int index, float u) {
  // TODO
  return 0.0f;
}

void BezierSurface::genBinomialCoeff() {
  // TODO
}
