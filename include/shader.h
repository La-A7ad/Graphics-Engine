#pragma once
#include <string>
#include <glad/glad.h>

class Shader {
public:
    unsigned int ID = 0;
    Shader() = default;
    
    bool loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath);
    void use() const { glUseProgram(ID); }
    void setBool(const std::string &name, bool value) const { glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); }
    void setInt(const std::string &name, int value) const { glUniform1i(glGetUniformLocation(ID, name.c_str()), value); }
    void setFloat(const std::string &name, float value) const { glUniform1f(glGetUniformLocation(ID, name.c_str()), value); }
    
    int getUniformLocation(const std::string &name) const { return glGetUniformLocation(ID, name.c_str()); }
    ~Shader() { if (ID) glDeleteProgram(ID); }
};
