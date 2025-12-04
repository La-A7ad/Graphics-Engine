#pragma once

namespace engine {

class VBO {
    private:
     GLuint ID;

    public:
        VBO();
        ~VBO();
        void Bind();
        void Unbind();
        void Delete();
        void SetData(const void*, GLsizeiptr, GLenum);


};

class VAO {
    private:
     GLuint ID;

    public:
        VAO();
        ~VAO();
        void Bind();
        void Unbind();
        void Delete();





};

class EBO {
    private:
    GLuint ID;

    public:
        EBO();
        ~EBO();
        void Bind();
        void Unbind();
        void Delete();
        void SetData();
        void SetData(const void*, GLsizeiptr, GLenum);
        

};



}

