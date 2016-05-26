#version 330

in vec4 position;
in vec3 normal;
in vec2 tex_coord;


uniform mat4 model_matrix;
uniform mat4 light_view_matrix;
uniform mat4 shadow_projection_matrix;

uniform mat4 light_view_matrix2;
uniform mat4 shadow_projection_matrix2;
uniform mat4 bias_matrix;

layout(std140) uniform CameraData
{
	mat4 view_matrix;
	mat4 projection_matrix;
	vec3 eye_position;
};

out VertexData
{
	vec3 normal_ws;
	vec3 position_ws;
	vec2 tex_coord;
	vec4 shadowCoord;
	vec4 shadowCoord2;
} outData;

void main()
{
	outData.tex_coord = tex_coord;
	outData.position_ws = vec3(model_matrix * position);

	// We transform the normal using model_matrix, not the transpose of its inverse (the normal matrix).
	// This is correct as long as the transformation contains only translations and rotations.
	outData.normal_ws = normalize(mat3(model_matrix) * normal);
	outData.shadowCoord = shadow_projection_matrix * light_view_matrix * model_matrix* position;
	outData.shadowCoord.xyz /= outData.shadowCoord.w;
	outData.shadowCoord.xyz = outData.shadowCoord.xyz * 0.5+0.5;

	outData.shadowCoord2 = shadow_projection_matrix2 * light_view_matrix2 * model_matrix* position;
	outData.shadowCoord2.xyz /= outData.shadowCoord2.w;
	outData.shadowCoord2.xyz = outData.shadowCoord2.xyz * 0.5 + 0.5;



	gl_Position = projection_matrix * view_matrix * model_matrix * position;
}

