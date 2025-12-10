#pragma once

#include <glm/glm.hpp>

namespace engine {

// Just expose GLM types directly - no need for custom wrappers
using mat4 = glm::mat4;
using vec3 = glm::vec3;
using vec2 = glm::vec2;

// Convenience functions that wrap GLM if needed
mat4 createTransform(const vec3& position, const vec3& rotation, const vec3& scale);
mat4 createPerspective(float fovy_rad, float aspect, float near_plane, float far_plane);
mat4 createLookAt(const vec3& eye, const vec3& center, const vec3& up);

}