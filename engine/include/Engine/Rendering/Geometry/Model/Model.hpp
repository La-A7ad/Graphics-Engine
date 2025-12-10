#pragma once

#include "Engine/Mesh.hpp"
#include "Engine/Shader.hpp"
#include <string>
#include <vector>

namespace engine {

class Model {
public:
    Model(const std::string& path);
    ~Model() = default;
    
    void Draw(const Shader& shader);
    std::vector<Mesh> meshes; //testing it as public

private:
    
    std::string directory;
    
    void loadModel(const std::string& path);
};

} // namespace engine