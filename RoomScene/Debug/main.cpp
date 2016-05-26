// PV112, lesson 6 - uniform buffer objects, instancing

#include <iostream>
#include "Graphics.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <ctime>

#include <iostream>
// Include GLEW, use static library
#define GLEW_STATIC
#include <GL/glew.h>
#pragma comment(lib, "glew32s.lib")		// Link with GLEW library

#pragma comment(lib, "DevIL.lib")		// Link with DevIL library

// Include FreeGLUT
#include <GL/freeglut.h>

// Include the most important GLM functions
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace PV112;

// Current window size
int win_width = 1920;
int win_height = 1080;

// Shader program for the cube and its uniforms
GLuint cube_program;
GLuint shader_program;
GLuint shader_view_matrix_loc;
GLuint shader_projection_matrix_loc;
GLuint shader_model_matrix_loc;
GLuint cube_bias_matrix_loc;
GLint cube_model_matrix_loc;

GLint cube_material_specular_color_loc;
GLint cube_material_shininess_loc;

GLint cube_light_position_loc;
GLint cube_light_ambient_color_loc;
GLint cube_light_diffuse_color_loc;
GLint cube_light_specular_color_loc;

GLint cube_eye_position_loc;

GLint cube_wood_tex_loc;
GLint stone_tex_loc;
GLint walls_tex_loc;
GLint cube_light_view_matrix_loc;
GLint cube_shadow_proj_matrix_loc;

GLint shader_projection_matrix_loc2 ;
GLint shader_model_matrix_loc2;
GLint shader_view_matrix_loc2;
GLint cube_light_view_matrix_loc2;
GLint cube_shadow_proj_matrix_loc2;


// Simple geometries that we will use in this lecture
PV112Geometry my_cube;
PV112Geometry my_floor;
PV112Geometry my_cornice;
PV112Geometry my_door;
PV112Geometry my_lamp;
PV112Geometry my_table;
PV112Geometry my_chair;
PV112Geometry my_map;

PV112Geometry my_bulb;
PV112Geometry my_notebook;
PV112Geometry my_notebook_display;
PV112Geometry my_cabinet;
PV112Geometry my_cabinet_leftDoor;
PV112Geometry my_cabinet_rightDoor;
PV112Geometry my_metronome;
PV112Geometry my_metronome_hand;
PV112Geometry my_box;
PV112Geometry my_clock;
PV112Geometry my_clock_hoursHand;
PV112Geometry my_clock_minuteHand;
PV112Geometry my_library;
PV112Geometry my_vase;
PV112Geometry my_couch;
PV112Geometry my_bin;


// Simple camera that allows us to look at the object from different views
PV112Camera my_camera;

// Textures ID
GLuint wood_tex;
GLuint stone_tex;
GLuint walls_tex;
GLuint woodChair_tex;
GLuint notebook_tex;
GLuint clock_tex;
GLuint map_tex;
GLuint clock_white_tex;
GLuint clock_black_tex;
GLuint doorwood_tex;
GLuint bin_tex;
GLuint hard_wood_tex;
GLuint bulb_tex;



std::vector<graphicsObject*> objects = std::vector<graphicsObject*>();

ShadowFramebuffer shadowFramebuffer;
ShadowFramebuffer shadowFramebuffer2;

// Data of our light
struct Light
{
	glm::vec4 position;
	glm::vec4 ambient_color;
	glm::vec4 diffuse_color;
	glm::vec4 specular_color;
};
Light LightData;			// Data of the light on CPU
GLuint LightData_ubo;		// Data of the light on GPU in a uniform buffer object

Light LightData2;
GLuint LightData2_ubo;
							// Data of our camera
struct Camera
{
	glm::mat4 view_matrix;
	glm::mat4 projection_matrix;
	glm::vec3 eye_position;
};
Camera CameraData;			// Data of the camera on CPU
GLuint CameraData_ubo;		// Data of the camera on GPU in a uniform buffer object

							// Positions of all instanced cubes
// Data of the cubes on CPU
GLuint CubeData_ubo;	// Data of the cubes on GPU in a uniform buffer object

						// Positions and colors of all instanced teapots

						// Current time of the application in seconds, for animations
