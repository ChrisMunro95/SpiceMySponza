#include <SceneModel/SceneModel.hpp>
#include <tcf/SimpleScene.hpp>

using namespace SceneModel;

/******************************************************************************
*
*
* STOP!
*
* You shouldn't be reading this source file.
* You don't need to know about the implementation of the GeometryBuilder.
* Do not base any of your code on how this is implemented,
* to do so would break the concept of encapsulation.
*
*
*****************************************************************************/

GeometryBuilder::GeometryBuilder()
{
    if (!readFile("sponza.tcf")) {
        throw std::runtime_error("Failed to read sponza.tcf data file");
    }
}

GeometryBuilder::~GeometryBuilder()
{

}

const std::vector<Mesh>& GeometryBuilder::getAllMeshes() const
{
    return meshes_;
}

const Mesh& GeometryBuilder::getMeshById(MeshId id) const
{
    return meshes_[id - 300];
}

bool GeometryBuilder::readFile(std::string filepath)
{
    tcf::Error error;
    tcf::SimpleScene tcf_scene = tcf::simpleSceneFromFile(filepath, &error);
    if (error != tcf::kNoError) {
        return false;
    }

    meshes_.clear();

    meshes_.reserve(tcf_scene.meshArray.size());
    for (const auto& mesh : tcf_scene.meshArray) {
        Mesh new_mesh(300 + meshes_.size());
        new_mesh.assignElementArray(std::vector<unsigned int>(
            (unsigned int*)&mesh.indexArray.front(),
            (unsigned int*)&mesh.indexArray.back() + 1));
        new_mesh.assignNormalArray(std::vector<glm::vec3>(
            (glm::vec3*)&mesh.normalArray.front(),
            (glm::vec3*)&mesh.normalArray.back() + 1));
        new_mesh.assignPositionArray(std::vector<glm::vec3>(
            (glm::vec3*)&mesh.vertexArray.front(),
            (glm::vec3*)&mesh.vertexArray.back() + 1));
        if (!mesh.tangentArray.empty()) {
            new_mesh.assignTangentArray(std::vector<glm::vec3>(
                (glm::vec3*)&mesh.tangentArray.front(),
                (glm::vec3*)&mesh.tangentArray.back() + 1));
        }
        if (!mesh.texcoordArray.empty()) {
            new_mesh.assignTextureCoordinateArray(std::vector<glm::vec2>(
                (glm::vec2*)&mesh.texcoordArray.front(),
                (glm::vec2*)&mesh.texcoordArray.back() + 1));
        }
        meshes_.push_back(new_mesh);
    }

    return true;
}
