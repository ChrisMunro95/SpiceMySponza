#version 330

uniform mat4 projection_view_model_xform;
uniform mat4 model_xform;

in vec3 vertex_position;
in vec3 vertex_normal;
in vec2 texture_coord;

out vec3 colour_normals;
out vec3 P;
out vec3 N;
out vec2 texcoords;

void main(void)
{
	P = vec3(model_xform * vec4(vertex_position, 1.0));
	N = vec3(mat3(model_xform) * normalize(vertex_normal));

	texcoords = texture_coord;

	colour_normals = vec3(mat3(model_xform) * vertex_normal);
	colour_normals = (colour_normals / 2) + 0.5;

	gl_Position = (projection_view_model_xform * model_xform) * vec4(vertex_position, 1.0);
}
