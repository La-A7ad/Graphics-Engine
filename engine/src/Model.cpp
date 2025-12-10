#include "Engine/Model.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <iostream>
#include <unordered_map>

namespace engine {

Model::Model(const std::string& path) {
    loadModel(path);
}

void Model::loadModel(const std::string& path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    directory = path.substr(0, path.find_last_of('/'));

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), directory.c_str())) {
        std::cerr << "TinyObjLoader Error: " << warn << err << "\n";
        return;
    }

    if (!warn.empty()) {
        std::cout << "TinyObjLoader Warning: " << warn << "\n";
    }

    // Process each shape (mesh)
    for (const auto& shape : shapes) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::unordered_map<std::string, unsigned int> uniqueVertices;

        // Process each face
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex{};

            // Position
            vertex.Position = glm::vec3(
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            );

            // Normal
            if (index.normal_index >= 0) {
                vertex.Normal = glm::vec3(
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                );
            } else {
                vertex.Normal = glm::vec3(0.0f, 0.0f, 0.0f);
            }

            // Texture coordinates
            if (index.texcoord_index >= 0) {
                vertex.TexCoords = glm::vec2(
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]
                );
            } else {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }

            // Create unique vertex key for deduplication
            std::string key = std::to_string(index.vertex_index) + "/" + 
                            std::to_string(index.normal_index) + "/" + 
                            std::to_string(index.texcoord_index);

            if (uniqueVertices.count(key) == 0) {
                uniqueVertices[key] = static_cast<unsigned int>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[key]);
        }

        if (!vertices.empty()) {
            meshes.emplace_back(vertices, indices);
        }
    }
}

void Model::Draw(const Shader& shader) {
    for (auto& mesh : meshes) {
        mesh.Draw(shader);
    }
}

} // namespace engine