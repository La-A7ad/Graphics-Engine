#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <cstdint>
#include <vector>

namespace engine {

class Color {
public:
    std::uint8_t r, g, b, a;

    //uint8_t is an unsigned 8-bit integer, stores values from 0-255

    Color(std::uint8_t r = 255,
          std::uint8_t g = 255,
          std::uint8_t b = 255,
          std::uint8_t a = 255)
        : r(r), g(g), b(b), a(a) {}
};

inline const Color White{255, 255, 255, 255};
inline const Color Black{0, 0, 0, 255};

class Vertex {
public:
    glm::vec3 position{0.0f};
    glm::vec3 normal{0.0f};
    glm::vec2 texCoord{0.0f};
    Color color{White};

    Vertex() = default; //default is white

    Vertex(const glm::vec3& pos,
           const glm::vec3& n,
           const glm::vec2& uv,
           const Color& col = White)
        : position(pos), normal(n), texCoord(uv), color(col) {}
};

class Mesh {
public:
    Mesh();
    ~Mesh();

    // Non-copyable (GPU resource owner)
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    // Movable
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    // Create GPU buffers from CPU-side vertex/index data
    void create(const std::vector<Vertex>& vertices,
                const std::vector<std::uint16_t>& indices);

    // Free GPU resources (called automatically in ~Mesh)
    void destroy();

    // Draw the mesh using currently bound shader / material
    void draw() const;

    // Convenience factory helpers
    static Mesh createCube(const glm::vec3& center  = glm::vec3(0.0f),
                           const glm::vec3& size    = glm::vec3(1.0f));

    static Mesh createPlane(const glm::vec3& center = glm::vec3(0.0f),
                            const glm::vec2& size   = glm::vec2(1.0f),
                            const glm::vec2& tiling = glm::vec2(1.0f));

    static Mesh createSphere(const glm::ivec2& segments = {32, 16},
                             const glm::vec3& center    = glm::vec3(0.0f),
                             float radius               = 1.0f);

    // Load from .obj using tinyobjloader (implementation in .cpp)
    static Mesh loadObj(const char* path);

    GLsizei indexCount() const { return m_indexCount; }

private:
    GLuint  m_vao;
    GLuint  m_vbo;
    GLuint  m_ebo;
    GLsizei m_indexCount;

    void setupVertexAttributes(); // internal VAO layout configuration
};

} // namespace engine
