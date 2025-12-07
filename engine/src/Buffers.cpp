#include <glad/glad.h>
#include "Engine/Buffers.hpp"
#include "Engine/Shader.hpp"

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

void VAO::AddAttribute(const Shader& shader,
                       const std::string& attribName,
                       GLsizei stride,
                       std::size_t offset)
{
    const Shader::ReflectedAttribs* attr = shader.getAttrib(attribName);
    if (!attr) {
        // attribute not found in shader
        return;
    }

    glEnableVertexAttribArray(attr->location);

    // Float-based attributes
    if (attr->baseType == GL_FLOAT ||
        attr->baseType == GL_HALF_FLOAT ||
        attr->baseType == GL_DOUBLE)
    {
        glVertexAttribPointer(
            attr->location,
            attr->components,
            attr->baseType,
            GL_FALSE,
            stride,
            reinterpret_cast<const void*>(offset)
        );
    }
    // Integer-based attributes
    else if (attr->baseType == GL_INT ||
             attr->baseType == GL_UNSIGNED_INT ||
             attr->baseType == GL_SHORT ||
             attr->baseType == GL_UNSIGNED_SHORT ||
             attr->baseType == GL_BYTE ||
             attr->baseType == GL_UNSIGNED_BYTE)
    {
        glVertexAttribIPointer(
            attr->location,
            attr->components,
            attr->baseType,
            stride,
            reinterpret_cast<const void*>(offset)
        );
    }
    else {
        // unsupported type for now
    }
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
