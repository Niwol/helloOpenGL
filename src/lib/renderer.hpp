#pragma once

#include "RenderObject.hpp"
#include "ShaderProgram.hpp"
#include "camera.hpp"
#include "opengl_stuff.h"

#include <memory>
#include <vector>

class Renderer {
public:
  Renderer(int width = 500, int height = 500);
  ~Renderer();

  void addRenderObject(RenderObject *ro);
  void setShaderProgram(ShaderProgram *sp);
  void setCamera(std::shared_ptr<Camera> cam);
  void setDimension(int width, int height);

  void draw();

private:
  std::vector<RenderObject *> m_renderObjects;
  ShaderProgram *m_shaderProgram;
  std::shared_ptr<Camera> m_camera;

  int m_width;
  int m_height;
};
