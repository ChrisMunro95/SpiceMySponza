#include "MyView.hpp"
#include <SceneModel/SceneModel.hpp>
#include <tygra/FileHelper.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cassert>
#include <unordered_map>

MyView::MyView()
{
}

MyView::~MyView() {
}

void MyView::setScene(std::shared_ptr<const SceneModel::Context> scene)
{
	scene_ = scene;
}

//When toggled it will shade the scene using the normals value
//so you can check to see if the scene has been drawn correctly
//with out any (BAD)lighting that may obscure your view 
void MyView::setNormalToggle(bool value){
	surfaceNormal_ = value;

	GLboolean normal_toggle = surfaceNormal_;
	GLuint toggle_id = glGetUniformLocation(shader_program_, "toggle_normal");

	glUniform1i(toggle_id, normal_toggle);
}

void MyView::windowViewWillStart(std::shared_ptr<tygra::Window> window)
{
	assert(scene_ != nullptr);

	//load shaders from text file, compile errors can be viewed via the info log.
	GLint compile_status = 0;

	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertex_shader_string = tygra::stringFromFile("sponza_vs.glsl");
	const char *vertex_shader_code = vertex_shader_string.c_str();
	glShaderSource(vertex_shader, 1,
		(const GLchar **)&vertex_shader_code, NULL);
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status != GL_TRUE) {
		const int string_length = 1024;
		GLchar log[string_length] = "";
		glGetShaderInfoLog(vertex_shader, string_length, NULL, log);
		std::cerr << log << std::endl;
	}

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragment_shader_string = tygra::stringFromFile("sponza_fs.glsl");
	const char *fragment_shader_code = fragment_shader_string.c_str();
	glShaderSource(fragment_shader, 1,
		(const GLchar **)&fragment_shader_code, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status != GL_TRUE) {
		const int string_length = 1024;
		GLchar log[string_length] = "";
		glGetShaderInfoLog(fragment_shader, string_length, NULL, log);
		std::cerr << log << std::endl;
	}

	//create a shader program and attach the vertex shader and fragment shader
	shader_program_ = glCreateProgram();
	glAttachShader(shader_program_, vertex_shader);
	glBindAttribLocation(shader_program_, 0, "vertex_position");
	glBindAttribLocation(shader_program_, 1, "vertex_normal");
	glBindAttribLocation(shader_program_, 2, "texture_coord");
	glDeleteShader(vertex_shader);
	glAttachShader(shader_program_, fragment_shader);
	glDeleteShader(vertex_shader);
	glLinkProgram(shader_program_);

	//test if the program linked successfully
	GLint link_status = 0;
	glGetProgramiv(shader_program_, GL_LINK_STATUS, &link_status);
	if (link_status != GL_TRUE) {
		const int string_length = 1024;
		GLchar log[string_length] = "";
		glGetProgramInfoLog(shader_program_, string_length, NULL, log);
		std::cerr << log << std::endl;
	}


	//get all of the sponza meshes from the GeometryBuilder
	SceneModel::GeometryBuilder builder;
	const auto& source_meshes = builder.getAllMeshes();

	/*
	##################################
	loop through every mesh getting its position, normal texcoord and element
	data, then fill the OPENGL buffers with the givin data, this is an efficient use 
	of the openGL API as all the buffers for the ENTIRE scene get created in one loop,
	to make it even more efficient an interleaved buffer should be used(however i've not got that working yet)
	##################################
	*/

	for (const auto scene_mesh : source_meshes){
		MeshGL& newMesh = sponza_mesh_[scene_mesh.getId()];

		//extract the position, normal texcoord and element data
		const auto& positions = scene_mesh.getPositionArray();
		const auto& elements = scene_mesh.getElementArray();
		const auto& normals = scene_mesh.getNormalArray();
		const auto& texcoords = scene_mesh.getTextureCoordinateArray();

		//fill the 'positions_vbo' with the position data
		glGenBuffers(1, &newMesh.positions_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, newMesh.positions_vbo);
		glBufferData(GL_ARRAY_BUFFER,
			positions.size() * sizeof(glm::vec3),
			positions.data(),
			GL_STATIC_DRAW);

		//unbind the active buffer to ensure no potential faults occur 
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//fill the 'normals_vbo' with the position data
		glGenBuffers(1, &newMesh.normals_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, newMesh.normals_vbo);
		glBufferData(GL_ARRAY_BUFFER,
			normals.size() * sizeof(glm::vec3),
			normals.data(),
			GL_STATIC_DRAW);

		//unbind the active buffer to ensure no potential faults occur 
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//fill the 'texcoords' with the position data
		glGenBuffers(1, &newMesh.texcoords_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, newMesh.texcoords_vbo);
		glBufferData(GL_ARRAY_BUFFER,
			texcoords.size() * sizeof(glm::vec2),
			texcoords.data(),
			GL_STATIC_DRAW);

		//unbind the active buffer to ensure no potential faults occur 
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//fill the 'element_vbo' with element data
		glGenBuffers(1, &newMesh.element_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh.element_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			elements.size() * sizeof(unsigned int),
			elements.data(),
			GL_STATIC_DRAW);

		//unbind the active buffer to ensure no potential faults occur 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//update the element count
		newMesh.element_count = elements.size();

		glGenVertexArrays(1, &newMesh.vao);
		glBindVertexArray(newMesh.vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh.element_vbo);

		glBindBuffer(GL_ARRAY_BUFFER, newMesh.positions_vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), TGL_BUFFER_OFFSET(0));

		glBindBuffer(GL_ARRAY_BUFFER, newMesh.normals_vbo);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), TGL_BUFFER_OFFSET(0));

		glBindBuffer(GL_ARRAY_BUFFER, newMesh.texcoords_vbo);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), TGL_BUFFER_OFFSET(0));

		//unbind the active buffer to ensure no potential faults occur 
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	/*
	###################################
	There are 4 textures for this scene seperated into two categories Diffuse and Specular
		Ive made things easy for myself by creating:
			- two textures arrays (to hold the Diffuse / Specular textures)
			- A Hash Map/un_ordered map (to hold the string value (filename) of the textures,
										and the index of the array (of where the texture is stored))
			
	The hash map will be used in the render method, as I'll Check every instance for a diffuse or specular
	texture, if one is found I'll then create a texture sampler and send it to the shader and handle it from 
	there.
	###################################
	*/

	//extract the materials form the scene
	const auto& sponza_materials = scene_->getAllMaterials();

	//loop through all the materials
	for (unsigned int i = 0; i < sponza_materials.size(); i++){

		diff_texture_.resize(sponza_materials.size());

		const auto diff_texture_string = sponza_materials[i].getDiffuseTexture();

		const auto spec_texture_string = sponza_materials[i].getSpecularTexture();

		//check to see if the DIFFUSE string is NOT empty, when it isnt that 
		//means a texture can be created and stored
		if (diff_texture_string != ""){

			//add the string along with the index value to the hash map
			textures_.insert({ diff_texture_string, i });

			//Create a texture object from pixel data read from an PNG.
			tygra::Image texture_image = tygra::imageFromPNG(diff_texture_string);

			if (texture_image.containsData()) {
				glGenTextures(1, &diff_texture_[i]);
				glBindTexture(GL_TEXTURE_2D, diff_texture_[i]);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				GLenum pixel_formats[] = { 0, GL_RED, GL_RG, GL_RGB, GL_RGBA };
				glTexImage2D(GL_TEXTURE_2D,
					0,
					GL_RGBA,
					texture_image.width(),
					texture_image.height(),
					0,
					pixel_formats[texture_image.componentsPerPixel()],
					texture_image.bytesPerComponent() == 1 ? GL_UNSIGNED_BYTE
					: GL_UNSIGNED_SHORT,
					texture_image.pixels());
				glGenerateMipmap(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);

			}

		}

		spec_texture_.resize(sponza_materials.size());

		//check to see if the SPECULAR string is NOT empty, when it isnt that 
		//means a texture can be created and stored
		if (spec_texture_string != ""){

			//add the string along with the index value to the hash map
			textures_.insert({ spec_texture_string, i });

			//Create a texture object from pixel data read from an PNG.
			tygra::Image texture_image = tygra::imageFromPNG(spec_texture_string);

			if (texture_image.containsData()) {
				glGenTextures(1, &spec_texture_[i]);
				glBindTexture(GL_TEXTURE_2D, spec_texture_[i]);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				GLenum pixel_formats[] = { 0, GL_RED, GL_RG, GL_RGB, GL_RGBA };
				glTexImage2D(GL_TEXTURE_2D,
					0,
					GL_RGBA,
					texture_image.width(),
					texture_image.height(),
					0,
					pixel_formats[texture_image.componentsPerPixel()],
					texture_image.bytesPerComponent() == 1 ? GL_UNSIGNED_BYTE
					: GL_UNSIGNED_SHORT,
					texture_image.pixels());
				glGenerateMipmap(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);

			}

		}

	}


}

