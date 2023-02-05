#pragma once

#include <GLFW/glfw3.h>
#include <array>

enum KeyState
{
    IDLE        = 0,
    PRESSED     = 1 << 0,
    RELEASED    = 1 << 1,
    HELD        = 1 << 2,
};

class EventHandler
{
public:
    EventHandler();
    EventHandler(const EventHandler&) = delete;

    ~EventHandler() = default;

    int getKeyState(int key);
    void getMouseState(double *x, double *y);
    int getMouseButtonState(int button);

    inline void setWindow(GLFWwindow *window)
    {
        m_window = window;
    }

private:

    std::array<int, 512> m_keyStates;
    std::array<int, 10> m_mouseButtonStates;

    GLFWwindow *m_window;
};

