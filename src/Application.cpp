#include "Application.hpp"
#include <GLFW/glfw3.h>


Application::Application() {}

Application::~Application()
{
    if(m_window)
        glfwDestroyWindow(m_window);
}


bool Application::construct(std::unique_ptr<Runable> runable, const Application::Options& options)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(options.width, options.height, "Hello OpenGL",
                                NULL, NULL);
    if(m_window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return false;
    }

    glfwMakeContextCurrent(m_window);
    m_appUtils.window = m_window;

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    glViewport(0, 0, options.width, options.height);

    glfwSetFramebufferSizeCallback(m_window, 
                                   [](GLFWwindow*, int width, int height)
    {
        glViewport(0, 0, width, height);
    });

    // glfwSetWindowUserPointer(m_window, this);

    // OpenGL stuff
    glEnable(GL_DEPTH_TEST);

    //  glEnable(GL_CULL_FACE);
    //  glCullFace(GL_BACK);
    //  glFrontFace(GL_CCW);
    
    // Runable
    m_runable = std::move(runable);

    return true;
}

void Application::run()
{
    if(m_runable->onCreate() == false)
        return;

    float currentTime = glfwGetTime();

    while(m_runable->onUpdate(m_appUtils) && !glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();

        float previusTime = currentTime;
        currentTime = glfwGetTime();

        m_appUtils.delatTime = currentTime - previusTime;
    }
}
