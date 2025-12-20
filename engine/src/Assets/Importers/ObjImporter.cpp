#include "Engine/Assets/Importers/ObjImporter.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <iostream>
#include <unordered_map>
#include <algorithm>

namespace engine {

static std::string getDirectory(const std::string& path) {
    const auto slash = path.find_last_of("/\\");
    if (slash == std::string::npos) return ".";
    return path.substr(0, slash);
}

static std::string ensureTrailingSlash(std::string dir) {
    if (!dir.empty() && dir.back() != '/' && dir.back() != '\\') dir.push_back('/');
    return dir;
}

std::unique_ptr<Model> ObjImporter::Import(const std::string& path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    const std::string directory = getDirectory(path);
    const std::string baseDir = ensureTrailingSlash(directory);

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), baseDir.c_str())) {
        std::cerr << "TinyObjLoader Error: " << warn << err << "\n";
        return nullptr;
    }

    if (!warn.empty()) {
        std::cout << "TinyObjLoader Warning: " << warn << "\n";
    }

    auto model = std::make_unique<Model>();
    model->directory = directory;
    model->sourcePath = path;

    // Process each shape (mesh)
    for (const auto& shape : shapes) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::unordered_map<std::string, unsigned int> uniqueVertices;

        // Process each face index
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
                vertex.Normal = glm::vec3(0.0f);
            }

            // Texture coordinates
            if (index.texcoord_index >= 0) {
                vertex.TexCoords = glm::vec2(
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]
                );
            } else {
                vertex.TexCoords = glm::vec2(0.0f);
            }

            // Unique vertex key for deduplication (OBJ indices can be separate per attribute)
            std::string key = std::to_string(index.vertex_index) + "/" +
                              std::to_string(index.normal_index) + "/" +
                              std::to_string(index.texcoord_index);

            auto it = uniqueVertices.find(key);
            if (it == uniqueVertices.end()) {
                const unsigned int newIndex = static_cast<unsigned int>(vertices.size());
                uniqueVertices.emplace(key, newIndex);
                vertices.push_back(vertex);
                indices.push_back(newIndex);
            } else {
                indices.push_back(it->second);
            }
        }

        if (!vertices.empty()) {
            model->meshes.emplace_back(std::move(vertices), std::move(indices));
        }
    }

    return model;
}

} // namespace engine
