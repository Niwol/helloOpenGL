#pragma once

#include <memory>
#include <vector>

#include "lib/RenderObject.hpp"
#include "lib/renderer.hpp"

#include "opengl_stuff.h"

class BezierSurface {
public:
  BezierSurface(uint nbPoints_line = 50, uint nbPoints_row = 50);
  ~BezierSurface();

  /**
   * Applyes the transform matrix to both the surface render object and the
   * control point render objects
   */
  void transform(glm::mat4 transform);

  void addLine(std::vector<glm::vec3> points);
  void setPoint(glm::vec3 position, uint index);
  void movePoint(glm::vec3 position, uint index);

  inline int get_nbRows() { return m_controlPoints.size(); }
  inline int get_nbLines() { return m_controlPoints.size(); }

  void genSurface(bool printWarning = false);

  inline std::shared_ptr<RenderObject> get_ro_surface() { return m_RO_surface; }
  inline std::vector<std::shared_ptr<RenderObject>> get_ro_controlPoints() {
    return m_ROs_controlPoints;
  }

private:
  float N(int i, int j, float u, float v);
  void genBinomialCoeff();

private:
  std::vector<glm::vec3> m_controlPoints;
  std::vector<uint> m_binomialCoeff_line;
  std::vector<uint> m_binomialCoeff_row;
  uint m_nbLines;
  uint m_nbRows;
  uint m_nbPoints_line;
  uint m_nbPoints_row;

  std::shared_ptr<RenderObject> m_RO_surface;
  std::vector<std::shared_ptr<RenderObject>> m_ROs_controlPoints;
};
