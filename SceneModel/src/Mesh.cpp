#include <SceneModel/SceneModel.hpp>

using namespace SceneModel;

Mesh::Mesh(MeshId i) : id(i)
{
}

MeshId Mesh::getId() const
{
    return id;
}

const std::vector<glm::vec3>& Mesh::getPositionArray() const
{
    return position_array;
}

void Mesh::assignPositionArray(std::vector<glm::vec3>&& p)
{
    position_array = p;
}

const std::vector<glm::vec3>& Mesh::getNormalArray() const
{
    return normal_array;
}

void Mesh::assignNormalArray(std::vector<glm::vec3>&& n)
{
    normal_array = n;
}

const std::vector<glm::vec3>& Mesh::getTangentArray() const
{
    return tangent_array;
}

void Mesh::assignTangentArray(std::vector<glm::vec3>&& t)
{
    tangent_array = t;
}

const std::vector<glm::vec2>& Mesh::getTextureCoordinateArray() const
{
    return texcoord_array;
}

void Mesh::assignTextureCoordinateArray(std::vector<glm::vec2>&& t)
{
    texcoord_array = t;
}

const std::vector<unsigned int> Mesh::getElementArray() const
{
    return element_array;
}

void Mesh::assignElementArray(std::vector<unsigned int>&& e)
{
    element_array = e;
}