float app_time_s = 0.0f;
float getAngleForMetronome(float time);

float getAngleForClockH();
float getAngleForClockM();
// Called when the user presses a key
void key_pressed(unsigned char key, int mouseX, int mouseY)
{
	switch (key)
	{
	case 27:		// VK_ESCAPE
		exit(0);
		break;
	case 'l':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glutPostRedisplay();
		break;
	case 'f':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glutPostRedisplay();
		break;
	case 't':
		glutFullScreenToggle();
		break;
	}
}

// Called when the user presses a mouse button
void mouse_bottom_changed(int button, int state, int x, int y)
{
	my_camera.OnMouseBottomChanged(button, state, x, y);
}

// Called when the user moves with the mouse (when some mouse button is pressed)
void mouse_moved(int x, int y)
{
	my_camera.OnMouseMoved(x, y);
}

// Draws PV112Geometry like DrawGeometry, it only uses instaned drawing
void DrawGeometryInstanced(const PV112Geometry &geom, int primcount)
{
	if (geom.DrawArraysCount > 0)
		glDrawArraysInstanced(geom.Mode, 0, geom.DrawArraysCount, primcount);
	if (geom.DrawElementsCount > 0)
		glDrawElementsInstanced(geom.Mode, geom.DrawElementsCount, GL_UNSIGNED_INT, nullptr, primcount);
}


