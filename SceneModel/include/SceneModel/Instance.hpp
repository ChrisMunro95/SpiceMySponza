#pragma once

#include "SceneModel_fwd.hpp"
#include <glm/glm.hpp>

namespace SceneModel
{

class Instance
{
public:
    Instance(InstanceId i);

    InstanceId getId() const;

    bool isStatic() const;

    MeshId getMeshId() const;

    MaterialId getMaterialId() const;

    glm::mat4x3 getTransformationMatrix() const;

    void setStatic(bool b);
    void setMeshId(MeshId id);
    void setMaterialId(MaterialId id);
    void setTransformationMatrix(glm::mat4x3 m);

private:
    InstanceId id{ 0 };
    MeshId mesh_id{ 0 };
    MaterialId material_id{ 0 };
    glm::mat4x3 xform{ 1.f };
    bool is_static{ false };

};

} // end namespace SceneModel
