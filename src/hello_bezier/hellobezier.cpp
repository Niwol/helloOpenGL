#include "hellobezier.h"

BezierDemo::BezierDemo(int width, int height)
    : OpenGLDemo(width, height), m_selectedPoint{0} {

  m_renderer = std::make_shared<Renderer>(width, height);

  // ********************************************************
  // ********************  Bezier curve  ********************
  m_bezierCurve = std::make_shared<BezierCurve>();

  m_bezierCurve->addPoint(glm::vec3(-1.0f, -0.5f, 0.0f));
  m_bezierCurve->addPoint(glm::vec3(-0.25f, 1.0f, 0.0f));
  m_bezierCurve->addPoint(glm::vec3(0.25f, -0.5f, 0.0f));
  m_bezierCurve->addPoint(glm::vec3(1.0f, 1.0f, 0.0f));

  m_bezierCurve->genCurve();
  m_renderer->addRenderObject(m_bezierCurve->get_ro_curve());

  m_bezierCurve->genControlPoints();
  m_renderer->addRenderObject(m_bezierCurve->get_ro_controlPoints());

  glm::vec3 translation = glm::vec3(-2.0f, 0.0f, -5.0f);
  m_bezierCurve->transform(glm::translate(glm::mat4(1.0f), translation));
  // ********************  Bezier curve  ********************
  // ********************************************************

  // **********************************************************
  // ********************  Shader program  ********************
  m_shaderProgram = std::make_shared<ShaderProgram>();

  if (!m_shaderProgram->createProgram("../src/shaders/simpleShader.vs",
                                      "../src/shaders/simpleShader.fs")) {
    std::cout << "Abord" << std::endl;
    exit(1);
  }
  m_renderer->setShaderProgram(m_shaderProgram);
  // ********************  Shader program  ********************
  // **********************************************************

  // **************************************************
  // ********************  Camera  ********************
  m_cameraSelector.push_back(
      []() -> Camera * { return new EulerCamera(glm::vec3(0.f, 0.f, 3.f)); });
  m_cameraSelector.push_back([]() -> Camera * {
    return new TrackballCamera(glm::vec3(0.f, 0.f, 3.f));
  });

  m_activeCamera = 0;
  m_camera.reset(m_cameraSelector[m_activeCamera]());

  m_camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));

  m_renderer->setCamera(m_camera);
  // ********************  Camera  ********************
  // **************************************************

  draw();
}

BezierDemo::~BezierDemo() {}

void BezierDemo::resize(int width, int height) {
  OpenGLDemo::resize(width, height);
}
void BezierDemo::mouseclick(int button, float xpos, float ypos) {
  m_mouseButton = button;
  m_mouseX = xpos;
  m_mouseY = ypos;
  m_camera->processmouseclick(m_mouseButton, xpos, ypos);
}

void BezierDemo::mousemove(float xpos, float ypos) {
  m_camera->processmousemovement(m_mouseButton, xpos, ypos, true);
}

void BezierDemo::mousewheel(float delta) {
  m_camera->processmousescroll(delta);
}

void BezierDemo::keyboardmove(int key, double time) {
  m_camera->processkeyboard(Camera_Movement(key), time);
}

bool BezierDemo::keyboard(unsigned char k) {
  int nbPoints = m_bezierCurve->get_nbPoints();

  switch (k) {
  case 'p':
    m_activeCamera = (m_activeCamera + 1) % 2;
    m_camera.reset(m_cameraSelector[m_activeCamera]());
    m_camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));
    m_renderer->setCamera(m_camera);
    return true;

  case 'a':
    m_selectedPoint = (m_selectedPoint - 1 + nbPoints) % nbPoints;
    return true;
  case 'e':
    m_selectedPoint = (m_selectedPoint + 1) % nbPoints;
    return true;
  case 'q':
    m_bezierCurve->movePoint(glm::vec3(-0.1f, 0.0f, 0.0f), m_selectedPoint);
    update();
    return true;
  case 'd':
    m_bezierCurve->movePoint(glm::vec3(0.1f, 0.0f, 0.0f), m_selectedPoint);
    update();
    return true;
  case 'z':
    m_bezierCurve->movePoint(glm::vec3(0.0f, 0.1f, 0.0f), m_selectedPoint);
    update();
    return true;
  case 's':
    m_bezierCurve->movePoint(glm::vec3(0.0f, -0.1f, 0.0f), m_selectedPoint);
    update();
    return true;

  default:
    return false;
  }
}

void BezierDemo::update() {
  m_bezierCurve->genCurve();
  m_bezierCurve->genControlPoints();
}

void BezierDemo::draw() {
  OpenGLDemo::draw();

  m_renderer->draw();
}