void MyView::windowViewDidReset(std::shared_ptr<tygra::Window> window,
	int width,
	int height)
{
	glViewport(0, 0, width, height);
}

void MyView::windowViewDidStop(std::shared_ptr<tygra::Window> window)
{
	glDeleteProgram(shader_program_);

	for (unsigned int i = 0; i < sponza_mesh_.size(); i++){
		glDeleteBuffers(1, &sponza_mesh_[i].positions_vbo);
		glDeleteBuffers(1, &sponza_mesh_[i].normals_vbo);
		glDeleteBuffers(1, &sponza_mesh_[i].texcoords_vbo);
		glDeleteBuffers(1, &sponza_mesh_[i].element_vbo);
		glDeleteVertexArrays(1, &sponza_mesh_[i].vao);
	}

}

void MyView::windowViewRender(std::shared_ptr<tygra::Window> window)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	assert(scene_ != nullptr);

	glClearColor(0.f, 0.f, 0.25f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const auto& camera = scene_->getCamera();

	//calc the aspect ratio of the viewport/window
	GLint viewport_size[4];
	glGetIntegerv(GL_VIEWPORT, viewport_size);
	const float aspect_ratio = viewport_size[2] / (float)viewport_size[3];

	//create the projection matrix using the aspect ratio
	glm::mat4 projection_xform = glm::perspective(75.f, aspect_ratio, 1.f, 1000.f);

	//create a 'scene view matrix' using data provided by the camera
	auto camera_position = camera.getPosition();
	auto camera_direction = camera.getDirection();

	//Send the camera position data to the shader program, this will be needed when calculating
	//the specular reflection for the Phong Shading Model
	GLuint camera_position_id = glGetUniformLocation(shader_program_, "Camera_Position");
	glUniform3fv(camera_position_id, 1, glm::value_ptr(camera_position));

	auto camera_at_position = camera_position + camera_direction;

	glm::mat4 view_xform = glm::lookAt(camera_position, camera_at_position, glm::vec3(0, 1, 0));

	//create the 'projection model veiw matrix' 
	glm::mat4 projection_view_model_xform = projection_xform * view_xform;

	//'Activate' The program so data can be sent to it
	glUseProgram(shader_program_);

	//add the projection_view_model_xform to the shader program
	GLuint projection_view_model_xform_id = glGetUniformLocation(shader_program_,
		"projection_view_model_xform");

	glUniformMatrix4fv(projection_view_model_xform_id, 1, GL_FALSE,
		glm::value_ptr(projection_view_model_xform));

	//get the light info from the scene and pass it to the shader program
	const auto& sponza_light_ = scene_->getAllLights();

	//create vectors that will hold all the necessary data
	std::vector<glm::vec3> light_positions;
	std::vector<glm::vec3> light_intensity;
	std::vector<float> light_range;

	const int sizeOfArray = sponza_light_.size();

	for (int i = 0; i < sizeOfArray; i++){
		light_positions.push_back(sponza_light_[i].getPosition());
		light_intensity.push_back(sponza_light_[i].getIntensity());
		light_range.push_back(sponza_light_[i].getRange());
	}

	//create the Uniform IDs and send the data to the shader program 
	GLuint light_position_id = glGetUniformLocation(shader_program_, "Light_Position");
	GLuint light_intensity_id = glGetUniformLocation(shader_program_, "Light_Intensity");
	GLuint light_range_id = glGetUniformLocation(shader_program_, "Light_Range");

	//NOTE: the data needs to be cast to a GLfloat for GLSL to accept the data
	glUniform3fv(light_position_id, sizeOfArray, reinterpret_cast<GLfloat *>(light_positions.data()));
	glUniform3fv(light_intensity_id, sizeOfArray, reinterpret_cast<GLfloat *>(light_intensity.data()));
	glUniform1fv(light_range_id, sizeOfArray, reinterpret_cast<GLfloat *>(light_range.data()));

	//loop throught every instance/mesh in the scene
	for (const auto& instance : scene_->getAllInstances()){

		// create and add the model_xform to the shader_program
		glm::mat4 model_xform = glm::mat4(instance.getTransformationMatrix());
		GLuint model_xform_id = glGetUniformLocation(shader_program_, "model_xform");
		glUniformMatrix4fv(model_xform_id, 1, GL_FALSE, glm::value_ptr(model_xform));

		//DIFFUSE
		//get the material id and create the vec3 from the get material id call
		auto material_instance_id = instance.getMaterialId();
		glm::vec3 material_diff_colour = scene_->getMaterialById(material_instance_id).getDiffuseColour();

		//material uniform
		GLuint diff_material_id = glGetUniformLocation(shader_program_, "diffuse_material_colour");
		glUniform3fv(diff_material_id, 1, glm::value_ptr(material_diff_colour));

		//AMBIENT
		glm::vec3 material_amb_colour = scene_->getMaterialById(material_instance_id).getAmbientColour();

		//material uniform
		GLuint amb_material_id = glGetUniformLocation(shader_program_, "ambient_material_colour");
		glUniform3fv(amb_material_id, 1, glm::value_ptr(material_amb_colour));

		//TEXTURES
		//get the diffuse texture string for THIS instance
		auto diff_texture_string = scene_->getMaterialById(material_instance_id).getDiffuseTexture();

		//get the specular texture string for THIS instance
		auto spec_texture_string = scene_->getMaterialById(material_instance_id).getSpecularTexture();

		//create a hash map iterator
		std::unordered_map<std::string, int>::const_iterator got;

		/*
		####################################
		If an Empty string is found that means there is no texture associated 
		with this instance so I send a 'FALSE' bool to the shader and handle it from 
		there i.e I Dont Sample any Texture

		When the string ISN'T empty I try match it with a Hash Map 'item' by iterating through
		all the Hash Map 'items', when a match is made i get it's index value and create the 
		texture sample needed and send a 'TRUE' bool to the shader and handle it from there
		i.e I DO sample the texture
		####################################
		*/

		if (diff_texture_string == ""){
			//no diffuse texture on this instance
			useDiffTexture_ = false;
		}
		else{
			got = textures_.find(diff_texture_string);
			if (got == textures_.end()){
				//no texture found within hash map
				useDiffTexture_ = false;
			}
			else{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, diff_texture_[got->second]);
				glUniform1i(glGetUniformLocation(shader_program_, "diff_tex_sample"), 0);
				useDiffTexture_ = true;
			}
		}
		//check for specular
		if (spec_texture_string == ""){
			//no specular texture on this instance
			useSpecTexture_ = false;
		}
		else{
			got = textures_.find(spec_texture_string);
			if (got == textures_.end()){
				//no texture found within hash map
				useSpecTexture_ = false;
			}
			else{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, spec_texture_[got->second]);
				glUniform1i(glGetUniformLocation(shader_program_, "spec_tex_sample"), 1);
				useSpecTexture_ = true;
			}
		}

		/*
		Create and send the Uniform Bool variables the the Shader program.
			There reason why i have two different booleans (Diff and Spec) is because
			one instance may have both a Diffuse texture and a Specular texture
			so i want to be able to handle that properly within the shader
		*/
		GLboolean useDiffTexture = useDiffTexture_;
		GLuint useDiffTexture_id = glGetUniformLocation(shader_program_, "useDiffTexture");
		glUniform1i(useDiffTexture_id, useDiffTexture);

		GLboolean useSpecTexture = useSpecTexture_;
		GLuint useSpecTexture_id = glGetUniformLocation(shader_program_, "useSpecTexture");
		glUniform1i(useSpecTexture_id, useSpecTexture);

		//specular
		auto specular = scene_->getMaterialById(material_instance_id).getSpecularColour();

		//specular uniform
		GLuint specular_id = glGetUniformLocation(shader_program_, "specular_colour");
		glUniform3fv(specular_id, 1, glm::value_ptr(specular));

		//shininess
		float shininess = scene_->getMaterialById(material_instance_id).getShininess();

		//shininess uniform
		GLuint shininess_id = glGetUniformLocation(shader_program_, "shininess");
		glUniform1f(shininess_id, shininess);

		auto id = instance.getMeshId();

		//draw the mesh
		const MeshGL& mesh = sponza_mesh_[instance.getMeshId()];

		glBindVertexArray(mesh.vao);
		glDrawElements(GL_TRIANGLES, mesh.element_count, GL_UNSIGNED_INT, 0);

	}

}
