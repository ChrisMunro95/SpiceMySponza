#pragma once

#include "SceneModel_fwd.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace SceneModel
{

class Mesh
{
public:
    Mesh(MeshId i);

    MeshId getId() const;

    bool isStatic() const { return true; }

    const std::vector<glm::vec3>& getPositionArray() const;

    const std::vector<glm::vec3>& getNormalArray() const;

    const std::vector<glm::vec3>& getTangentArray() const;

    const std::vector<glm::vec2>& getTextureCoordinateArray() const;

    const std::vector<unsigned int> getElementArray() const;

    void assignPositionArray(std::vector<glm::vec3>&& p);
    void assignNormalArray(std::vector<glm::vec3>&& n);
    void assignTangentArray(std::vector<glm::vec3>&& t);
    void assignTextureCoordinateArray(std::vector<glm::vec2>&& t);
    void assignElementArray(std::vector<unsigned int>&& e);


private:
    MeshId id{ 0 };
    std::vector<glm::vec3> position_array;
    std::vector<glm::vec3> normal_array;
    std::vector<glm::vec3> tangent_array;
    std::vector<glm::vec2> texcoord_array;
    std::vector<unsigned int> element_array;

};

} // end namespace SceneModel
