#include "Engine/Core/Math/Transform.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace engine {

mat4 createTransform(const vec3& position, const vec3& rotation, const vec3& scale) {
    mat4 m = glm::mat4(1.0f);
    m = glm::translate(m, position);
    m = glm::rotate(m, rotation.x, vec3(1, 0, 0));
    m = glm::rotate(m, rotation.y, vec3(0, 1, 0));
    m = glm::rotate(m, rotation.z, vec3(0, 0, 1));
    m = glm::scale(m, scale);
    return m;
}

mat4 createPerspective(float fovy_rad, float aspect, float near_plane, float far_plane) {
    return glm::perspective(fovy_rad, aspect, near_plane, far_plane);
}

mat4 createLookAt(const vec3& eye, const vec3& center, const vec3& up) {
    return glm::lookAt(eye, center, up);
}

}