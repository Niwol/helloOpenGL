//#pragma once
//
//#include <iostream>
//#include <memory>
//
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <Eigen/Dense>
//#include <OpenMesh/Core/Mesh/TriMeshT.hh>
//
//#include "lib/Camera.hpp"
//#include "lib/renderer.hpp"
//#include "lib/RenderObject.hpp"
//#include "lib/Geometry/operations.hpp"
//
//class ApplicationM2S1 {
//public:
//  ApplicationM2S1();
//  ~ApplicationM2S1();
//
//  bool construct(int width, int height);
//
//  void run();
//
//private:
//  void onCreate();
//  void onUpdate(float dt);
//
//  void processInput(float dt);
//
//private:
//
//  std::shared_ptr<Renderer> m_renderer;
//  Camera m_camera;
//  Scene m_scene;
//
//  GLFWwindow *m_window = nullptr;
//
//
//  bool m_renderFill = true;
//  bool m_cullFaces = true;
//
//  // Mesh
//  std::shared_ptr<RenderObject> m_mainObject;
//  uint32_t m_selectedVertex = 0;
//  uint32_t m_ringLevel = 0;
//
//  // Input stuff
//  int m_lastMouseX = 0;
//  int m_lastMouseY = 0;
//  bool m_mouseLeftHold = false;
//  bool m_keyWHold = false;
//  bool m_keyLHold = false;
//  bool m_keysHold[400] = {false};
//};
