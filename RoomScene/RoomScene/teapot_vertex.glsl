#version 330

in vec4 position;
in vec3 normal;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

uniform vec4 material_color;

out vec3 VS_normal_ws;
out vec3 VS_position_ws;
out vec3 VS_color;

void main()
{
	VS_position_ws = vec3(model_matrix * position);
	
	// We transform the normal using model_matrix, not the transpose of its inverse (the normal matrix).
	// This is correct as long as the transformation contains only translations and rotations.
	VS_normal_ws = normalize(mat3(model_matrix) * normal);

	VS_color = material_color.rgb;

	gl_Position = projection_matrix * view_matrix * model_matrix * position;
}
