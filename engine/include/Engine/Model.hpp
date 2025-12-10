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

private:
    std::vector<Mesh> meshes;
    std::string directory;
    
    void loadModel(const std::string& path);
};

} // namespace engine