#pragma once

#include "opengldemo.h"

#include "lib/BezierCurve.hpp"
#include "lib/RenderObject.hpp"
#include "lib/ShaderProgram.hpp"
#include "lib/camera.hpp"
#include "lib/mesh.hpp"
#include "lib/renderer.hpp"

#include <memory>
#include <vector>

class BezierDemo : public OpenGLDemo {
public:
  explicit BezierDemo(int width, int height);
  ~BezierDemo() override;

  virtual void resize(int width, int height) override;

  // User input
  virtual void mouseclick(int button, float xpos, float ypos) override;
  virtual void mousemove(float xpos, float ypos) override;
  virtual void mousewheel(float delta) override;
  virtual void keyboardmove(int key, double time) override;
  virtual bool keyboard(unsigned char k) override;

  void update();

  void draw() override;

private:
  ShaderProgram m_shaderProgram;
  Renderer m_renderer;
  RenderObject m_renderObject;
  RenderObject m_ro_controlPoints;

  // Bezier curve
  BezierCurve m_bezierCurve;
  int m_selectedPoint;

  // Camera
  using CameraSelector = std::function<Camera *()>;
  std::vector<CameraSelector> m_cameraSelector;
  uint m_activeCamera;
  std::shared_ptr<Camera> m_camera;

  // Mouse
  int m_mouseButton;
  float m_mouseX;
  float m_mouseY;
};
