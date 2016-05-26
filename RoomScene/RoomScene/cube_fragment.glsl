#version 330

out vec4 final_color;

in VertexData
{
	vec3 normal_ws;
vec3 position_ws;
vec2 tex_coord;
vec4 shadowCoord;
vec4 shadowCoord2;
} inData;

layout(std140) uniform CameraData
{
	mat4 view_matrix;
	mat4 projection_matrix;
	vec3 eye_position;
};

uniform vec3 material_specular_color;
uniform float material_shininess;



layout(std140) uniform LightData
{
	vec4 light_position;
	vec4 light_ambient_color;
	vec4 light_diffuse_color;
	vec4 light_specular_color;
};

layout(std140) uniform LightData2
{
	vec4 light2_position;
	vec4 light2_ambient_color;
	vec4 light2_diffuse_color;
	vec4 light2_specular_color;
};

uniform sampler2D wood_tex;
uniform sampler2D shadow_tex;
uniform sampler2D shadow_tex2;
uniform sampler2D mirror_tex;


void main()
{
	
	vec3 tex_color;
	tex_color = texture(wood_tex, inData.tex_coord).rgb;
	//tex_color = texture(shadow_tex, inData.tex_coord).rgb;
	//tex_color = vec3(texture(shadow_tex, gl_FragCoord.xy / vec2(4096f,4096f)).r);
	//float s = texture(shadow_tex2, inData.tex_coord).r;
	//final_color = vec4(tex_color, 1.0f);
	//return;

	//if(s>0)
	//	final_color = vec4(s,s, s,1);
	//else
	//{
	//	final_color = vec4(0, 0, 0,1);
	//}
	//return;
	// Lighting. Just compute tex_color and the lighting will be properly computed

	vec3 N = normalize(inData.normal_ws);
	vec3 Eye = normalize(eye_position - inData.position_ws);

	vec3 L;
	if (light_position.w == 0.0)
		L = normalize(light_position.xyz);
	else
		L = normalize(light_position.xyz - inData.position_ws);

	vec3 H = normalize(L + Eye);

	float Idiff = max(dot(N, L), 0.0);
	float Ispec = Idiff * pow(max(dot(N, H), 0.0), material_shininess);
	vec3 mat_ambient = tex_color;
	vec3 mat_diffuse = tex_color;
	vec3 mat_specular = material_specular_color;
	
	vec3 fromLight = inData.position_ws - light_position.xyz;
	vec3 lightDirection = vec3(0, -1, 0);
	float lightDistance = length(fromLight);
	float lightPower = 1.0f / clamp(lightDistance * lightDistance, 0.01f, 1.0f);
	lightPower *= pow(clamp(dot(normalize(fromLight), lightDirection), 0, 1), 4);
	
	float bias = 0.0001;
	float visibility = 1.0;
	
	if (texture2D(shadow_tex, inData.shadowCoord.xy).r < inData.shadowCoord.z - bias || inData.shadowCoord.z < 0 || inData.shadowCoord.z > 1 
		|| inData.shadowCoord.x < 0 || inData.shadowCoord.x > 1 || inData.shadowCoord.y < 0 || inData.shadowCoord.y > 1)
	{
		visibility = 0.8f;

	}

		

	/*for (int i = 4; i >= 1; --i)
	{
		float test = i * 0.2f;
		if (lightPower > test)
		{
			lightPower = test + 0.2f;
			break;
		}
		if (Idiff > test)
		{
			Idiff = test + 0.2f;
			break;
		}
	}
	if (lightPower < 0.1)
		lightPower = 0.0f;
	if (Idiff < 0.1)
		Idiff = 0.0f;*/


	//SECOND LIGHT

	// Lighting. Just compute tex_color and the lighting will be properly computed

	
	if (light2_position.w == 0.0)
		L = normalize(light2_position.xyz);
	else
		L = normalize(light2_position.xyz - inData.position_ws);

	H = normalize(L + Eye);

	float Idiff2 = max(dot(N, L), 0.0);
	float Ispec2 = Idiff2 * pow(max(dot(N, H), 0.0), material_shininess);
	
	vec3 fromLight2 = inData.position_ws - light2_position.xyz;
	vec3 lightDirection2 = vec3(0, -1, 0);
	float lightDistance2 = length(fromLight2);
	float lightPower2 = 1.0f / clamp(lightDistance2 * lightDistance2, 0.01f, 1.0f);
	lightPower2 *= pow(clamp(dot(normalize(fromLight2), lightDirection2), 0, 1), 2f);
	 
	
	lightPower2 = 0;
	
	vec3 light =
		mat_ambient * light_ambient_color.rgb +
		mat_diffuse * light_diffuse_color.rgb * Idiff * lightPower +
		mat_specular * light_specular_color.rgb * Ispec * lightPower +
		mat_ambient * light2_ambient_color.rgb +
		mat_diffuse * light2_diffuse_color.rgb * Idiff2 * lightPower2 +
		mat_specular * light2_specular_color.rgb * Ispec2 * lightPower2;
	
	light *= visibility;
	 //toonShading
	//if (abs(dot(Eye, N)) < 0.2)
	//{
	//	light = vec3(0, 0, 0);
	//}

	final_color = vec4(light * visibility, 1.0);
	//final_color = vec4( tex_color, 1);
}