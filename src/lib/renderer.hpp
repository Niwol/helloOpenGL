#pragma once

#include "Light/PointLight.hpp"
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

  void addPointLight(std::shared_ptr<PointLight> pointLight,
                     bool printWarning = false);

  void removePointLight(std::shared_ptr<PointLight> pointLight,
                        bool printWarning = false);

  /**
   * Sets the camera of the renderer
   */
  void setCamera(std::shared_ptr<Camera> cam);

  /**
   * Gieves the dimensions of the window to the renderer
   */
  void setDimension(int width, int height);

  /**
   * Sets up all shaders and draw all render objects attached to the renderer
   */
  void draw();

private:
  std::map<uint, std::shared_ptr<RenderObject>> m_renderObjects;
  std::map<uint, std::shared_ptr<PointLight>> m_pointLights;
  std::shared_ptr<Camera> m_camera;

  int m_width;
  int m_height;

  uint m_nextID;
};
