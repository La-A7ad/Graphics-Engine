#include "Engine/Transform.hpp"

namespace engine {

// --------------------------------------------------
// Matrix Multiply (row-major)
// --------------------------------------------------
mat4 mul(const mat4& a, const mat4& b) {
    mat4 r{};
    r.fill(0.0f);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            float sum = 0;
            for (int k = 0; k < 4; k++) {
                sum += a[i * 4 + k] * b[k * 4 + j];
            }
            r[i * 4 + j] = sum;
        }
    }
    return r;
}

// --------------------------------------------------
// Translation Matrix
// --------------------------------------------------
mat4 translate(float x, float y, float z) {
    mat4 m = identity();
    m[12] = x;
    m[13] = y;
    m[14] = z;
    return m;
}

// --------------------------------------------------
// Scale Matrix
// --------------------------------------------------
mat4 scale(float x, float y, float z) {
    mat4 m{};
    m.fill(0.0f);

    m[0]  = x;
    m[5]  = y;
    m[10] = z;
    m[15] = 1.0f;
    return m;
}

// --------------------------------------------------
// Rotation X
// --------------------------------------------------
mat4 rotate_x(float rad) {
    float c = std::cos(rad);
    float s = std::sin(rad);

    mat4 m = identity();
    m[5]  = c;
    m[6]  = s;
    m[9]  = -s;
    m[10] = c;

    return m;
}

// --------------------------------------------------
// Rotation Y
// --------------------------------------------------
mat4 rotate_y(float rad) {
    float c = std::cos(rad);
    float s = std::sin(rad);

    mat4 m = identity();
    m[0]  = c;
    m[2]  = -s;
    m[8]  = s;
    m[10] = c;

    return m;
}

// --------------------------------------------------
// Rotation Z
// --------------------------------------------------
mat4 rotate_z(float rad) {
    float c = std::cos(rad);
    float s = std::sin(rad);

    mat4 m = identity();
    m[0] = c;
    m[1] = s;
    m[4] = -s;
    m[5] = c;

    return m;
}

// --------------------------------------------------
// Perspective Projection (OpenGL-style)
// --------------------------------------------------
mat4 perspective(float fovy_rad, float aspect, float nearp, float farp) {
    float f = 1.0f / std::tan(fovy_rad * 0.5f);

    mat4 m{};
    m.fill(0.0f);

    m[0]  = f / aspect;
    m[5]  = f;
    m[10] = (farp + nearp) / (nearp - farp);
    m[11] = -1.0f;
    m[14] = (2 * farp * nearp) / (nearp - farp);

    return m;
}

// --------------------------------------------------
// LookAt Camera Matrix
// --------------------------------------------------
mat4 lookAt(
    float eyeX, float eyeY, float eyeZ,
    float centerX, float centerY, float centerZ,
    float upX, float upY, float upZ)
{
    float fx = centerX - eyeX;
    float fy = centerY - eyeY;
    float fz = centerZ - eyeZ;

    float rlf = 1.0f / std::sqrt(fx*fx + fy*fy + fz*fz);
    fx *= rlf; fy *= rlf; fz *= rlf;

    float rlu = 1.0f / std::sqrt(upX*upX + upY*upY + upZ*upZ);
    float ux = upX * rlu;
    float uy = upY * rlu;
    float uz = upZ * rlu;

    float sx = fy * uz - fz * uy;
    float sy = fz * ux - fx * uz;
    float sz = fx * uy - fy * ux;

    float rls = 1.0f / std::sqrt(sx*sx + sy*sy + sz*sz);
    sx *= rls; sy *= rls; sz *= rls;

    float ux2 = sy * fz - sz * fy;
    float uy2 = sz * fx - sx * fz;
    float uz2 = sx * fy - sy * fx;

    mat4 m{};
    m.fill(0.0f);

    // Rotation
    m[0] = sx;
    m[1] = ux2;
    m[2] = -fx;

    m[4] = sy;
    m[5] = uy2;
    m[6] = -fy;

    m[8] = sz;
    m[9] = uz2;
    m[10] = -fz;

    // Translation
    m[12] = -(sx * eyeX + sy * eyeY + sz * eyeZ);
    m[13] = -(ux2 * eyeX + uy2 * eyeY + uz2 * eyeZ);
    m[14] =   fx * eyeX + fy * eyeY + fz * eyeZ;

    m[15] = 1.0f;

    return m;
}

} // namespace engine
