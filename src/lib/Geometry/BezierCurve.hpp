#pragma once

#include <memory>
#include <vector>

#include "lib/RenderObject.hpp"
#include "lib/moveable.hpp"

#include "opengl_stuff.h"

class BezierCurve : public Moveable {
public:
  BezierCurve(bool isSegementLegthCurve = false, float segmentLength = 0.5,
              float step = 0.01, uint nbPoints = 50);
  ~BezierCurve();

  /**
   * Applyes the transform matrix to both the curve render object and the
   * control polynom render object
   */
  void transform(glm::mat4 transform);

  /**
   * Adds a control point to the curve
   */
  void addPoint(glm::vec3 position, bool bGenCurve = false);

  /**
   * Sets the position of the control point at `index
   */
  void setPoint(glm::vec3 position, uint index);

  /**
   * Moves the control point at `index` in the direction given by `direction`
   */
  void move(glm::vec3 direction, uint index);

  /**
   * Generates the bezier curve
   */
  void genCurve(bool printWarning = false);

  /**
   * Returns the number of control points
   */
  inline int get_nbControlPoints() { return m_controlPoints.size(); }

  /**
   * Gets the curve render object
   */
  inline std::shared_ptr<RenderObject> get_ro_curve() { return m_RO_curve; }

  /**
   * Gets the control point polynom render object
   */
  inline std::shared_ptr<RenderObject> get_ro_controlPolynom() {
    return m_RO_controlPolynom;
  }

private:
  void genCurve_normal(bool printWarning = false);
  void genCurve_segmentLength(bool printWarning = false);
  float N(int index, float u);
  void genBinomialCoeff();

private:
  std::vector<glm::vec3> m_controlPoints;
  std::vector<uint> m_binomialCoeff;
  uint m_nbControlPoints;
  uint m_nbPoints;

  bool m_isSegmentLengthCurve;
  float m_segmentLength;
  float m_step;

  std::shared_ptr<RenderObject> m_RO_curve;
  std::shared_ptr<RenderObject> m_RO_controlPolynom;
};
