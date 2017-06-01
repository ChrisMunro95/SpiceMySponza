#include <SceneModel/SceneModel.hpp>

using namespace SceneModel;

Light::Light(LightId i) : id(i)
{
}

LightId Light::getId() const
{
    return id;
}

bool Light::isStatic() const
{
    return is_static;
}

void Light::setStatic(bool b)
{
    is_static = b;
}

glm::vec3 Light::getPosition() const
{
    return position;
}

void Light::setPosition(glm::vec3 p)
{
    position = p;
}

float Light::getRange() const
{
    return range;
}

void Light::setRange(float r)
{
    range = r;
}

glm::vec3 Light::getIntensity() const
{
    return intensity;
}

void Light::setIntensity(glm::vec3 i)
{
    intensity = i;
}
