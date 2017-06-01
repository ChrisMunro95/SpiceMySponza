#include <SceneModel/SceneModel.hpp>

using namespace SceneModel;

Material::Material(MaterialId i) : id_(i)
{
}

MaterialId Material::getId() const
{
    return id_;
}

glm::vec3 Material::getAmbientColour() const
{
	return ambient_colour_;
}

void Material::setAmbientColour(glm::vec3 c)
{
	ambient_colour_ = c;
}

glm::vec3 Material::getDiffuseColour() const
{
    return diffuse_colour_;
}

void Material::setDiffuseColour(glm::vec3 c)
{
    diffuse_colour_ = c;
}

const std::string& Material::getDiffuseTexture() const
{
	return diffuse_texture_;
}

void Material::setDiffuseTexture(const std::string& s)
{
	diffuse_texture_ = s;
}

glm::vec3 Material::getSpecularColour() const
{
    return specular_colour_;
}

void Material::setSpecularColour(glm::vec3 c)
{
    specular_colour_ = c;
}

float Material::getShininess() const
{
    return shininess_;
}

void Material::setShininess(float s)
{
    shininess_ = s;
}

const std::string& Material::getSpecularTexture() const
{
	return specular_texture_;
}

void Material::setSpecularTexture(const std::string& s)
{
	specular_texture_ = s;
}

bool Material::isShiny() const
{
    return shininess_ > 0;
}
