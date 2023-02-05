#include "lib/EventHandler.hpp"
#include <GLFW/glfw3.h>
#include <cstring>

EventHandler::EventHandler()
{
    void *data = m_keyStates.data();
    std::memset(data, 0, m_keyStates.size() * sizeof(int));

    data = m_mouseButtonStates.data();
    std::memset(data, 0, m_mouseButtonStates.size() * sizeof(int));
}

int EventHandler::getKeyState(int key)
{
    if(glfwGetKey(m_window, key) == GLFW_PRESS)
    {
        if(m_keyStates[key] == KeyState::IDLE)
        {
            m_keyStates[key] = KeyState::HELD;
            return KeyState::PRESSED;
        }

        return KeyState::HELD;
    }
    else
    {
        if(m_keyStates[key] == KeyState::HELD)
        {
            m_keyStates[key] = KeyState::IDLE;
            return KeyState::RELEASED;
        }

        return KeyState::IDLE;
    }
}

int EventHandler::getMouseButtonState(int button)
{
    if(glfwGetMouseButton(m_window, button) == GLFW_PRESS)
    {
        if(m_mouseButtonStates[button] == KeyState::IDLE)
        {
            m_mouseButtonStates[button] = KeyState::HELD;
            return KeyState::PRESSED;
        }

        return KeyState::HELD;
    }
    else
    {
        if(m_mouseButtonStates[button] == KeyState::HELD)
        {
            m_mouseButtonStates[button] = KeyState::IDLE;
            return KeyState::RELEASED;
        }

        return KeyState::IDLE;
    }
}

void EventHandler::getMouseState(double *x, double *y)
{
    glfwGetCursorPos(m_window, x, y);
}
