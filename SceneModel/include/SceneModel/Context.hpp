#pragma once

#include "SceneModel_fwd.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <chrono>
#include <memory>

namespace SceneModel
{

class Context
{
public:

    Context();

    ~Context();

    void update();

    bool toggleCameraAnimation();

    float getTimeInSeconds() const;

    glm::vec3 getUpDirection() const;

    glm::vec3 getAmbientLightIntensity() const;

    const Camera& getCamera() const;

    Camera& getCamera();

    const std::vector<Light>& getAllLights() const;

    const std::vector<Material>& getAllMaterials() const;

    const Material& getMaterialById(MaterialId id) const;

    const std::vector<Instance>& getAllInstances() const;

    const Instance& getInstanceById(InstanceId id) const;

    const std::vector<InstanceId> getInstancesByMeshId(MeshId id) const;

private:

    bool readFile(std::string filepath);

    std::chrono::system_clock::time_point start_time_;
	float time_seconds_{ 0 };

    std::shared_ptr<FirstPersonMovement> camera_movement_;
    Camera camera_;
	bool animate_camera_{ false };

    std::vector<Light> lights_;

    std::vector<Material> materials_;

    std::vector<Instance> instances_;

    std::vector<std::vector<InstanceId>> instances_by_mesh_;

};

} // end namespace SceneModel