// Initializes OpenGL stuff
void init()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);

	// Force indices of input variables
	int position_loc = 0;
	int normal_loc = 1;
	int tex_coord_loc = 2;

	//------------
	//--  Cube  --

	// Create shader program
	cube_program = CreateAndLinkProgram("cube_vertex.glsl", "cube_fragment.glsl",
		position_loc, "position", normal_loc, "normal", tex_coord_loc, "tex_coord");

	shader_program = CreateAndLinkProgram("shadowMapVertexShader.glsl", "shadowMapFragmentShader.glsl",
		position_loc, "position", normal_loc, "normal", tex_coord_loc, "tex_coord");


	if (0 == cube_program)
		WaitForEnterAndExit();
	if (0 == shader_program)
		WaitForEnterAndExit();
	glUseProgram(cube_program);
	// Get variable locations
	cube_model_matrix_loc = glGetUniformLocation(cube_program, "model_matrix");
	cube_bias_matrix_loc = glGetUniformLocation(cube_program, "bias_matrix");

	cube_material_specular_color_loc = glGetUniformLocation(cube_program, "material_specular_color");
	cube_material_shininess_loc = glGetUniformLocation(cube_program, "material_shininess");
	cube_light_position_loc = glGetUniformLocation(cube_program, "light_position");
	cube_light_diffuse_color_loc = glGetUniformLocation(cube_program, "light_diffuse_color");
	cube_light_ambient_color_loc = glGetUniformLocation(cube_program, "light_ambient_color");
	cube_light_specular_color_loc = glGetUniformLocation(cube_program, "light_specular_color");
	cube_eye_position_loc = glGetUniformLocation(cube_program, "eye_position");
	cube_wood_tex_loc = glGetUniformLocation(cube_program, "wood_tex");
	stone_tex_loc = glGetUniformLocation(cube_program, "stone_tex");
	walls_tex_loc = glGetUniformLocation(cube_program, "walls_tex");

	GLint shadowTex = glGetUniformLocation(cube_program, "shadow_tex");
	GLint shadowTex2 = glGetUniformLocation(cube_program, "shadow_tex2");

	glUniform1i(cube_wood_tex_loc, 0);
	glUniform1i(shadowTex, 1);
	glUniform1i(shadowTex2, 2);

	cube_light_view_matrix_loc = glGetUniformLocation(cube_program, "light_view_matrix");
	cube_shadow_proj_matrix_loc = glGetUniformLocation(cube_program, "shadow_projection_matrix");

	cube_light_view_matrix_loc2 = glGetUniformLocation(cube_program, "light_view_matrix2");
	cube_shadow_proj_matrix_loc2 = glGetUniformLocation(cube_program, "shadow_projection_matrix2");




	
	//SHADOW PROGRAM
	glUseProgram(shader_program);

	CreateFramebuffer(&shadowFramebuffer, glm::vec2(win_width, win_height));
	shader_view_matrix_loc = glGetUniformLocation(shader_program, "view_matrix");
	shader_projection_matrix_loc = glGetUniformLocation(shader_program, "projection_matrix");
	shader_model_matrix_loc = glGetUniformLocation(shader_program, "model_matrix");

	CreateFramebuffer(&shadowFramebuffer2, glm::vec2(win_width, win_height));


	//LIGHT AND CAMERA UNIFORM
	glUseProgram(cube_program);
	int cube_LightData_loc = glGetUniformBlockIndex(cube_program, "LightData");
	glUniformBlockBinding(cube_program, cube_LightData_loc, 0);
	int cube_LightData2_loc = glGetUniformBlockIndex(cube_program, "LightData2");
	glUniformBlockBinding(cube_program, cube_LightData2_loc, 2);

	int cube_CameraData_loc = glGetUniformBlockIndex(cube_program, "CameraData");
	glUniformBlockBinding(cube_program, cube_CameraData_loc, 1);


	// Set the sampler to take the data for 0-th texture unit


	glUseProgram(0);



	//-------------
	//--  Light  --

	// Light from small Lamp
	LightData.position = glm::vec4(3.05, -0.56, -2.14, 1.0f);
	
	LightData.ambient_color = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	LightData.diffuse_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	LightData.specular_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	glGenBuffers(1, &LightData_ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, LightData_ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Light), &LightData, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//Light from big Lamp

	LightData2.position = glm::vec4(-4.5, 1.5, -2.14, 1);

	LightData2.ambient_color = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
	LightData2.diffuse_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	LightData2.specular_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	glGenBuffers(1, &LightData2_ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, LightData2_ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Light), &LightData2, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 1);

	//--------------
	//--  Camera  --

	CameraData.view_matrix = glm::mat4(1.0f);
	CameraData.projection_matrix = glm::mat4(1.0f);
	CameraData.eye_position = glm::vec3(0.0f);

	glGenBuffers(1, &CameraData_ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, CameraData_ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Camera), &CameraData, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);



	//-------------
	//--  Other  --

	// Create geometries
	my_cube = CreateWalls(position_loc, normal_loc, tex_coord_loc);
	my_floor = CreateFloor(position_loc, normal_loc, tex_coord_loc);
	my_cornice = CreateCornice(position_loc, normal_loc, tex_coord_loc);
	my_map = CreateMap(position_loc, normal_loc, tex_coord_loc);
	my_clock_hoursHand = CreateHHand(position_loc, normal_loc, tex_coord_loc);
	my_clock_minuteHand = CreateMHand( position_loc, normal_loc, tex_coord_loc);
	my_chair = LoadOBJ("chair2.obj", position_loc, normal_loc, tex_coord_loc);
	my_lamp = LoadOBJ("lamp.obj", position_loc, normal_loc, tex_coord_loc);
	my_table = LoadOBJ("table.obj", position_loc, normal_loc, tex_coord_loc);
	my_bin = LoadOBJ("bin.obj", position_loc, normal_loc, tex_coord_loc);
	my_door = LoadOBJ("door.obj", position_loc, normal_loc, tex_coord_loc);
	my_bulb = LoadOBJ("bulb.obj", position_loc, normal_loc, tex_coord_loc);
	my_notebook = LoadOBJ("notebook.obj", position_loc, normal_loc, tex_coord_loc);
	my_notebook_display = LoadOBJ("notebook-display.obj", position_loc, normal_loc, tex_coord_loc);
	my_cabinet = LoadOBJ("cabinet.obj", position_loc, normal_loc, tex_coord_loc);
	my_cabinet_rightDoor = LoadOBJ("cabinet-rightDoor.obj", position_loc, normal_loc, tex_coord_loc);
	my_cabinet_leftDoor = LoadOBJ("cabinet-leftDoor.obj", position_loc, normal_loc, tex_coord_loc);
	my_metronome = LoadOBJ("metronome.obj", position_loc, normal_loc, tex_coord_loc);
	my_metronome_hand = LoadOBJ("metronome-hand.obj", position_loc, normal_loc, tex_coord_loc);
	my_box = LoadOBJ("box.obj", position_loc, normal_loc, tex_coord_loc);
	my_clock = LoadOBJ("clock.obj", position_loc, normal_loc, tex_coord_loc);
	my_vase = LoadOBJ("vase.obj", position_loc, normal_loc, tex_coord_loc);
	my_library = LoadOBJ("library.obj", position_loc, normal_loc, tex_coord_loc);
	

	// Load and set the textures
	CreateAndLoadTexture2D(L"wood.jpg", wood_tex);

	CreateAndLoadTexture2D(L"darkWood2.jpg", stone_tex);
	
	CreateAndLoadTexture2D(L"walls5.jpg", walls_tex);
	
	CreateAndLoadTexture2D(L"woodchair.jpg", woodChair_tex);
	
	CreateAndLoadTexture2D(L"notebook.jpg", notebook_tex);
	
	CreateAndLoadTexture2D(L"metronomewood.jpg", clock_tex);
	
	CreateAndLoadTexture2D(L"map.jpg", map_tex);
	
	CreateAndLoadTexture2D(L"metal.jpg", clock_white_tex);
	
	CreateAndLoadTexture2D(L"clockblack.jpg",clock_black_tex);
	
	CreateAndLoadTexture2D(L"doorwood.jpg", doorwood_tex);

	CreateAndLoadTexture2D(L"bin.jpg", bin_tex);
	
	CreateAndLoadTexture2D(L"hardwood.jpg", hard_wood_tex);
	
	CreateAndLoadTexture2D(L"bulb.jpg", bulb_tex);
}

