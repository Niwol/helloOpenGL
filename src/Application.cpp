#include "Application.hpp"
#include <Eigen/src/Core/Matrix.h>
#include <OpenMesh/Core/Mesh/TriMeshT.hh>

Application::Application() {}


Application::~Application() {
  if(m_window)
    glfwDestroyWindow(m_window);
}

bool Application::construct(int width, int height) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  m_window = glfwCreateWindow(width, height, "Hello OpenGL", NULL, NULL);
  if(m_window == nullptr) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();

    return false;
  }

  glfwMakeContextCurrent(m_window);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return false;
  }

  glViewport(0, 0, width, height);

  glfwSetFramebufferSizeCallback(m_window, 
                                 [](GLFWwindow*, int width, int height){
    glViewport(0, 0, width, height);
  });

  glfwSetWindowUserPointer(m_window, this);

  // OpenGL stuff
  glEnable(GL_DEPTH_TEST);

//  glEnable(GL_CULL_FACE);
//  glCullFace(GL_BACK);
//  glFrontFace(GL_CCW);

  return true;
}

void Application::run() {
  onCreate();

  float currentTime = glfwGetTime();
  float previousTime = 0.0f;

  while(!glfwWindowShouldClose(m_window))
  {
    previousTime = currentTime;
    currentTime = glfwGetTime();
    float dt = currentTime - previousTime;

    onUpdate(dt);
  }
}

void Application::onCreate() {
  m_renderer = std::make_shared<Renderer>();

  m_camera = Camera({0.0f, 0.0f, -5.0f});

   auto sp = std::make_shared<ShaderProgram>("../src/shaders/simpleShader.vs",
                                            "../src/shaders/simpleShader.fs");

   auto normal_sp = 
     std::make_shared<ShaderProgram>("../src/shaders/normalShader.vs",
                                     "../src/shaders/normalShader.fs");

  auto mat = std::make_shared<Material>();
  mat->diffuse = {0.2f, 0.5f, 0.7f};
  mat->useVertexColor = true;

  auto obj = std::make_shared<RenderObject>();
  {
    OpenMesh::IO::Options opt;
    auto &mesh = obj->getMesh()->m_mesh;

    mesh.request_vertex_normals();

    std::vector<std::string> objectPaths;
    objectPaths.push_back("../object_files/armadillo.obj");
    objectPaths.push_back("../object_files/bunnyhead.obj");
    objectPaths.push_back("../object_files/lucy.obj");
    objectPaths.push_back("../object_files/stanford-bunny.obj");
    objectPaths.push_back("../object_files/teapot.obj");

    if(!OpenMesh::IO::read_mesh(mesh, objectPaths[0], opt))
    {
      std::cout << "Error loading mesh" << std::endl;
    }

    if(!opt.check(OpenMesh::IO::Options::VertexNormal))
    {
      mesh.request_face_normals();
      mesh.update_normals();
      mesh.release_face_normals();
    }

    mesh.request_vertex_colors();
    MyMesh::Color color;
    color[0] = 1.0f;
    color[1] = 1.0f;
    color[2] = 0.0f;

    colorVertices(mesh, color);
//    for(auto v_iter = mesh.vertices().begin();
//             v_iter != mesh.vertices().end();
//             v_iter++)
//    {
//      auto p = mesh.point(*v_iter);
//      auto n = mesh.normal(*v_iter);
//
//      mesh.set_point(*v_iter, p + n);
//    }
  }

  obj->getMesh()->commit2();
  
  glm::mat4 t(1.0f);
  t = glm::scale(t, glm::vec3(10.0f));
  obj->transform(t);

  obj->setShaderProgram(sp);
  obj->setMaterial(mat);

  m_mainObject = obj;

  m_scene.objects.push_back(obj);
}

void Application::onUpdate(float dt) {
  glfwPollEvents();
  processInput(dt);

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  int width, height;
  glfwGetWindowSize(m_window, &width, &height);
  m_renderer->setDimension(width, height);
  m_renderer->render(m_scene, m_camera);
  
  glfwSwapBuffers(m_window);
}

