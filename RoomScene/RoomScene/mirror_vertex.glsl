#version 330

in vec4 position;
in vec3 normal;
in vec2 tex_coord;

uniform mat4 model_matrix;

uniform mat4 view_matrix;		// Light view!
uniform mat4 projection_matrix;



void main()
{

	gl_Position = projection_matrix * view_matrix * model_matrix * position;
}