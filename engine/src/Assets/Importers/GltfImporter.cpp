#include "Engine/Assets/Importers/GltfImporter.hpp"

#define CGLTF_IMPLEMENTATION
#include "cgltf.h"

#include <iostream>

namespace engine {

static std::string getDirectory(const std::string& path) {
    const auto slash = path.find_last_of("/\\");
    if (slash == std::string::npos) return ".";
    return path.substr(0, slash);
}

static cgltf_accessor* findAttr(const cgltf_primitive& prim, cgltf_attribute_type type, int index = 0) {
    for (cgltf_size i = 0; i < prim.attributes_count; ++i) {
        const cgltf_attribute& a = prim.attributes[i];
        if (a.type == type && (int)a.index == index) return a.data;
    }
    return nullptr;
}

std::unique_ptr<Model> GltfImporter::Import(const std::string& path) {
    cgltf_options options{};
    cgltf_data* data = nullptr;

    cgltf_result res = cgltf_parse_file(&options, path.c_str(), &data);
    if (res != cgltf_result_success || !data) {
        std::cerr << "cgltf: failed to parse glTF file: " << path << "\n";
        return nullptr;
    }

    res = cgltf_load_buffers(&options, data, path.c_str());
    if (res != cgltf_result_success) {
        std::cerr << "cgltf: failed to load buffers for: " << path << "\n";
        cgltf_free(data);
        return nullptr;
    }

    // Validation is helpful, but not strictly required
    cgltf_validate(data);

    auto model = std::make_unique<Model>();
    model->directory = getDirectory(path);
    model->sourcePath = path;

    for (cgltf_size mi = 0; mi < data->meshes_count; ++mi) {
        const cgltf_mesh& mesh = data->meshes[mi];

        for (cgltf_size pi = 0; pi < mesh.primitives_count; ++pi) {
            const cgltf_primitive& prim = mesh.primitives[pi];
            if (prim.type != cgltf_primitive_type_triangles) continue;

            cgltf_accessor* posAcc = findAttr(prim, cgltf_attribute_type_position);
            if (!posAcc) continue;

            cgltf_accessor* nrmAcc = findAttr(prim, cgltf_attribute_type_normal);
            cgltf_accessor* uvAcc  = findAttr(prim, cgltf_attribute_type_texcoord, 0);

            std::vector<Vertex> vertices;
            vertices.resize(posAcc->count);

            for (cgltf_size v = 0; v < posAcc->count; ++v) {
                Vertex vert{};

                float p[3] = {0,0,0};
                cgltf_accessor_read_float(posAcc, v, p, 3);
                vert.Position = glm::vec3(p[0], p[1], p[2]);

                if (nrmAcc) {
                    float n[3] = {0,0,0};
                    cgltf_accessor_read_float(nrmAcc, v, n, 3);
                    vert.Normal = glm::vec3(n[0], n[1], n[2]);
                } else {
                    vert.Normal = glm::vec3(0.0f);
                }

                if (uvAcc) {
                    float uv[2] = {0,0};
                    cgltf_accessor_read_float(uvAcc, v, uv, 2);

                    // NOTE:
                    // Your Texture loader flips image data vertically (stbi_set_flip_vertically_on_load(true)).
                    // glTF texture coordinates are typically authored with a top-left image origin in mind,
                    // so we flip V here to compensate and match your global image-flip policy.
                    vert.TexCoords = glm::vec2(uv[0], 1.0f - uv[1]);
                } else {
                    vert.TexCoords = glm::vec2(0.0f);
                }

                vertices[v] = vert;
            }

            std::vector<unsigned int> indices;
            if (prim.indices) {
                indices.resize(prim.indices->count);
                for (cgltf_size i = 0; i < prim.indices->count; ++i) {
                    indices[i] = (unsigned int)cgltf_accessor_read_index(prim.indices, i);
                }
            } else {
                // Non-indexed primitive: emit 0..N-1
                indices.resize(vertices.size());
                for (size_t i = 0; i < vertices.size(); ++i) indices[i] = (unsigned int)i;
            }

            if (!vertices.empty()) {
                model->meshes.emplace_back(std::move(vertices), std::move(indices));
            }
        }
    }

    cgltf_free(data);

    if (model->meshes.empty()) {
        std::cerr << "GltfImporter: loaded glTF but found no triangle meshes: " << path << "\n";
    }

    return model;
}

} // namespace engine
