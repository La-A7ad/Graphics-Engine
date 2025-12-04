#pragma once

namespace engine {

class VBO {
    private:
        GLuint ID;

    public:
        VBO();
        void Bind();
        void Unbind();
        void Delete();


};

class VAO {
    private:
        GLuint ID;


    public:
        VAO();
        void Bind();
        void Unbind();
        void Delete();





};

}

class EBO {
    private:
        GLuint ID;
    public:
        EBO();
        void Bind();
        void Unbind();
        void Delete();
        

}