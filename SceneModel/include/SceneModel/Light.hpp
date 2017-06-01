#pragma once

#include "SceneModel_fwd.hpp"
#include <glm/glm.hpp>

namespace SceneModel
{

class Light
{
public:
    Light(LightId);

    LightId getId() const;

    bool isStatic() const;
    void setStatic(bool);

    glm::vec3 getPosition() const;
    void setPosition(glm::vec3 p);

    float getRange() const;
    void setRange(float r);

    glm::vec3 getIntensity() const;
    void setIntensity(glm::vec3 i);

private:
    LightId id{ 0 };
    glm::vec3 position{ 0, 0, 0 };
    bool is_static{ false };
    glm::vec3 intensity{ 0.8f, 0.8f, 1 };
    float range{ 1 };

};

} // end namespace SceneModel
