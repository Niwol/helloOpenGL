#pragma once

#include "opengldemo.h"

#include "lib/Geometry/BezierCurve.hpp"
#include "lib/Geometry/BezierSurface.hpp"
#include "lib/Geometry/mesh.hpp"
#include "lib/Light/PointLight.hpp"
#include "lib/RenderObject.hpp"
#include "lib/ShaderProgram.hpp"
#include "lib/camera.hpp"
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
  std::shared_ptr<ShaderProgram> m_shaderProgram;
  std::shared_ptr<ShaderProgram> m_normalShaderProgram;
  std::shared_ptr<ShaderProgram> m_uvShaderProgram;
  std::shared_ptr<ShaderProgram> m_textureShaderProgram;
  std::shared_ptr<ShaderProgram> m_simpleLightShaderProgram;
  std::shared_ptr<ShaderProgram> m_brdfShaderProgram;
  std::shared_ptr<Renderer> m_renderer;

  // Bezier Geometry
  std::shared_ptr<BezierCurve> m_bezierCurve;
  std::shared_ptr<BezierSurface> m_bezierSurface;
  int m_selectedPoint;
  int m_selectedObject;

  // Materials
  std::shared_ptr<Material> m_colorMaterial;
  std::shared_ptr<Material> m_textureMaterial;

  // Lights
  std::shared_ptr<PointLight> m_light;

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
