#ifndef __VIEW_H__
#define __VIEW_H__

#include <stdio.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN // For Winsock2

#include <windows.h> // For Windows

#include <GL/gl3w.h> // Used to Access OpenGL Functions
#include <glfw3.h>

#include <iostream>
#include <string>

#include "Model.h"
#include "Math\MatrixStack.h"
#include "InputHandler.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// Include all GLM core / GLSL features
#include <glm/glm.hpp> // vec2, vec3, mat4, radians

// Include all GLM extensions
#include <glm/ext.hpp> // perspective, translate, rotate

class Camera;

#define LIGHTS_ENABLED true

enum VIEW_STATE
{
	VIEW_ORTHO = 0,
	VIEW_PERSPECTIVE,
};

enum VIEWPORT_INFO
{
	VIEWPORT_WIDTH = 2,
	VIEWPORT_HEIGHT,
};

enum UNIFORM_TYPE
{
	U_MVP = 0,
	U_MODELVIEW,
	U_MODELVIEW_INVERSE_TRANSPOSE,
	U_MATERIAL_AMBIENT,
	U_MATERIAL_DIFFUSE,
	U_MATERIAL_SPECULAR,
	U_MATERIAL_SHININESS,
	U_LIGHTENABLED,
	U_NUMLIGHTS,
	U_LIGHT0_TYPE,
	U_LIGHT0_POSITION,
	U_LIGHT0_COLOR,
	U_LIGHT0_POWER,
	U_LIGHT0_KC,
	U_LIGHT0_KL,
	U_LIGHT0_KQ,
	U_LIGHT0_SPOTDIRECTION,
	U_LIGHT0_COSCUTOFF,
	U_LIGHT0_COSINNER,
	U_LIGHT0_EXPONENT,

	U_COLOR_TEXTURE_ENABLED,
	U_COLOR_TEXTURE_ENABLED1,
	U_COLOR_TEXTURE,
	U_COLOR_TEXTURE1,

	U_TEXT_ENABLED,
	U_TEXT_COLOR,

	U_FOG_COLOR,
	U_FOG_START,
	U_FOG_END,
	U_FOG_DENSITY,
	U_FOG_TYPE,
	U_FOG_ENABLE,

	U_OBJECT_ALPHA,

	U_TOTAL,
};

class View
{
public:
	View(Model * theModel);
	virtual ~View();

	BOOL CreateGLWindow(const char * title, int width, int height, int bits);
	BOOL InitGL();
	BOOL InitProgramInfo();

	void SwapBuffers();

	virtual void Update(double dt);

	// Rendering Functions
	void RenderImGui();
	void RenderDebugInformation();
	void RenderFPS();
	void RenderAxis();
	void RenderMesh(Mesh* mesh, bool enableLight);

	// Generic Functions
	int getWindowWidth();
	int getWindowHeight();
	GLFWwindow * getWindow();

	// Camera Functions
	void OnResizeWindow();
	void LoadOrthoCamera();
	void LoadPerspectiveCamera(double fov);
	VIEW_STATE GetViewState() { return m_eViewState; }

	// Testing
	Mesh* m_mAxes;
	MS projectionStack;
	MS viewStack;
	MS modelStack;
	glm::mat4 ProjectionM4;
	glm::mat4 ViewM4;
	glm::mat4 ModelM4;

protected:
	GLint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);

	int m_iWindow_Width;
	int m_iWindow_Height;
	std::string m_sWindow_Title;

	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

private:
	GLFWwindow * m_window;
	GLint m_viewPort[4];

	Model * theModel;

	VIEW_STATE m_eViewState;

	Camera * m_cCamera;
};


#endif