#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

static std::string readFileToString(const std::string& path) {
    std::ifstream file(path, std::ios::in);
    if (!file.is_open()) {
        return std::string();
    }
    std::ostringstream contents;
    contents << file.rdbuf();
    file.close();
    return contents.str();
}

static void printShaderLog(GLuint shader) {
    GLint len = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 1) {
        std::string log; log.resize(len);
        glGetShaderInfoLog(shader, len, nullptr, &log[0]);
        std::cerr << log << "\n";
    }
}

static void printProgramLog(GLuint prog) {
    GLint len = 0;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
    if (len > 1) {
        std::string log; log.resize(len);
        glGetProgramInfoLog(prog, len, nullptr, &log[0]);
        std::cerr << log << "\n";
    }
}

bool Shader::loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertCode = readFileToString(vertexPath);
    std::string fragCode = readFileToString(fragmentPath);
    if (vertCode.empty()) {
        std::cerr << "Failed to read vertex shader file: " << vertexPath << "\n";
        return false;
    }
    if (fragCode.empty()) {
        std::cerr << "Failed to read fragment shader file: " << fragmentPath << "\n";
        return false;
    }

    const char* vShaderCode = vertCode.c_str();
    const char* fShaderCode = fragCode.c_str();

    GLuint vertex, fragment;
    GLint success;

    
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        std::cerr << "ERROR: Vertex shader compilation failed.\n";
        printShaderLog(vertex);
        glDeleteShader(vertex);
        return false;
    }

    
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        std::cerr << "ERROR: Fragment shader compilation failed.\n";
        printShaderLog(fragment);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return false;
    }

    
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        std::cerr << "ERROR: Shader program linking failed.\n";
        printProgramLog(ID);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        glDeleteProgram(ID);
        ID = 0;
        return false;
    }

    
    glDetachShader(ID, vertex);
    glDetachShader(ID, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return true;
}
