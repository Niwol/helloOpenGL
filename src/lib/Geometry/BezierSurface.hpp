#pragma once

#include <memory>
#include <vector>

#include "lib/RenderObject.hpp"
#include "lib/moveable.hpp"
#include "lib/renderer.hpp"

#include "opengl_stuff.h"

class BezierSurface : public Moveable {
public:
  BezierSurface(uint nbPoints_line = 50, uint nbPoints_row = 50);
  ~BezierSurface();

  /**
   * Applyes the transform matrix to both the surface render object and the
   * control point render objects
   */
  void transform(glm::mat4 transform);

  /**
   * Adds a line of control points.
   * The line must be the same length than the first line added.
   */
  void addLine(std::vector<glm::vec3> points, bool bGenSurface = false);

  /**
   * Sets the position of the control point at `index`
   */
  void setPoint(glm::vec3 position, uint index);

  /**
   * Moves the control point at `index` in the direction given by `direction`
   */
  void move(glm::vec3 direction, uint index);

  /**
   * Returns the number of rows in the control point grid
   */
  inline int get_nbRows() { return m_nbRows; }

  /**
   * Returns the number of lines in the control point grid
   */
  inline int get_nbLines() { return m_nbLines; }

  /**
   * Generates the surface
   */
  void genSurface(bool printWarning = false);

  /**
   * Gets the surface render object
   */
  inline std::shared_ptr<RenderObject> get_ro_surface() { return m_RO_surface; }

  /**
   * Gets a vector with all the control point render objects
   */
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
