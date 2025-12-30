#include "Engine/Core/Input/InputManager.hpp"
#include <iostream>

namespace engine {

InputManager& InputManager::Instance() {
    static InputManager instance;
    return instance;
}

void InputManager::Init(GLFWwindow* window) {
    m_Window = window;
    
    // Set user pointer so callbacks can access this instance
    glfwSetWindowUserPointer(window, this);
    
    // Register callbacks
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetScrollCallback(window, ScrollCallback);
    
    // Initialize mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    m_MousePosition = glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
    m_PrevMousePosition = m_MousePosition;
    
    std::cout << "✓ Input system initialized\n";
}

void InputManager::Update() {
    if (!m_Window) return;
    
    // Update previous state for "just pressed/released" detection
    m_PrevKeyState = m_KeyState;
    m_PrevMouseButtonState = m_MouseButtonState;
    
    // Update mouse position and calculate delta
    m_PrevMousePosition = m_MousePosition;
    double xpos, ypos;
    glfwGetCursorPos(m_Window, &xpos, &ypos);
    m_MousePosition = glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
    
    if (m_FirstMouse) {
        m_PrevMousePosition = m_MousePosition;
        m_MouseDelta = glm::vec2(0.0f);
        m_FirstMouse = false;
    } else {
        m_MouseDelta = m_MousePosition - m_PrevMousePosition;
    }
    
    // Reset scroll delta (set by callback)
    m_ScrollDelta = 0.0f;
}

bool InputManager::IsKeyPressed(int key) const {
    auto it = m_KeyState.find(key);
    return it != m_KeyState.end() && it->second;
}

bool InputManager::IsKeyJustPressed(int key) const {
    auto curr = m_KeyState.find(key);
    auto prev = m_PrevKeyState.find(key);
    
    bool currPressed = (curr != m_KeyState.end() && curr->second);
    bool prevPressed = (prev != m_PrevKeyState.end() && prev->second);
    
    return currPressed && !prevPressed;
}

bool InputManager::IsKeyJustReleased(int key) const {
    auto curr = m_KeyState.find(key);
    auto prev = m_PrevKeyState.find(key);
    
    bool currPressed = (curr != m_KeyState.end() && curr->second);
    bool prevPressed = (prev != m_PrevKeyState.end() && prev->second);
    
    return !currPressed && prevPressed;
}

bool InputManager::IsMouseButtonPressed(int button) const {
    auto it = m_MouseButtonState.find(button);
    return it != m_MouseButtonState.end() && it->second;
}

bool InputManager::IsMouseButtonJustPressed(int button) const {
    auto curr = m_MouseButtonState.find(button);
    auto prev = m_PrevMouseButtonState.find(button);
    
    bool currPressed = (curr != m_MouseButtonState.end() && curr->second);
    bool prevPressed = (prev != m_PrevMouseButtonState.end() && prev->second);
    
    return currPressed && !prevPressed;
}

bool InputManager::IsMouseButtonJustReleased(int button) const {
    auto curr = m_MouseButtonState.find(button);
    auto prev = m_PrevMouseButtonState.find(button);
    
    bool currPressed = (curr != m_MouseButtonState.end() && curr->second);
    bool prevPressed = (prev != m_PrevMouseButtonState.end() && prev->second);
    
    return !currPressed && prevPressed;
}

glm::vec2 InputManager::GetMousePosition() const {
    return m_MousePosition;
}

glm::vec2 InputManager::GetMouseDelta() const {
    return m_MouseDelta;
}

float InputManager::GetScrollDelta() const {
    return m_ScrollDelta;
}

void InputManager::SetCursorMode(int mode) {
    if (m_Window) {
        glfwSetInputMode(m_Window, GLFW_CURSOR, mode);
        if (mode == GLFW_CURSOR_DISABLED) {
            m_FirstMouse = true; // Reset to avoid jump
        }
    }
}

bool InputManager::IsCursorDisabled() const {
    if (!m_Window) return false;
    return glfwGetInputMode(m_Window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
}

// Static callbacks
void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    InputManager* input = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (!input) return;
    
    if (action == GLFW_PRESS) {
        input->m_KeyState[key] = true;
    } else if (action == GLFW_RELEASE) {
        input->m_KeyState[key] = false;
    }
}

void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    InputManager* input = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (!input) return;
    
    if (action == GLFW_PRESS) {
        input->m_MouseButtonState[button] = true;
    } else if (action == GLFW_RELEASE) {
        input->m_MouseButtonState[button] = false;
    }
}

void InputManager::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    InputManager* input = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (!input) return;
    
    input->m_ScrollDelta = static_cast<float>(yoffset);
}

} // namespace engine