#version 330

out vec4 final_color;

in vec3 VS_normal_ws;
in vec3 VS_position_ws;
in vec3 VS_color;

uniform vec3 eye_position;

uniform vec3 material_specular_color;
uniform float material_shininess;

uniform vec4 light_position;
uniform vec4 light_ambient_color;
uniform vec4 light_diffuse_color;
uniform vec4 light_specular_color;

void main()
{
	vec3 N = normalize(VS_normal_ws);
	vec3 Eye = normalize(eye_position - VS_position_ws);

	vec3 L;
	if (light_position.w == 0.0)
		L = normalize(light_position.xyz);
	else
		L = normalize(light_position.xyz - VS_position_ws);

	vec3 H = normalize(L + Eye);

	float Idiff = max(dot(N, L), 0.0);
	float Ispec = Idiff * pow(max(dot(N, H), 0.0), material_shininess);

	vec3 mat_ambient = VS_color;
	vec3 mat_diffuse = VS_color;
	vec3 mat_specular = material_specular_color;

	vec3 light =
		mat_ambient * light_ambient_color.rgb +
		mat_diffuse * light_diffuse_color.rgb * Idiff +
		mat_specular * light_specular_color.rgb * Ispec;

	final_color = vec4(light, 1.0);
}