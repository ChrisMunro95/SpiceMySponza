#pragma once

#include "SceneModel_fwd.hpp"
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace SceneModel
{

class GeometryBuilder
{
public:

    GeometryBuilder();

    ~GeometryBuilder();

    const std::vector<Mesh>& getAllMeshes() const;

    const Mesh& getMeshById(MeshId id) const;

private:

    bool readFile(std::string filepath);

    std::vector<Mesh> meshes_;

};

} // end namespace SceneModel
