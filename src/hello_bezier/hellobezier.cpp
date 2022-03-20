#include "hellobezier.h"

BezierDemo::BezierDemo(int width, int height)
    : OpenGLDemo(width, height), m_selectedPoint{0}, m_selectedObject{0} {

  m_renderer = std::make_shared<Renderer>(width, height);

  // ********************************************************
  // ********************  Bezier curve  ********************
  m_bezierCurve = std::make_shared<BezierCurve>();

  m_bezierCurve->addPoint(glm::vec3(-1.0f, -0.5f, 0.0f));
  m_bezierCurve->addPoint(glm::vec3(-0.25f, 1.0f, 0.0f));
  m_bezierCurve->addPoint(glm::vec3(0.25f, -0.5f, 0.0f));
  m_bezierCurve->addPoint(glm::vec3(1.0f, 1.0f, 0.0f));

  m_renderer->addRenderObject(m_bezierCurve->get_ro_curve());
  m_renderer->addRenderObject(m_bezierCurve->get_ro_controlPolynom());

  glm::vec3 translation = glm::vec3(-2.0f, 0.0f, -5.0f);
  m_bezierCurve->transform(glm::translate(glm::mat4(1.0f), translation));
  // ********************  Bezier curve  ********************
  // ********************************************************

  // **********************************************************
  // ********************  Bezier surface  ********************
  m_bezierSurface = std::make_shared<BezierSurface>(20, 50);

  m_bezierSurface->addLine({
      glm::vec3(-1.0f, -0.5f, 1.0f),
      glm::vec3(-0.3f, 0.5f, 1.0f),
      glm::vec3(0.3f, 0.5f, 1.0f),
      glm::vec3(1.0f, -0.5f, 1.0f),
  });
  m_bezierSurface->addLine({
      glm::vec3(-1.0f, -0.2f, 0.3f),
      glm::vec3(-0.3f, 0.2f, 0.3f),
      glm::vec3(0.3f, 0.2f, 0.3f),
      glm::vec3(1.0f, -0.2f, 0.3f),
  });
  m_bezierSurface->addLine({
      glm::vec3(-1.0f, 0.2f, -0.3f),
      glm::vec3(-0.3f, -0.2f, -0.3f),
      glm::vec3(0.3f, -0.2f, -0.3f),
      glm::vec3(1.0f, 0.2f, -0.3f),
  });
  m_bezierSurface->addLine({
      glm::vec3(-1.0f, 0.5f, -1.0f),
      glm::vec3(-0.3f, -0.5f, -1.0f),
      glm::vec3(0.3f, -0.5f, -1.0f),
      glm::vec3(1.0f, 0.5f, -1.0f),
  });

  for (auto &ro : m_bezierSurface->get_ro_controlPoints())
    m_renderer->addRenderObject(ro);

  m_renderer->addRenderObject(m_bezierSurface->get_ro_surface());

  // auto cube = std::make_shared<RenderObject>();
  // cube->getMesh()->to_cube();
  // m_renderer->addRenderObject(cube);

  std::vector<GLfloat> vertices = {
      -0.5f, 0.0f, -0.5f,

      -0.5f, 0.0f, 0.5f,

      0.5f,  0.0f, -0.5f,

      0.0f,  0.0f, 0.0f,
  };

  glm::vec3 p1, p2, p3;
  p1.x = vertices[0];
  p1.y = vertices[1];
  p1.z = vertices[2];
  p2.x = vertices[3];
  p2.y = vertices[4];
  p2.z = vertices[5];
  p3.x = vertices[6];
  p3.y = vertices[7];
  p3.z = vertices[8];

  glm::vec3 v1, v2;
  v1 = p3 - p1;
  v2 = p2 - p1;
  glm::vec3 normal = glm::cross(v2, v1);
  normal = glm::normalize(normal);

  vertices[9] = p1.x + normal.x;
  vertices[10] = p1.y + normal.y;
  vertices[11] = p1.z + normal.z;

  auto ro = std::make_shared<RenderObject>();
  ro->getMesh()->set_vertices(vertices);
  ro->getMesh()->set_indices({0, 1, 2, 0, 1, 3, 1, 2, 3, 1, 3, 2},
                             GL_TRIANGLES);

  ro->transform(glm::translate(glm::mat4(1.0f), glm::vec3(5.f, 0.0f, 0.0f)));

  m_renderer->addRenderObject(ro);

  // ********************  Bezier surface  ********************
  // **********************************************************

  // **********************************************************
  // ********************  Shader program  ********************
  m_shaderProgram = std::make_shared<ShaderProgram>();
  m_normalShaderProgram = std::make_shared<ShaderProgram>();

  if (!m_shaderProgram->createProgram("../src/shaders/simpleShader.vs",
                                      "../src/shaders/simpleShader.fs")) {
    std::cout << "Abord" << std::endl;
    exit(1);
  }

  if (!m_normalShaderProgram->createProgram("../src/shaders/normalShader.vs",
                                            "../src/shaders/normalShader.fs")) {
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

  // glEnable(GL_CULL_FACE);
  // glCullFace(GL_BACK);
  // glFrontFace(GL_CCW);

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
  int nbPoints = m_bezierCurve->get_nbControlPoints();

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
    return true;
  case 'd':
    m_bezierCurve->movePoint(glm::vec3(0.1f, 0.0f, 0.0f), m_selectedPoint);
    return true;
  case 'z':
    m_bezierCurve->movePoint(glm::vec3(0.0f, 0.1f, 0.0f), m_selectedPoint);
    return true;
  case 's':
    m_bezierCurve->movePoint(glm::vec3(0.0f, -0.1f, 0.0f), m_selectedPoint);
    return true;

  case 'n':
    m_renderer->setShaderProgram(m_normalShaderProgram);
    // draw();
    return true;
  case 'b':
    m_renderer->setShaderProgram(m_shaderProgram);
    // draw();
    return true;

  default:
    return false;
  }
}

void BezierDemo::update() {}

void BezierDemo::draw() {
  OpenGLDemo::draw();

  m_renderer->draw();
}