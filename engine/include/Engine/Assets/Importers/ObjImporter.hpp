#pragma once

#include "Engine/Rendering/Geometry/Model/Model.hpp"
#include <memory>
#include <string>

namespace engine {

/**
 * OBJ importer: parses .obj files into an engine::Model data structure.
 * This is format-specific parsing (asset pipeline), not rendering logic.
 */
class ObjImporter {
public:
    static std::unique_ptr<Model> Import(const std::string& path);
};

} // namespace engine
