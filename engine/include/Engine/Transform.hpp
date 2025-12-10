#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace engine {

// Use GLM mat4 type
using mat4 = glm::mat4;
using vec3 = glm::vec3;
using vec4 = glm::vec4;

// --------------------------------------------------
// Identity Matrix
// --------------------------------------------------
inline mat4 identity() {
    return glm::mat4(1.0f);
}

// --------------------------------------------------
// Matrix Multiply
// out = a * b
// --------------------------------------------------
inline mat4 mul(const mat4& a, const mat4& b) {
    return a * b;
}

// --------------------------------------------------
// Basic Transformations
// --------------------------------------------------
inline mat4 translate(float x, float y, float z) {
    return glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}

inline mat4 translate(const vec3& v) {
    return glm::translate(glm::mat4(1.0f), v);
}

inline mat4 scale(float x, float y, float z) {
    return glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));
}

inline mat4 scale(const vec3& v) {
    return glm::scale(glm::mat4(1.0f), v);
}

inline mat4 rotate_x(float rad) {
    return glm::rotate(glm::mat4(1.0f), rad, glm::vec3(1.0f, 0.0f, 0.0f));
}

inline mat4 rotate_y(float rad) {
    return glm::rotate(glm::mat4(1.0f), rad, glm::vec3(0.0f, 1.0f, 0.0f));
}

inline mat4 rotate_z(float rad) {
    return glm::rotate(glm::mat4(1.0f), rad, glm::vec3(0.0f, 0.0f, 1.0f));
}

// --------------------------------------------------
// Camera / Projection
// --------------------------------------------------
inline mat4 perspective(float fovy_rad, float aspect, float nearp, float farp) {
    return glm::perspective(fovy_rad, aspect, nearp, farp);
}

inline mat4 lookAt(
    float eyeX, float eyeY, float eyeZ,
    float centerX, float centerY, float centerZ,
    float upX, float upY, float upZ)
{
    return glm::lookAt(
        glm::vec3(eyeX, eyeY, eyeZ),
        glm::vec3(centerX, centerY, centerZ),
        glm::vec3(upX, upY, upZ)
    );
}

inline mat4 lookAt(const vec3& eye, const vec3& center, const vec3& up) {
    return glm::lookAt(eye, center, up);
}

} // namespace engine