// Called when the window needs to be rerendered
void render()
{
	my_camera.UpdateRotation();
	
	// set all objects textures and model matrixes
	//
	//--------------------------------------
	//
	// Set model's model matrix
	glm::mat4 model_matrix;
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.015, 0.015, 0.015));;
	graphicsObject *tempObject;

	// Set the Floor
	model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(5, 2, 3));
	tempObject = new graphicsObject(my_floor, model_matrix,cube_model_matrix_loc, stone_tex);
	objects.push_back(tempObject);
	
	// Set the map
	model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.5, 0.65, -2)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.8, 0.5, 0.5));
	tempObject = new graphicsObject(my_map, model_matrix, cube_model_matrix_loc, map_tex);
	objects.push_back(tempObject);

	// Set the Walls
	model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(5, 2, 3));
	tempObject = new graphicsObject(my_cube, model_matrix, cube_model_matrix_loc, walls_tex);
	objects.push_back(tempObject);
	
	// Set the Cornice
	model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(5, 2, 3));
	tempObject = new graphicsObject(my_cornice, model_matrix, cube_model_matrix_loc, stone_tex);
	objects.push_back(tempObject);
		
	// Set the Door
	model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, -2, 3)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.007, 0.007, 0.007)) *glm::rotate(glm::mat4(1.0f), glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f))* glm::rotate(glm::mat4(1.0f), glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));;
	tempObject = new graphicsObject(my_door, model_matrix, cube_model_matrix_loc, doorwood_tex);
	objects.push_back(tempObject);
	
	//Set the lamp with first Light
	model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(3, -0.9, -2.2)) * scaleMatrix * glm::rotate(glm::mat4(1.0f), glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
	tempObject = new graphicsObject(my_lamp, model_matrix, cube_model_matrix_loc, clock_white_tex);
	objects.push_back(tempObject);
	
	//Set the bulb of first Lamp
	model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(3.00, -0.662, -2.14)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.003, 0.003, 0.003))* glm::rotate(glm::mat4(1.0f), glm::radians(-30.f), glm::vec3(1.f, 0.f, 0.f));
	tempObject = new graphicsObject(my_bulb, model_matrix, cube_model_matrix_loc, notebook_tex);
	objects.push_back(tempObject);

	//Set the lamp with second Light
	model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-4.5, -2, -2.14)) * scaleMatrix *glm::scale(glm::mat4(),glm::vec3(2.f,11.f,2.f)) * glm::rotate(glm::mat4(1.0f), glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
	tempObject = new graphicsObject(my_lamp, model_matrix, cube_model_matrix_loc, clock_white_tex);
	objects.push_back(tempObject);

	//Set the bulb light
	model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(LightData2.position.x, LightData2.position.y, LightData2.position.z)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.009, 0.009, 0.009));
	tempObject = new graphicsObject(my_bulb, model_matrix, cube_model_matrix_loc, bulb_tex);
	objects.push_back(tempObject);

	
	
	
	//Set the table
	model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(3.8, -2, -2.3)) * scaleMatrix;
	tempObject = new graphicsObject(my_table, model_matrix, cube_model_matrix_loc, woodChair_tex);
	objects.push_back(tempObject);
	
	//Set the chair
	model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(3.8, -2, -1.4)) * glm::rotate(glm::mat4(1.0f), glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));
	tempObject = new graphicsObject(my_chair, model_matrix, cube_model_matrix_loc, woodChair_tex);
	objects.push_back(tempObject);
		
	//Set bin
	model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(3, -2, -2.3)) * scaleMatrix;
	tempObject = new graphicsObject(my_bin, model_matrix, cube_model_matrix_loc, bin_tex);
	objects.push_back(tempObject);

	
	//Set the notebook and display
	model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(3.8, -0.9, -2)) *  scaleMatrix;
	tempObject = new graphicsObject(my_notebook, model_matrix, cube_model_matrix_loc, walls_tex);
	objects.push_back(tempObject);

	tempObject = new graphicsObject(my_notebook_display, model_matrix, cube_model_matrix_loc, clock_black_tex);
	objects.push_back(tempObject);
	
	// Set the cabinet and his doors
	model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-3.8, -2, 2)) * scaleMatrix * glm::rotate(glm::mat4(1.0f), glm::radians(135.f), glm::vec3(0.f, 1.f, 0.f));
	tempObject = new graphicsObject(my_cabinet, model_matrix, cube_model_matrix_loc, hard_wood_tex);
	objects.push_back(tempObject);

	tempObject = new graphicsObject(my_cabinet_leftDoor, model_matrix, cube_model_matrix_loc, hard_wood_tex);
	objects.push_back(tempObject);

	//Set vase
	model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-4, -2, -1.7)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.03, 0.03, 0.03));
	tempObject = new graphicsObject(my_vase, model_matrix, cube_model_matrix_loc, clock_tex);
	objects.push_back(tempObject);
	
	//Set clock
	model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-4.9, 1, -2)) * scaleMatrix  *glm::rotate(glm::mat4(1.0f), glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
	tempObject = new graphicsObject(my_clock, model_matrix, cube_model_matrix_loc, walls_tex);
	objects.push_back(tempObject);
	
	model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-4.9, 0.92, -2)) * scaleMatrix *glm::rotate(glm::mat4(1.0f), glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f)) *glm::rotate(glm::mat4(1.0f), glm::radians(getAngleForClockH()), glm::vec3(0.f, 0.f, 1.f));
	tempObject = new graphicsObject(my_clock_hoursHand, model_matrix, cube_model_matrix_loc, clock_black_tex);
	objects.push_back(tempObject);
	
	model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-4.9, 0.92, -2))* scaleMatrix *glm::rotate(glm::mat4(1.0f), glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f)) *glm::rotate(glm::mat4(1.0f), glm::radians(getAngleForClockM()), glm::vec3(0.f, 0.f, 1.f));
	tempObject = new graphicsObject(my_clock_minuteHand, model_matrix, cube_model_matrix_loc, clock_black_tex);
	objects.push_back(tempObject);

	// Set metronome with hand
	model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(3.5, -0.9, -2.4)) * scaleMatrix;
	tempObject = new graphicsObject(my_metronome, model_matrix, cube_model_matrix_loc, clock_tex);
	objects.push_back(tempObject);
	
	tempObject = new graphicsObject(my_metronome_hand, model_matrix, cube_model_matrix_loc, walls_tex);
	objects.push_back(tempObject);
	

	//----------------------------------------
	
	//SHADOW PASS FOR FIRST LIGHT
	
	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	glUseProgram(shader_program);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFramebuffer.framebuffer);
	glClear(GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
		
	model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(5, 2, 3));;
	glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), float(win_width) / float(win_height), 0.01f, 500.0f);
	glm::mat4 shadow_projection_matrix = glm::perspective(glm::radians(60.f), 1.0f, 0.01f, 2.f);
	//shadow_projection_matrix = glm::perspective(glm::radians(60.f), 1.0f, 0.1f, 6.f);

	glm::vec3 lightCentre = glm::vec3(LightData.position.x , LightData.position.y, LightData.position.z) + glm::vec3(0.0,-2,0.5f) ;
	//lightCentre = glm::vec3(0,0,0) + glm::vec3(-1.0, -2, -1.f);
	glm::vec3 lightPosition = glm::vec3(LightData.position.x, LightData.position.y - 0.05f, LightData.position.z);
	//lightPosition = glm::vec3(-4.9, 1, -2.9);
	glm::mat4 light_view_matrix =  glm::lookAt(lightPosition, lightCentre, glm::normalize(glm::vec3(0.f, 1.0f, 0.f)));
	
	glUniformMatrix4fv(shader_view_matrix_loc, 1, GL_FALSE, glm::value_ptr(light_view_matrix));
	glUniformMatrix4fv(shader_projection_matrix_loc, 1, GL_FALSE, glm::value_ptr(shadow_projection_matrix));
	
	//DRAW ALL OBJECTS IN SCENE FOR SHADOWS
	//DrawObjectsForShadowPass(objects, shader_model_matrix_loc);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
	
	
	
	

	//shadow pass 2
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader_program);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFramebuffer2.framebuffer);
	glClear(GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);

	glm::mat4 model_matrix2 = glm::scale(glm::mat4(1.0f), glm::vec3(5, 2, 3));;
	glm::mat4 projection_matrix2 = glm::perspective(glm::radians(45.0f), float(win_width) / float(win_height), 0.01f, 500.0f);
	glm::mat4 shadow_projection_matrix2 = glm::perspective(glm::radians(120.f), 1.0f, 0.1f, 7.f);
	glm::vec3 lightCentre2 = glm::vec3(LightData2.position.x , LightData2.position.y, LightData2.position.z) + glm::vec3(0.01, -0.2, 0.5f);
	glm::mat4 light_view_matrix2 =  //glm::lookAt(glm::vec3(LightData.position.x, LightData.position.y, LightData.position.z), lightCentre, glm::vec3(0.f, 1.01f, 0.f));
		glm::lookAt(glm::vec3(LightData2.position.x, LightData2.position.y - 0.f, LightData2.position.z), lightCentre2, glm::normalize(glm::vec3(0.f, 1.0f, 0.f)));

	glUniformMatrix4fv(shader_view_matrix_loc, 1, GL_FALSE, glm::value_ptr(light_view_matrix2));
	glUniformMatrix4fv(shader_projection_matrix_loc, 1, GL_FALSE, glm::value_ptr(shadow_projection_matrix2));

	//DRAW ALL OBJECTS
	//DrawObjectsForShadowPass(objects, shader_model_matrix_loc);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//MAIN PASS
	
	//-------------
	//--  Light  --

	
	// Update the data of the light
	
	glBindBuffer(GL_UNIFORM_BUFFER, LightData_ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Light), &LightData);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, LightData_ubo);
	
	glBindBuffer(GL_UNIFORM_BUFFER, LightData2_ubo);
	glBufferSubData( GL_UNIFORM_BUFFER, 0, sizeof(Light), &LightData2);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, 2, LightData2_ubo);
	
	//--------------
	//--  Camera  --

	// Camera matrices and eye position
	
	CameraData.projection_matrix = shadow_projection_matrix; glm::perspective(glm::radians(45.0f), float(win_width) / float(win_height), 0.1f, 500.0f);
	CameraData.projection_matrix =  glm::perspective(glm::radians(45.0f), float(win_width) / float(win_height), 0.1f, 500.0f);

	CameraData.view_matrix = light_view_matrix; glm::lookAt(my_camera.GetEyePosition(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 100.0f, 0.0f)); // light_view_matrix2;
	CameraData.view_matrix =  glm::lookAt(my_camera.GetEyePosition(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 100.0f, 0.0f)); // light_view_matrix2;

	CameraData.eye_position = my_camera.GetEyePosition();
	
	// Update the data of the camera
	glBindBuffer(GL_UNIFORM_BUFFER, CameraData_ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Camera), &CameraData);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, 1, CameraData_ubo);

	//-------------
	//--  Cubes  --

	// Use the cube shader program
	glUseProgram(cube_program);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowFramebuffer.depthTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowFramebuffer2.depthTexture);
	glUniformMatrix4fv(cube_shadow_proj_matrix_loc, 1, GL_FALSE, glm::value_ptr(shadow_projection_matrix));
	glUniformMatrix4fv(cube_light_view_matrix_loc, 1, GL_FALSE, glm::value_ptr(light_view_matrix));

	glUniformMatrix4fv(cube_shadow_proj_matrix_loc2, 1, GL_FALSE, glm::value_ptr(shadow_projection_matrix2));
	glUniformMatrix4fv(cube_light_view_matrix_loc2, 1, GL_FALSE, glm::value_ptr(light_view_matrix2));

	// Set model's material and texture
	glUniform3f(cube_material_specular_color_loc, 1.0f, 1.0f, 1.0f);
	glUniform1f(cube_material_shininess_loc, 200.0f);
		
	//DRAW ALL OBJECTS
	DrawObjects(objects);
			
	//-------------------------------------
	//--  Reset the VAO and the program  --

	glBindVertexArray(0);
	glUseProgram(0);
	// Swaps the front and back buffer (double-buffering)
	glutSwapBuffers();
}

