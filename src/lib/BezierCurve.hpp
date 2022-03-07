#pragma once

#include <vector>

#include "opengl_stuff.h"

class BezierCurve {
public:
  BezierCurve(uint nbPoints = 50);
  ~BezierCurve();

  void addPoint(glm::vec3 position);
  void setPoint(glm::vec3 position, uint index);
  void movePoint(glm::vec3 position, uint index);

  inline int get_nbPoints() { return m_controlPoints.size(); }

  /**
   * This method generates the vertices and indices of the curve and puts them
   * in the given parameters.
   * @param vertices will get the vertices of the curve
   * @param indices will get the indices of the curve
   */
  void genCurve(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);

  /**
   * This method generates the vertices and indices of the control points and
   * puts them in the given parameters.
   * @param vertices will get the vertices of the control points
   * @param indices will get the indices of the control points
   */
  void genControlPoints(std::vector<GLfloat> &vertices,
                        std::vector<GLuint> &indices);

private:
  float N(int index, float u);
  void genBinomialCoeff();

private:
  std::vector<glm::vec3> m_controlPoints;
  std::vector<uint> m_binomialCoeff;
  uint m_nbControlPoints;
  uint m_nbPoints;
};