void Application::processInput(float dt)
{
  enum KeyState
  {
    NONE = 0,
    RELEASED = 1,
    PRESSED = 2,
    HELD = 4
  };

  auto checkKeyState = [this](int key)
  {
    int keyState = KeyState::NONE;

    if(glfwGetKey(this->m_window, key) == GLFW_PRESS)
      keyState = KeyState::HELD;

    if(this->m_keysHold[key] == false && keyState & KeyState::HELD)
    {
      this->m_keysHold[key] = true;
      keyState |= KeyState::PRESSED;

      return keyState;
    }
    
    if(this->m_keysHold[key] == true && !(keyState & KeyState::HELD))
    {
      this->m_keysHold[key] = false;
      keyState = KeyState::RELEASED;

      return keyState;
    }

    return keyState;
  };

  // **************** Event handleing ****************
  
//  if(glfwGetKey(m_window, GLFW_KEY_ESCAPE))
//    glfwSetWindowShouldClose(m_window, true);
  if(checkKeyState(GLFW_KEY_ESCAPE) & KeyState::PRESSED)
    glfwSetWindowShouldClose(m_window, true);


  // **************** Camera movement ****************

  // Movement
  if(checkKeyState(GLFW_KEY_W) & KeyState::HELD)
    m_camera.move(CameraMovement::FORWARD_XZ, dt);

  if(checkKeyState(GLFW_KEY_S) & KeyState::HELD)
    m_camera.move(CameraMovement::BACKWARD_XZ, dt);
  
  if(checkKeyState(GLFW_KEY_A) & KeyState::HELD)
    m_camera.move(CameraMovement::LEFT, dt);
  
  if(checkKeyState(GLFW_KEY_D) & KeyState::HELD)
    m_camera.move(CameraMovement::RIGHT, dt);
  
  if(checkKeyState(GLFW_KEY_SPACE) & KeyState::HELD)
    m_camera.move(CameraMovement::UP, dt);
  
  if(checkKeyState(GLFW_KEY_LEFT_CONTROL) & KeyState::HELD)
    m_camera.move(CameraMovement::DOWN, dt);

  // Scroll
  glfwSetScrollCallback(m_window, [](GLFWwindow* window, double, double yoff)
  {
    auto app = static_cast<Application*>(glfwGetWindowUserPointer(window));

    app->m_camera.move(CameraMovement::FORWARD, yoff * 0.1f);
  });

  // Rotation
  if(glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
  {
    double x, y;
    glfwGetCursorPos(m_window, &x, &y);

    if(!m_mouseLeftHold)
    {
      m_lastMouseX = (int)x;
      m_lastMouseY = (int)y;
    }

    int dx = (int)x - m_lastMouseX;
    int dy = (int)y - m_lastMouseY;
    m_camera.handleMouseMovement(dx, dy);

    m_lastMouseX = (int)x;
    m_lastMouseY = (int)y;

    m_mouseLeftHold = true;
  } else
    m_mouseLeftHold = false;

  // **************** Draw mode **************** 

  if(checkKeyState(GLFW_KEY_Z) & KeyState::PRESSED)
  {
      m_renderFill = !m_renderFill;
      
      if(m_renderFill)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } 

  // **************** Operations **************** 

  if(checkKeyState(GLFW_KEY_X) & KeyState::PRESSED)
  {
      auto mesh = m_mainObject->getMesh();

      laplacianSmoothing(mesh->m_mesh);
      mesh->commit2();
  }

  if(checkKeyState(GLFW_KEY_C) & KeyState::HELD)
  {
      auto mesh = m_mainObject->getMesh();

      laplacianSmoothing(mesh->m_mesh);
      mesh->commit2();
  } 

  if(checkKeyState(GLFW_KEY_O) & KeyState::PRESSED)
  {
    auto mesh = m_mainObject->getMesh();
    auto vertex = mesh->m_mesh.vertices().to_vector()[m_selectedVertex];
    colorVertexRegion(mesh->m_mesh, vertex, m_ringLevel, [](float dist){
      MyMesh::Color color;
      color[0] = 0.0f;
      color[1] = 0.0f;
      color[2] = 0.0f;

      return color;
    });

    mesh->commit2();
  }


  if(checkKeyState(GLFW_KEY_I) & KeyState::PRESSED)
    m_ringLevel++;

  if(checkKeyState(GLFW_KEY_K) & KeyState::PRESSED && m_ringLevel > 0)
    m_ringLevel--;
}

