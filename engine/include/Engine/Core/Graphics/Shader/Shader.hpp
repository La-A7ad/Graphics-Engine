#pragma once

#include <glad/glad.h>
#include <string>
#include <unordered_map>

namespace engine {

class Shader {

public:

    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void use() const;

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec4(const std::string& name, const float* value); 



    //functions used by the VAO to link attributes to the vertex attrib pointer


    GLint getAttribLocation(const std::string&) const;


    GLint getUniformLocation(const std::string& name) const;
    void setMat4(const std::string& name, const float* value) const;
    GLuint getID() const { return ID; }

    





    void ReflectAttribs();

    struct ReflectedAttribs {
        const std::string name;
        const GLint location;
        const GLint components;
        const GLenum baseType;

        ReflectedAttribs(const std::string& n,
                    GLint loc,
                    GLint comp,
                    GLenum type)
        : name(n), location(loc), components(comp), baseType(type)
    {}


     };

    const Shader::ReflectedAttribs* getAttrib(const std::string&) const;


    // Delete Copy
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    // Move Constructor
    Shader(Shader&& other) noexcept : ID(other.ID) {
        other.ID = 0;
    }

    // Move Assignment
    Shader& operator=(Shader&& other) noexcept {
        if (this != &other) {
            if (ID) glDeleteProgram(ID);
            ID = other.ID;
            other.ID = 0;
        }
        return *this;
    }




private:

    unsigned int ID;
    static std::string readFile(const char* path);
    static unsigned int compileShader(GLenum type, const std::string& source);


     std::unordered_map<std::string, ReflectedAttribs> m_Attributes;






};

} // namespace engine
