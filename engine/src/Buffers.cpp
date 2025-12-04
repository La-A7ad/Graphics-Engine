#include <glad/glad.h>
#include "Engine/Buffers.hpp"

using namespace engine;


VBO::VBO() {
    glGenBuffers(1, &ID);

}

VBO::~VBO() {
    glDeleteBuffers(1, &ID);
}

void VBO::Bind() {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::SetData(const void* data, GLsizeiptr size, GLenum usage) {
      glBindBuffer(GL_ARRAY_BUFFER, ID);
      glBufferData(GL_ARRAY_BUFFER,size,data, usage );
}

VAO::VAO() {
    glGenVertexArrays(1, &ID);
}

VAO::~VAO() {
    glDeleteVertexArrays(1, &ID);
}

void VAO::Bind() {
    glBindVertexArray(ID);
}

void VAO::Unbind() {
    glBindVertexArray(0);
}

EBO::EBO() {
    glGenBuffers(1, &ID);

}

EBO::~EBO() {
    glDeleteBuffers(1, &ID);
}

void EBO::Bind() {

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::Unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::SetData(const void* data, GLsizeiptr size, GLenum usage) {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,size,data,usage);

}
