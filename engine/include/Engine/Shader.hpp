#pragma once

#include <glad/glad.h>
#include <string>

namespace engine {

class Shader {
public:
    Shader(const char* vertexPath, const char* fragmentPath);

    void use() const;

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

private:
    unsigned int ID;
    static std::string readFile(const char* path);
    static unsigned int compileShader(GLenum type, const std::string& source);
};

} // namespace engine