float getAngleForClockH()
{
	time_t t = time(0);   
	struct tm * now = new tm();
	__time64_t long_time;
	_time64(&long_time);
	localtime_s(now,&long_time);
	int hour;
	if (now->tm_hour > 12)
	{
		hour = now->tm_hour - 12;
	}
	return -(hour * 30 + now->tm_min /2);
}

float getAngleForClockM()
{
	time_t t = time(0);
	struct tm * now = new tm();
	
	__time64_t long_time;
	_time64(&long_time);
	localtime_s(now, &long_time);
	return -(now->tm_min * 6) ;
}
float getAngleForMetronome(float time)
{
	time = (int)time % 120;
	if (time <= 30)
	{
		return time;
	}
	else
		if (time <= 60)
		{
			return 60 - time;
		}
		else
			if (time <= 90)
			{
				return -(time - 60);
			}
			else 
			{
				return time - 120;
			}
}
// Called when the window changes its size
void reshape(int width, int height)
{
	win_width = width;
	win_height = height;

	glViewport(0, 0, win_width, win_height);
}

// Callback function to be called when we make an error in OpenGL
void GLAPIENTRY simple_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
	switch (type)			// Report only some errors
	{
	case GL_DEBUG_TYPE_ERROR:
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		cout << message << endl;		// Put the breakpoint here
		return;
	default:
		return;
	}
}

// Simple timer function for animations
void timer(int)
{
	// Add 20 ms to the application time, register the timer to be called again in 20 ms, and rerender the scene
	app_time_s += 0.020f;
	glutTimerFunc(20, timer, 0);
	glutPostRedisplay();
}

int main(int argc, char ** argv)
{
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);		// Return from the main loop

																						// Set OpenGL Context parameters
	glutInitContextVersion(3, 3);				// Use OpenGL 3.3
	glutInitContextProfile(GLUT_CORE_PROFILE);	// Use OpenGL core profile
	glutInitContextFlags(GLUT_DEBUG);			// Use OpenGL debug context

												// Initialize window
	glutInitWindowSize(win_width, win_height);
	glutCreateWindow("PV112 - cv6");

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	// Initialize DevIL library
	InitDevIL();

	// Set the debug callback
	SetDebugCallback(simple_debug_callback);

	// Initialize out OpenGL stuff
	init();

	// Register callbacks
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key_pressed);
	glutTimerFunc(20, timer, 0);
	glutMouseFunc(mouse_bottom_changed);
	glutMotionFunc(mouse_moved);

	// Run the main loop
	glutMainLoop();

	return 0;
}