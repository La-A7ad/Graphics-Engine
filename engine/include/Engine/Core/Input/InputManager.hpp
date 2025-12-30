#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <unordered_map>

namespace engine {

/**
 * InputManager - Centralized input handling system
 * 
 * Singleton pattern for global access from any system/component
 * Tracks keyboard, mouse buttons, and mouse position/delta
 */
class InputManager {
public:
    // Singleton access
    static InputManager& Instance();
    
    // Initialize with GLFW window
    void Init(GLFWwindow* window);
    
    // Call once per frame to update input state
    void Update();
    
    // Keyboard input
    bool IsKeyPressed(int key) const;
    bool IsKeyJustPressed(int key) const;
    bool IsKeyJustReleased(int key) const;
    
    // Mouse buttons
    bool IsMouseButtonPressed(int button) const;
    bool IsMouseButtonJustPressed(int button) const;
    bool IsMouseButtonJustReleased(int button) const;
    
    // Mouse position and movement
    glm::vec2 GetMousePosition() const;
    glm::vec2 GetMouseDelta() const;
    float GetScrollDelta() const;
    
    // Mouse cursor control
    void SetCursorMode(int mode); // GLFW_CURSOR_NORMAL, GLFW_CURSOR_HIDDEN, GLFW_CURSOR_DISABLED
    bool IsCursorDisabled() const;
    
private:
    InputManager() = default;
    ~InputManager() = default;
    
    // Delete copy constructor and assignment
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;
    
    GLFWwindow* m_Window = nullptr;
    
    // Keyboard state tracking
    std::unordered_map<int, bool> m_KeyState;
    std::unordered_map<int, bool> m_PrevKeyState;
    
    // Mouse button state tracking
    std::unordered_map<int, bool> m_MouseButtonState;
    std::unordered_map<int, bool> m_PrevMouseButtonState;
    
    // Mouse position tracking
    glm::vec2 m_MousePosition{0.0f, 0.0f};
    glm::vec2 m_PrevMousePosition{0.0f, 0.0f};
    glm::vec2 m_MouseDelta{0.0f, 0.0f};
    
    // Scroll tracking
    float m_ScrollDelta = 0.0f;
    
    // GLFW callbacks
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    
    // First mouse movement flag
    bool m_FirstMouse = true;
};

} // namespace engine