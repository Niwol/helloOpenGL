#include "Application.hpp"
#include "lib/RenderObject.hpp"
#include "lib/ShaderProgram.hpp"
#include "lib/renderer.hpp"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <memory>

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

  auto obj = std::make_shared<RenderObject>();
  obj->getMesh()->to_cube();
  obj->setShaderProgram(sp);

  m_scene.objects.push_back(obj);
}

void Application::onUpdate(float dt) {
  glfwPollEvents();
  processInput(dt);

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);


  int width, height;
  glfwGetWindowSize(m_window, &width, &height);
  m_renderer->setDimension(width, height);
  m_renderer->render(m_scene, m_camera);
  
  glfwSwapBuffers(m_window);
}

void Application::processInput(float dt)
{
  if(glfwGetKey(m_window, GLFW_KEY_ESCAPE))
    glfwSetWindowShouldClose(m_window, true);

//  if(glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
//  {
//    m_renderFill = !m_renderFill;
//    
//    if(m_renderFill)
//      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//    else
//      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//  }

  if(glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
    m_camera.move(CameraMovement::FORWARD, dt);

  if(glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
    m_camera.move(CameraMovement::BACKWARD, dt);
  
  if(glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
    m_camera.move(CameraMovement::LEFT, dt);
  
  if(glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
    m_camera.move(CameraMovement::RIGHT, dt);
  
  if(glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
    m_camera.move(CameraMovement::UP, dt);
  
  if(glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    m_camera.move(CameraMovement::DOWN, dt);

}

