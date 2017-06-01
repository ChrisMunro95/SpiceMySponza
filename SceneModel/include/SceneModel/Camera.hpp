#pragma once

#include "SceneModel_fwd.hpp"
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <chrono>
#include <memory>

namespace SceneModel
{

class Camera
{
public:
    Camera();

    bool isStatic() const { return false; }

    glm::vec3 getPosition() const;

    glm::vec3 getDirection() const;

    float getVerticalFieldOfViewInDegrees() const;

    float getNearPlaneDistance() const;

    float getFarPlaneDistance() const;

    glm::vec3 getLinearVelocity() const;

    glm::vec2 getRotationalVelocity() const;

    void setPosition(glm::vec3 p);
    void setDirection(glm::vec3 d);
    void setVerticalFieldOfViewInDegrees(float d);
    void setNearPlaneDistance(float n);
    void setFarPlaneDistance(float f);
    void setLinearVelocity(glm::vec3 v);
    void setRotationalVelocity(glm::vec2 v);

private:
    glm::vec3 position;
    glm::vec3 direction;
    float vertical_field_of_view_degrees;
    float near_plane_distance;
    float far_plane_distance;
    glm::vec3 translation_speed;
    glm::vec2 rotation_speed;

};

} // end namespace SceneModel
