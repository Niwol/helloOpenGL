#pragma once

#include "RenderObject.hpp"
#include "ShaderProgram.hpp"
#include "camera.hpp"
#include "opengl_stuff.h"

#include <map>
#include <memory>
#include <vector>

class Renderer {
public:
  Renderer(int width = 500, int height = 500);
  ~Renderer();

  void addRenderObject(std::shared_ptr<RenderObject> ro,
                       bool printWarning = false);
  void removeRenderObject(std::shared_ptr<RenderObject> ro,
                          bool printWarning = false);

  void setShaderProgram(std::shared_ptr<ShaderProgram> sp);
  void setCamera(std::shared_ptr<Camera> cam);
  void setDimension(int width, int height);

  void draw();

private:
  std::map<uint, std::shared_ptr<RenderObject>> m_renderObjects;
  std::shared_ptr<ShaderProgram> m_shaderProgram;
  std::shared_ptr<Camera> m_camera;

  int m_width;
  int m_height;

  uint m_nextID;
};
