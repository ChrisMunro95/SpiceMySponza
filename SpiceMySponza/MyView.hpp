#pragma once

#include <SceneModel/SceneModel_fwd.hpp>
#include <tygra/WindowViewDelegate.hpp>
#include <tgl/tgl.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <memory>
#include <unordered_map>

class MyView : public tygra::WindowViewDelegate
{
public:
	
    MyView();
	
    ~MyView();

    void setScene(std::shared_ptr<const SceneModel::Context> scene);

	void setNormalToggle(bool value);
	bool getToggleNormal(){ return surfaceNormal_; };

private:

    void
    windowViewWillStart(std::shared_ptr<tygra::Window> window) override;
	
    void
    windowViewDidReset(std::shared_ptr<tygra::Window> window,
                       int width,
                       int height) override;

    void
    windowViewDidStop(std::shared_ptr<tygra::Window> window) override;
    
    void
    windowViewRender(std::shared_ptr<tygra::Window> window) override;

    std::shared_ptr<const SceneModel::Context> scene_;

private:

	bool surfaceNormal_ = false;
	bool useDiffTexture_ = false;
	bool useSpecTexture_ = false;

	GLuint shader_program_;
	std::vector<GLuint> diff_texture_;
	std::vector<GLuint> spec_texture_;

	std::unordered_map<std::string, int> textures_;


	struct MeshGL{
		GLuint positions_vbo;
		GLuint normals_vbo;
		GLuint texcoords_vbo;
		GLuint element_vbo;
		GLuint vao;

		int element_count;

		MeshGL() : positions_vbo(0),
				   normals_vbo(0),
				   texcoords_vbo(0),
				   element_vbo(0),
				   vao(0),
				   element_count(0){}
	};

	std::map<SceneModel::MeshId, MeshGL> sponza_mesh_;

};
