#pragma once

#include "opengldemo.h"

#include "lib/ShaderProgram.hpp"
#include "lib/camera.hpp"
#include "lib/mesh.hpp"
#include "lib/renderer.hpp"

#include <vector>

class BezierDemo : public OpenGLDemo {
public:
  explicit BezierDemo(int width, int heigth);
  ~BezierDemo() override;

  void draw() override;

private:
  // Shader program
  ShaderProgram m_shaderProgram;
  Renderer m_renderer;
  Camera m_camera;
  Mesh m_mesh;

  // Bezier curve

  // Renderer
};
