#pragma once

#include <string>
#include "Engine/Core/Graphics/Shader/Shader.hpp"


namespace engine {

class VBO {
    private:
     GLuint ID;

    public:
        VBO();
        ~VBO();
        void Bind();
        void Unbind();
        void SetData(const void*, GLsizeiptr, GLenum);

        // Delete Copy
    VBO(const VBO&) = delete;
    VBO& operator=(const VBO&) = delete;

    // Move Constructor
    VBO(VBO&& other) noexcept : ID(other.ID) {
        other.ID = 0;
    }

    // Move Assignment
    VBO& operator=(VBO&& other) noexcept {
        if (this != &other) {
            if (ID) glDeleteBuffers(1, &ID);
            ID = other.ID;
            other.ID = 0;
        }
        return *this;
    }

        
};

class VAO {
    private:
     GLuint ID;
     
    public:
        VAO();
        ~VAO();
        void Bind();
        void Unbind();
        void AddAttribute(const Shader&, const std::string&, GLsizei, std::size_t offset);

        // Delete Copy
    VAO(const VAO&) = delete;
    VAO& operator=(const VAO&) = delete;

    // Move Constructor
    VAO(VAO&& other) noexcept : ID(other.ID) {
        other.ID = 0;
    }

    // Move Assignment
    VAO& operator=(VAO&& other) noexcept {
        if (this != &other) {
            if (ID) glDeleteVertexArrays(1, &ID);
            ID = other.ID;
            other.ID = 0;
        }
        return *this;
    }
};

class EBO {
    private:
    GLuint ID;

    public:
        EBO();
        ~EBO();
        void Bind();
        void Unbind();
        void SetData(const void*, GLsizeiptr, GLenum);

        // Delete Copy
    EBO(const EBO&) = delete;
    EBO& operator=(const EBO&) = delete;

    // Move Constructor
    EBO(EBO&& other) noexcept : ID(other.ID) {
        other.ID = 0;
    }

    // Move Assignment
    EBO& operator=(EBO&& other) noexcept {
        if (this != &other) {
            if (ID) glDeleteBuffers(1, &ID);
            ID = other.ID;
            other.ID = 0;
        }
        return *this;
    }

        
};

}