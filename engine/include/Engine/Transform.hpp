#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma once
#include <array>
#include <cmath>

namespace engine {

using mat4 = std::array<float, 16>;

// --------------------------------------------------
// Identity Matrix
// --------------------------------------------------
inline mat4 identity() {
    mat4 m{};
    m.fill(0.0f);
    m[0] = 1; 
    m[5] = 1; 
    m[10] = 1; 
    m[15] = 1;
    return m;
}

// --------------------------------------------------
// Matrix Multiply (row-major)
// out = a * b
// --------------------------------------------------
mat4 mul(const mat4& a, const mat4& b);

// --------------------------------------------------
// Basic Transformations
// --------------------------------------------------
mat4 translate(float x, float y, float z);
mat4 scale(float x, float y, float z);

mat4 rotate_x(float rad);
mat4 rotate_y(float rad);
mat4 rotate_z(float rad);

// --------------------------------------------------
// Camera / Projection
// --------------------------------------------------
mat4 perspective(float fovy_rad, float aspect, float nearp, float farp);

mat4 lookAt(
    float eyeX, float eyeY, float eyeZ,
    float centerX, float centerY, float centerZ,
    float upX, float upY, float upZ
);

} // namespace engine
