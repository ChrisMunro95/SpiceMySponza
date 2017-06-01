#version 330

uniform vec3 Camera_Position;

uniform vec3 Light_Position[22];
uniform vec3 Light_Intensity[22];
uniform float Light_Range[22];

uniform vec3 diffuse_material_colour;
uniform vec3 ambient_material_colour;
uniform vec3 specular_colour;
uniform float shininess;

uniform sampler2D diff_tex_sample;

uniform sampler2D spec_tex_sample;


uniform bool useDiffTexture;
uniform bool useSpecTexture;
uniform bool toggle_normal;

in vec3 colour_normals;
in vec3 P;
in vec3 N;
in vec2 texcoords;

out vec4 fragment_colour;

vec3 Lights[22];

vec3 newLight(vec3 lightPos, vec3 vertPos, float lightRange, vec3 light_intensity);

void main(void)
{
	for (int i = 0; i < 22; i++){
		Lights[i] = newLight(Light_Position[i], P, Light_Range[i], Light_Intensity[i]);
	}

	vec3 allLights= vec3(Lights[0] + Lights[1] + Lights[2] + Lights[3] + Lights[4] + Lights[5] +
						  Lights[6] + Lights[7] + Lights[8] + Lights[9] + Lights[10] + Lights[11] + Lights[12] + 
						  Lights[13] + Lights[14] + Lights[15] + Lights[16] + Lights[17] + Lights[19] + Lights[20] + 
						  Lights[21]);

	vec3 diff_texture = texture(diff_tex_sample, texcoords).rgb;

	vec3 spec_texture = texture(spec_tex_sample, texcoords).rgb;

	vec3 textures = diff_texture * spec_texture;

	if(toggle_normal == true){
		fragment_colour = vec4(colour_normals, 1.0);
	}else{
		if(useDiffTexture == true && useSpecTexture == false){		
			fragment_colour = vec4(allLights * diff_texture, 1.0);
		}
		else if (useSpecTexture == true && useDiffTexture == false){
			fragment_colour = vec4(allLights * spec_texture, 1.0);
		}
		else if (useDiffTexture == true && useSpecTexture == true){
			fragment_colour = vec4(allLights * textures, 1.0);
		}
		else{
			fragment_colour = vec4(allLights, 1.0);
		}
	}

}

vec3 newLight(vec3 lightPos, vec3 vertPos, float lightRange, vec3 light_intensity)
{
	
	//Distance Attenuatuion
	float D = length(vertPos - lightPos);
	float range = lightRange;
	float light_attenuation = 1 - smoothstep(0.0f, range, D);

	//Ambient Intensity
	vec3 ambient_intensity = ambient_material_colour;

	//Diffuse Intensity
	vec3 L = normalize(lightPos - vertPos); //surface to light
	float diffDot = max(dot(L, N), 0);
	vec3 diffuse_intensity = diffuse_material_colour * diffDot;
	diffuse_intensity = diffuse_intensity;

	//Specular
	vec3 H = -L;
	vec3 reflectVector = reflect(H, N);
	vec3 surface2camera = normalize(Camera_Position - vertPos);
	float cosAngle = max(0.0, dot(reflectVector, surface2camera));
	
	vec3 specular;
	
	if(shininess <= 0){
		specular = vec3(0, 0, 0);
	}
	else{
		specular = vec3(specular_colour * pow(cosAngle, shininess));
	}
	specular = clamp(specular, 0.0, 1.0);

	if(diffDot < 0) specular = vec3(0, 0, 0);

	vec3 scattered_light = vec3(ambient_intensity * diffuse_intensity * light_attenuation * light_intensity);
	vec3 reflected_light = vec3(specular * light_attenuation);

	vec3 final_light = scattered_light + reflected_light;

	return final_light;

}
