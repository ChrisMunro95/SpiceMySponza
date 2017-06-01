#include <SceneModel/SceneModel.hpp>

using namespace SceneModel;

Camera::Camera()
{
    position = glm::vec3(0, 0, 0);
    direction = glm::vec3(0, 0, -1);
    vertical_field_of_view_degrees = 60;
    near_plane_distance = 1;
    far_plane_distance = 10000;
    translation_speed = glm::vec3(0, 0, 0);
    rotation_speed = glm::vec2(0, 0);
}

glm::vec3 Camera::getPosition() const
{
    return position;
}

void Camera::setPosition(glm::vec3 p)
{
    position = p;
}

glm::vec3 Camera::getDirection() const
{
    return direction;
}

void Camera::setDirection(glm::vec3 d)
{
    direction = d;
}

float Camera::getVerticalFieldOfViewInDegrees() const
{
    return vertical_field_of_view_degrees;
}

void Camera::setVerticalFieldOfViewInDegrees(float d)
{
    vertical_field_of_view_degrees = d;
}

float Camera::getNearPlaneDistance() const
{
    return near_plane_distance;
}

void Camera::setNearPlaneDistance(float n)
{
    near_plane_distance = n;
}

float Camera::getFarPlaneDistance() const
{
    return far_plane_distance;
}

void Camera::setFarPlaneDistance(float f)
{
    far_plane_distance = f;
}

glm::vec3 Camera::getLinearVelocity() const
{
    return translation_speed;
}

void Camera::setLinearVelocity(glm::vec3 v)
{
    translation_speed = v;
}

glm::vec2 Camera::getRotationalVelocity() const
{
    return rotation_speed;
}

void Camera::setRotationalVelocity(glm::vec2 v)
{
    rotation_speed = v;
}
