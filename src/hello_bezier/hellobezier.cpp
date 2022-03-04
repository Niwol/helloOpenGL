#include "hellobezier.h"

BezierDemo::BezierDemo(int width, int heigth) : OpenGLDemo(width, heigth) {

  m_mesh.to_square();
  m_renderer.addMesh(&m_mesh);

  if (!m_shaderProgram.createProgram("../src/shaders/simpleShader.vs",
                                     "../src/shaders/simpleShader.fs")) {
    std::cout << "Abord" << std::endl;
    exit(1);
  }

  m_renderer.setShaderProgram(&m_shaderProgram);
}

BezierDemo::~BezierDemo() {}

void BezierDemo::draw() {
  OpenGLDemo::draw();

  m_renderer.draw();
}