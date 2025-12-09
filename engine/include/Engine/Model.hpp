#pragma once
#include "model/mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>


namespace engine {

class Model {
public:
    std::vector<Mesh> meshes;
    std::string directory;

    Model(const std::string& path) { loadModel(path); }
    void Draw(unsigned int shaderProgram) const;

private:
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};

}