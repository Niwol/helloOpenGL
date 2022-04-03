#include "hellobezier.h"

BezierDemo::BezierDemo(int width, int height)
    : OpenGLDemo(width, height), m_selectedPoint{0}, m_selectedObject{0} {

  m_renderer = std::make_shared<Renderer>(width, height);
  // **********************************************************
  // ********************  Shader program  ********************
  m_shaderProgram = std::make_shared<ShaderProgram>();
  m_normalShaderProgram = std::make_shared<ShaderProgram>();
  m_uvShaderProgram = std::make_shared<ShaderProgram>();
  m_textureShaderProgram = std::make_shared<ShaderProgram>();
  m_simpleLightShaderProgram = std::make_shared<ShaderProgram>();
  m_brdfShaderProgram = std::make_shared<ShaderProgram>();

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

  if (!m_uvShaderProgram->createProgram("../src/shaders/uvShader.vs",
                                        "../src/shaders/uvShader.fs")) {
    std::cout << "Abord" << std::endl;
    exit(1);
  }

  if (!m_textureShaderProgram->createProgram(
          "../src/shaders/textureShader.vs",
          "../src/shaders/textureShader.fs")) {
    std::cout << "Abord" << std::endl;
    exit(1);
  }

  if (!m_simpleLightShaderProgram->createProgram(
          "../src/shaders/simpleLightShader.vs",
          "../src/shaders/simpleLightShader.fs")) {
    std::cout << "Abord" << std::endl;
    exit(1);
  }

  if (!m_brdfShaderProgram->createProgram("../src/shaders/brdfShader.vs",
                                          "../src/shaders/brdfShader.fs")) {
    std::cout << "Abord" << std::endl;
    exit(1);
  }

  // ********************  Shader program  ********************
  // **********************************************************

  // ****************************************************
  // ********************  Material  ********************
  m_colorMaterial = std::make_shared<Material>();
  m_textureMaterial = std::make_shared<Material>();

  // Color material
  m_colorMaterial->diffuse = glm::vec3(0.7f, 0.3f, 0.5f);
  m_colorMaterial->specular = glm::vec3(0.5f);
  m_colorMaterial->shininess = 32;
  m_colorMaterial->isTextureMaterial = false;

  // Texture Material
  m_textureMaterial->isTextureMaterial = true;
  m_textureMaterial->diffuseTexture =
      std::make_shared<Texture>("diffuseTexture");
  if (!m_textureMaterial->diffuseTexture->load("../textures/container2.png")) {
    m_textureMaterial->diffuseTexture.reset();
    m_textureMaterial->isTextureMaterial = false;
  }

  m_textureMaterial->specularTexture =
      std::make_shared<Texture>("specularTexture");
  if (!m_textureMaterial->specularTexture->load(
          "../textures/container2_specular.png")) {
    m_textureMaterial->specularTexture.reset();
    m_textureMaterial->isTextureMaterial = false;
  }
  m_textureMaterial->shininess = 32;
  // ********************  Material  ********************
  // ****************************************************

  // ********************************************************
  // ********************  Bezier curve  ********************
  m_bezierCurve = std::make_shared<BezierCurve>();

  m_bezierCurve->addPoint(glm::vec3(-1.0f, -0.5f, 0.0f));
  m_bezierCurve->addPoint(glm::vec3(-0.25f, 1.0f, 0.0f));
  m_bezierCurve->addPoint(glm::vec3(0.25f, -0.5f, 0.0f));
  m_bezierCurve->addPoint(glm::vec3(1.0f, 1.0f, 0.0f));

  m_renderer->addRenderObject(m_bezierCurve->get_ro_curve());
  m_renderer->addRenderObject(m_bezierCurve->get_ro_controlPolynom());

  m_bezierCurve->get_ro_curve()->setShaderProgram(m_shaderProgram);
  m_bezierCurve->get_ro_controlPolynom()->setShaderProgram(m_shaderProgram);

  glm::vec3 translation = glm::vec3(-2.0f, 0.0f, -5.0f);
  m_bezierCurve->transform(glm::translate(glm::mat4(1.0f), translation));
  // ********************  Bezier curve  ********************
  // ********************************************************

  // **********************************************************
  // ********************  Bezier surface  ********************
  m_bezierSurface = std::make_shared<BezierSurface>(50, 40);

  m_bezierSurface->addLine({
      glm::vec3(-1.0f, -2.0f, 1.0f),
      glm::vec3(-0.3f, 2.0f, 1.0f),
      glm::vec3(0.3f, 2.0f, 1.0f),
      glm::vec3(1.0f, -2.0f, 1.0f),
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

  for (auto &ro : m_bezierSurface->get_ro_controlPoints()) {
    ro->setShaderProgram(m_shaderProgram);
    m_renderer->addRenderObject(ro);
  }

  m_bezierSurface->get_ro_surface()->setMaterial(m_colorMaterial);
  m_bezierSurface->get_ro_surface()->setShaderProgram(m_shaderProgram);
  m_renderer->addRenderObject(m_bezierSurface->get_ro_surface());
  // ********************  Bezier surface  ********************
  // **********************************************************

  // **************************************************
  // ********************  Lights  ********************
  m_light = std::make_shared<PointLight>();
  m_light->setPosition(glm::vec3(-0.7f, 1.0f, -0.5f));

  m_light->getRenderObject()->setShaderProgram(m_shaderProgram);
  m_light->addShaderProgram(m_simpleLightShaderProgram);
  m_light->addShaderProgram(m_textureShaderProgram);
  m_light->addShaderProgram(m_brdfShaderProgram);

  m_renderer->addRenderObject(m_light->getRenderObject());
  m_renderer->addPointLight(m_light);
  // ********************  Lights  ********************
  // **************************************************

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
    m_light->move(glm::vec3(-0.1f, 0.0f, 0.0f));
    m_bezierCurve->movePoint(glm::vec3(-0.1f, 0.0f, 0.0f), m_selectedPoint);
    return true;
  case 'd':
    m_light->move(glm::vec3(0.1f, 0.0f, 0.0f));
    m_bezierCurve->movePoint(glm::vec3(0.1f, 0.0f, 0.0f), m_selectedPoint);
    return true;
  case 'z':
    m_light->move(glm::vec3(0.0f, 0.0f, -0.1f));
    m_bezierCurve->movePoint(glm::vec3(0.0f, 0.1f, 0.0f), m_selectedPoint);
    return true;
  case 's':
    m_light->move(glm::vec3(0.0f, 0.0f, 0.1f));
    m_bezierCurve->movePoint(glm::vec3(0.0f, -0.1f, 0.0f), m_selectedPoint);
    return true;

  case 'n':
    m_bezierSurface->get_ro_surface()->setShaderProgram(m_normalShaderProgram);
    return true;
  case 'b':
    m_bezierSurface->get_ro_surface()->setShaderProgram(m_uvShaderProgram);
    return true;
  case 'v':
    m_bezierSurface->get_ro_surface()->setMaterial(m_textureMaterial);
    m_bezierSurface->get_ro_surface()->setShaderProgram(m_textureShaderProgram);
    return true;
  case 'c':
    m_bezierSurface->get_ro_surface()->setMaterial(m_colorMaterial);
    m_bezierSurface->get_ro_surface()->setShaderProgram(
        m_simpleLightShaderProgram);
    return true;
  case 'm':
    m_bezierSurface->get_ro_surface()->setMaterial(m_colorMaterial);
    m_bezierSurface->get_ro_surface()->setShaderProgram(m_brdfShaderProgram);
    return true;
  case 'x':
    m_bezierSurface->get_ro_surface()->setShaderProgram(m_shaderProgram);
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