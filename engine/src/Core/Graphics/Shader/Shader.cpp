#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "Engine/Core/Graphics/Shader/Shader.hpp"


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

// FIX: Implement missing setMat4 function
void Shader::setMat4(const std::string& name, const float* value) const {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, value);
}

GLint Shader::getAttribLocation(const std::string& name) const {
    return (glGetAttribLocation(ID, name.c_str()));
}

GLint Shader::getUniformLocation(const std::string& name) const {
    return glGetUniformLocation(ID, name.c_str());
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
        GLint   size      = 0;  // This is array size, not component count!
        GLenum  type      = 0;

        glGetActiveAttrib(
            ID,
            i,
            maxNameLen,
            &written,
            &size,
            &type,
            nameBuf.data()
        );

       
        std::string name(nameBuf.data(), written);
        GLint location = getAttribLocation(name);

        if (location < 0) {
            continue;
        }

        // Now we need to determine the component count and base type from the type enum
        GLint components = 0;
        GLenum baseType = 0;

        // Determine components and base type from the type enum
        switch (type) {
            // Float types
            case GL_FLOAT:
                components = 1;
                baseType = GL_FLOAT;
                break;
            case GL_FLOAT_VEC2:
                components = 2;
                baseType = GL_FLOAT;
                break;
            case GL_FLOAT_VEC3:
                components = 3;
                baseType = GL_FLOAT;
                break;
            case GL_FLOAT_VEC4:
                components = 4;
                baseType = GL_FLOAT;
                break;
            
            // Int types
            case GL_INT:
                components = 1;
                baseType = GL_INT;
                break;
            case GL_INT_VEC2:
                components = 2;
                baseType = GL_INT;
                break;
            case GL_INT_VEC3:
                components = 3;
                baseType = GL_INT;
                break;
            case GL_INT_VEC4:
                components = 4;
                baseType = GL_INT;
                break;
            
            // Unsigned int types
            case GL_UNSIGNED_INT:
                components = 1;
                baseType = GL_UNSIGNED_INT;
                break;
            case GL_UNSIGNED_INT_VEC2:
                components = 2;
                baseType = GL_UNSIGNED_INT;
                break;
            case GL_UNSIGNED_INT_VEC3:
                components = 3;
                baseType = GL_UNSIGNED_INT;
                break;
            case GL_UNSIGNED_INT_VEC4:
                components = 4;
                baseType = GL_UNSIGNED_INT;
                break;
            
            // Matrix types (treated as multiple vec4s, but we'll store the column count)
            case GL_FLOAT_MAT2:
                components = 4;  // 2x2 = 4 floats
                baseType = GL_FLOAT;
                break;
            case GL_FLOAT_MAT3:
                components = 9;  // 3x3 = 9 floats
                baseType = GL_FLOAT;
                break;
            case GL_FLOAT_MAT4:
                components = 16; // 4x4 = 16 floats
                baseType = GL_FLOAT;
                break;
            
            default:
                // Unknown or unsupported type, default to float
                std::cerr << "Warning: Unknown attribute type " << type << " for " << name << "\n";
                components = 1;
                baseType = GL_FLOAT;
                break;
        }

        m_Attributes.emplace(
            name,
            ReflectedAttribs(name, location, components, baseType)
        );
    }



}

Shader::~Shader() {
    if (ID != 0) {
        glDeleteProgram(ID);
        ID = 0;
    }
}


 

} // namespace engine