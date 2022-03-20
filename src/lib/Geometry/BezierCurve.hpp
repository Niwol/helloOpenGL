#pragma once

#include <memory>
#include <vector>

#include "lib/RenderObject.hpp"

#include "opengl_stuff.h"

class BezierCurve {
public:
  BezierCurve(uint nbPoints = 50);
  ~BezierCurve();

  /**
   * Applyes the transform matrix to both the curve render object and the
   * control polynom render object
   */
  void transform(glm::mat4 transform);

  void addPoint(glm::vec3 position);
  void setPoint(glm::vec3 position, uint index);
  void movePoint(glm::vec3 position, uint index);

  inline int get_nbControlPoints() { return m_controlPoints.size(); }

  inline std::shared_ptr<RenderObject> get_ro_curve() { return m_RO_curve; }
  inline std::shared_ptr<RenderObject> get_ro_controlPolynom() {
    return m_RO_controlPolynom;
  }

private:
  void genCurve(bool printWarning = false);
  float N(int index, float u);
  void genBinomialCoeff();

private:
  std::vector<glm::vec3> m_controlPoints;
  std::vector<uint> m_binomialCoeff;
  uint m_nbControlPoints;
  uint m_nbPoints;

  std::shared_ptr<RenderObject> m_RO_curve;
  std::shared_ptr<RenderObject> m_RO_controlPolynom;
};
