#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <glad/glad.h>
#include "Engine/Shader.hpp"


namespace engine {

std::string Shader::readFile(const char* path) {
    std::ifstream file(path, std::ios::in);
    if (!file) {
        std::cerr << "Shader has not been read successfully " << path << "\n";
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

unsigned int Shader::compileShader(GLenum type, const std::string& source) {
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "COMPILATION FAILED\n" << infoLog << "\n";
    }

    return shader;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode   = readFile(vertexPath);
    std::string fragmentCode = readFile(fragmentPath);

    unsigned int vertex   = compileShader(GL_VERTEX_SHADER,   vertexCode);
    unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    int success;
    char infoLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM_LINKING_FAILED\n" << infoLog << "\n";
    }

    ReflectAttribs();

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() const {
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

GLint Shader::getAttribLocation(const std::string& name) const {
    return (glGetAttribLocation(ID, name.c_str()));
}



const Shader::ReflectedAttribs* Shader::getAttrib(const std::string& name) const {
    auto it = m_Attributes.find(name);

    if (it == m_Attributes.end()) {
        return nullptr;
    }

    return &it->second; 
}


void Shader::ReflectAttribs() {
    GLint count = 0;
    glGetProgramiv(ID, GL_ACTIVE_ATTRIBUTES, &count);

    GLint maxNameLen = 0;
    glGetProgramiv(ID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxNameLen);

    std::vector<char> nameBuf(maxNameLen);
    m_Attributes.clear();


    for (GLint i = 0; i < count; ++i) {
        GLsizei written   = 0;
        GLint   components = 0;
        GLenum  type       = 0;

        glGetActiveAttrib(
            ID,
            i,
            maxNameLen,
            &written,
            &components,
            &type,
            nameBuf.data()
        );

       
        std::string name(nameBuf.data(), written);
        GLint location = getAttribLocation(name);

        if (location < 0) {
            continue;
        }

        GLenum baseType = 0;

        // float-based attributes (float, vec2, vec3, vec4)
        if (type == GL_FLOAT      ||
            type == GL_FLOAT_VEC2 ||
            type == GL_FLOAT_VEC3 ||
            type == GL_FLOAT_VEC4)
        {
            baseType = GL_FLOAT;
        }
        // int-based attributes (int, ivec2, ivec3, ivec4)
        else if (type == GL_INT      ||
                 type == GL_INT_VEC2 ||
                 type == GL_INT_VEC3 ||
                 type == GL_INT_VEC4)
        {
            baseType = GL_INT;
        }
        // unsigned-int-based attributes (uint, uvec2, uvec3, uvec4)
        else if (type == GL_UNSIGNED_INT      ||
                 type == GL_UNSIGNED_INT_VEC2 ||
                 type == GL_UNSIGNED_INT_VEC3 ||
                 type == GL_UNSIGNED_INT_VEC4)
        {
            baseType = GL_UNSIGNED_INT;
        }
        else {
            // For now, treat everything else as floatish so it doesn't just die.
            // You can refine this later for doubles, bools, etc.
            baseType = GL_FLOAT;
        }
        m_Attributes.emplace(
            name,
            ReflectedAttribs(name, location, components, baseType)
        );
    }
}

 

} // namespace engine
