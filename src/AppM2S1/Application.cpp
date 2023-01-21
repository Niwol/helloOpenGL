#include "Application.hpp"
#include <Eigen/src/Core/Matrix.h>
#include <OpenMesh/Core/Mesh/TriMeshT.hh>

ApplicationM2S1::ApplicationM2S1() {}


ApplicationM2S1::~ApplicationM2S1() {
  if(m_window)
    glfwDestroyWindow(m_window);
}

bool ApplicationM2S1::construct(int width, int height) {
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

void ApplicationM2S1::run() {
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

void ApplicationM2S1::onCreate() {
  m_renderer = std::make_shared<Renderer>();

  m_camera = Camera({0.0f, 0.0f, -5.0f});

  auto sp = std::make_shared<ShaderProgram>("../src/shaders/simpleShader.vs",
                                            "../src/shaders/simpleShader.fs");

  auto normal_sp = 
    std::make_shared<ShaderProgram>("../src/shaders/normalShader.vs",
                                    "../src/shaders/normalShader.fs");

  // RenderObject
  auto obj = std::make_shared<RenderObject>();

  // Mesh loading
  OpenMesh::IO::Options opt;
  auto &mesh = obj->getMesh()->m_mesh;

  mesh.request_vertex_normals();

  std::vector<std::string> objectPaths;
  objectPaths.push_back("../object_files/armadillo.obj");
  objectPaths.push_back("../object_files/bunnyhead.obj");
  objectPaths.push_back("../object_files/lucy.obj");
  objectPaths.push_back("../object_files/stanford-bunny.obj");
  objectPaths.push_back("../object_files/teapot.obj");
  objectPaths.push_back("../object_files/woody.obj");
  objectPaths.push_back("../object_files/spot.obj");

  if(!OpenMesh::IO::read_mesh(mesh, objectPaths[6], opt))
  {
    std::cout << "Error loading mesh" << std::endl;
  }

  if(!opt.check(OpenMesh::IO::Options::VertexNormal))
  {
    mesh.request_face_normals();
    mesh.update_normals();
    mesh.release_face_normals();
  }

  // Mesh color
  mesh.request_vertex_colors();
  MyColor color = {0.0f, 0.0f, 0.0f};

  colorVertices(mesh, color);
  obj->getMesh()->commit2();
  
  // Mesh scale
  glm::mat4 t(1.0f);
  t = glm::scale(t, glm::vec3(30.0f));
  obj->transform(t);

  // Shader program
  obj->setShaderProgram(sp);

  // Material
  auto mat = std::make_shared<Material>();
  mat->useVertexColor = true;
  obj->setMaterial(mat);

  m_mainObject = obj;

  m_scene.objects.push_back(obj);
}

void ApplicationM2S1::onUpdate(float dt) {
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

void ApplicationM2S1::processInput(float dt)
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
    auto app = static_cast<ApplicationM2S1*>(glfwGetWindowUserPointer(window));

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


  // **************** Laplacian smoothing **************** 

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


  // **************** Vertex coloring **************** 
  
  auto blackFunc = [](MyMesh& mesh, OpenMesh::SmartVertexHandle vertex, float)
  {
    MyMesh::Color color;
    color[0] = 0;
    color[1] = 0;
    color[2] = 0;

    mesh.set_color(vertex, color);
  };

  auto grayScaleFunc = [](MyMesh& mesh, OpenMesh::SmartVertexHandle vertex, float x)
  {
    MyColor color = {1.0f, 0.0f, 0.0f};
    color *= x;

    MyMesh::Color res = color_floatToUint(color);

    mesh.set_color(vertex, res);
  };

  bool redraw = false;
  uint32_t previousRingLevel = m_ringLevel;
  uint32_t previousVertex = m_selectedVertex;
  auto mesh = m_mainObject->getMesh();

  if(checkKeyState(GLFW_KEY_O) & KeyState::PRESSED)
  {
    if(m_selectedVertex < mesh->m_mesh.n_vertices() - 1)
    {
      previousVertex = m_selectedVertex;
      m_selectedVertex++;
      redraw = true;
    }
  }

  if(checkKeyState(GLFW_KEY_L) & KeyState::PRESSED)
  {
    if(m_selectedVertex > 0)
    {
      previousVertex = m_selectedVertex;
      m_selectedVertex--;
      redraw = true;
    }
  }
  
  if(checkKeyState(GLFW_KEY_P) & KeyState::HELD)
  {
    if(m_selectedVertex < mesh->m_mesh.n_vertices() - 1)
    {
      previousVertex = m_selectedVertex;
      m_selectedVertex++;
      redraw = true;
    }
  }

  if(checkKeyState(GLFW_KEY_SEMICOLON) & KeyState::HELD)
  {
    if(m_selectedVertex > 0)
    {
      previousVertex = m_selectedVertex;
      m_selectedVertex--;
      redraw = true;
    }
  }

  if(checkKeyState(GLFW_KEY_I) & KeyState::PRESSED)
  {
    previousRingLevel = m_ringLevel;
    m_ringLevel++;
    redraw = true;
  }

  if(checkKeyState(GLFW_KEY_K) & KeyState::PRESSED && m_ringLevel > 0)
  {
    if(m_ringLevel > 0)
    {
      previousRingLevel = m_ringLevel;
      m_ringLevel--;
      redraw = true;
    }
  }

  if(redraw)
  {
    auto vertices = mesh->m_mesh.vertices().to_vector();

    auto vertex = vertices[previousVertex];
    operationOnVertexRegion(mesh->m_mesh, vertex, previousRingLevel, blackFunc);

    vertex = vertices[m_selectedVertex];
    operationOnVertexRegion(mesh->m_mesh, vertex, m_ringLevel, grayScaleFunc);

    mesh->commit2();
  }


  // **************** Vertex movement **************** 

  Eigen::Vector3f movement = {0.0f, 0.0f, 0.0f};
  float movementValue = 0.01f;
  bool moved = false;
  auto moveFunc = [&movement](MyMesh& mesh, OpenMesh::SmartVertexHandle vertex, float weight)
  {
    MyMesh::Point moveTo;
    moveTo[0] = movement.x() * weight;
    moveTo[1] = movement.y() * weight;
    moveTo[2] = movement.z() * weight;

    mesh.set_point(vertex, mesh.point(vertex) + moveTo);
  };

  if(checkKeyState(GLFW_KEY_T) & KeyState::PRESSED)
  {
    movement.z() += movementValue;
    moved = true;
  }
  
  if(checkKeyState(GLFW_KEY_G) & KeyState::PRESSED)
  {
    movement.z() -= movementValue;
    moved = true;
  }

  if(checkKeyState(GLFW_KEY_F) & KeyState::PRESSED)
  {
    movement.x() += movementValue;
    moved = true;
  }
  
  if(checkKeyState(GLFW_KEY_H) & KeyState::PRESSED)
  {
    movement.x() -= movementValue;
    moved = true;
  }

  if(checkKeyState(GLFW_KEY_R) & KeyState::PRESSED)
  {
    movement.y() -= movementValue;
    moved = true;
  }

  if(checkKeyState(GLFW_KEY_Y) & KeyState::PRESSED)
  {
    movement.y() += movementValue;
    moved = true;
  }

  if(moved)
  {
    auto vertex = mesh->m_mesh.vertices().to_vector()[m_selectedVertex];
    operationOnVertexRegion(mesh->m_mesh, vertex, m_ringLevel, moveFunc);
    mesh->commit2();
  }
}

