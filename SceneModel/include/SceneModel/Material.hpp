#pragma once

#include "SceneModel_fwd.hpp"
#include <string>
#include <glm/glm.hpp>

namespace SceneModel
{

class Material
{
public:
    Material(MaterialId i);

    MaterialId getId() const;

    bool isStatic() const { return true; }

	glm::vec3 getAmbientColour() const;
	void setAmbientColour(glm::vec3 c);

    glm::vec3 getDiffuseColour() const;
    void setDiffuseColour(glm::vec3 c);

	const std::string& getDiffuseTexture() const;
	void setDiffuseTexture(const std::string& s);

    glm::vec3 getSpecularColour() const;
    void setSpecularColour(glm::vec3 c);

    float getShininess() const;
    void setShininess(float s);

	const std::string& getSpecularTexture() const;
	void setSpecularTexture(const std::string& s);

    bool isShiny() const;


private:
    MaterialId id_{ 0 };
	glm::vec3 ambient_colour_{ 0.5f, 0.5f, 0.5f };
    glm::vec3 diffuse_colour_{ 0.5f, 0.5f, 0.5f };
    glm::vec3 specular_colour_{ 1.f, 1.f, 1.f };
    float shininess_{ 0.f };
	std::string diffuse_texture_;
	std::string specular_texture_;

};

} // end namespace SceneModel
