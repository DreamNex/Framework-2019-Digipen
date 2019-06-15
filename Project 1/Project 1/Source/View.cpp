#include "View.h"
#include "State.h"
#include "Camera.h"

#include <fstream>
#include <algorithm>

View::View(Model * theModel)
	: theModel(theModel)
{
	std::cout << "View Created" << std::endl;
	m_mAxes = nullptr;
	m_cCamera = nullptr;
}

View::~View()
{
	if (m_cCamera != nullptr)
	{
		delete m_cCamera;
		m_cCamera = nullptr;
	}
	if (m_mAxes != nullptr)
	{
		delete m_mAxes;
		m_mAxes = nullptr;
	}
	glfwTerminate();
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

BOOL View::CreateGLWindow(const char * title, int width, int height, int bits)
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	const char* glsl_version = "#version 450";
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5); //Request a specific OpenGL version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

	m_window = glfwCreateWindow(width, height, title, NULL, NULL);
	m_iWindow_Width = width;
	m_iWindow_Height = height;
	m_sWindow_Title = title;

	if (!m_window)
	{
		fprintf(stderr, "Failed to create OpenGL Window");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(m_window);

	// For setting keyboard callback
	auto InputHandler_Key_CallBack = [](GLFWwindow * window, int key, int scancode, int action, int mods)
	{
		static_cast<InputHandler*>(glfwGetWindowUserPointer(window))->Key_Callback(key, scancode, action, mods);
	};

	// For setting mouse callback
	auto InputHandler_Mouse_CallBack = [](GLFWwindow * window, int button, int action, int mods)
	{
		static_cast<InputHandler*>(glfwGetWindowUserPointer(window))->Mouse_Callback(button, action, mods);
	};

	// For setting mouse scroll callback
	auto InputHandler_Scroll_CallBack = [](GLFWwindow * window, double xOffset, double yOffset)
	{
		static_cast<InputHandler*>(glfwGetWindowUserPointer(window))->Scroll_Callback(xOffset, yOffset);
	};

	auto InputHandler_Char_CallBack = [](GLFWwindow * window, unsigned int c)
	{
		static_cast<InputHandler*>(glfwGetWindowUserPointer(window))->Char_Callback(c);
	};

	glfwSwapInterval(1); // Vsync Enabled
	gl3wInit();

	// Setup ImGui Context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// Setup ImGui binding
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);


	// *TODO* - FIX INPUTHANDLER CHAR/SCROLL CALLBACKS
	// Sets the Callbacks
	glfwSetKeyCallback(m_window, InputHandler_Key_CallBack);
	glfwSetMouseButtonCallback(m_window, InputHandler_Mouse_CallBack);
	glfwSetWindowFocusCallback(m_window, View::Focus_Callback);
	//glfwSetCharCallback(m_window, InputHandler_Char_CallBack);
	//glfwSetScrollCallback(m_window, InputHandler_Scroll_CallBack);

	// Setup Viewport
	glGetIntegerv(GL_VIEWPORT, m_viewPort);
	m_iWindow_Width = width;
	m_iWindow_Height = height;
	glViewport(0, 0, m_iWindow_Width, m_iWindow_Height);

	if (!InitGL())
	{
		return FALSE;
	}

	// Move the screen to the middle 
	glfwSetWindowPos(m_window, ((float)(glfwGetVideoMode(glfwGetPrimaryMonitor())->width) * 0.5f) - ((float)(this->m_iWindow_Width) * 0.5f), ((float)(glfwGetVideoMode(glfwGetPrimaryMonitor())->height) * 0.5f) - ((float)(this->m_iWindow_Height) * 0.5f));
	// Move cursor to middle
	glfwSetCursorPos(this->getWindow(), this->getWindowWidth() / 2, this->getWindowHeight() / 2);

	m_cCamera = new Camera(this);
	//m_mAxes = MeshBuilder::GenerateAxis("Axis");
	m_mAxes = MeshBuilder::GenerateCube("Cube", Mesh::Color(0.1f,0.2f,0.3f), 10.f);

	return TRUE;
}

void View::Update(double dt)
{
	if (InputHandler::getInstance().IsKeyPressed(GLFW_KEY_F1))
	{
		if (InputHandler::getInstance().IsKeyTriggered(GLFW_KEY_F1))
		{
			glDisable(GL_CULL_FACE);
		}
		else
		{
			glEnable(GL_CULL_FACE);
		}
	}

	if (InputHandler::getInstance().IsKeyPressed(GLFW_KEY_F2))
	{
		if (InputHandler::getInstance().IsKeyTriggered(GLFW_KEY_F2))
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

	}

	this->m_cCamera->Update(dt);

	this->OnResizeWindow();
}

void View::Focus_Callback(GLFWwindow* window, int focused)
{
	if (focused)
	{
		InputHandler::setMouseEnabled(true);
		InputHandler::setKeyboardEnabled(true);
	}
	else
	{
		InputHandler::setMouseEnabled(false);
		InputHandler::setKeyboardEnabled(false);
	}
}

BOOL View::InitGL()
{
	//glShadeModel(GL_SMOOTH);
	glClearColor(0.f, 0.f, 0.0f, 1.f); //Initialize Background Colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST); //Enable Depth Test
	glDepthFunc(GL_LEQUAL); //If the image is closer to camera, allow it to be ontop
	glEnable(GL_CULL_FACE); //Allow culling of faces
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Allows the interior of polygon to be filled
	glEnable(GL_BLEND); //Allow blending of textures with alpha
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Alpha stuff
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //Nicest Perspective that can be calculated

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	if (!InitProgramInfo())
	{
		std::cout << "Failed to Load Program Parameters" << std::endl;
	}

	/*if (!InitLightsInfo())
	{
		std::cout << "Failed to Load Lights" << std::endl;
	}*/

	return TRUE;
}

BOOL View::InitProgramInfo()
{
	m_programID = LoadShaders("Shaders//Test.vertexshader", "Shaders//Test.fragmentshader");
	//Model View
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	//Material
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	//Lights
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	//Color
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[0]");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture[0]");
	m_parameters[U_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_programID, "colorTextureEnabled[1]");
	m_parameters[U_COLOR_TEXTURE1] = glGetUniformLocation(m_programID, "colorTexture[1]");
	//Text
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	//Fog #TODO
	m_parameters[U_FOG_COLOR] = glGetUniformLocation(m_programID, "fog[0].color.r");
	m_parameters[U_FOG_START] = glGetUniformLocation(m_programID, "fog[0].start");
	m_parameters[U_FOG_END] = glGetUniformLocation(m_programID, "fog[0].end");
	m_parameters[U_FOG_DENSITY] = glGetUniformLocation(m_programID, "fog[0].density");
	m_parameters[U_FOG_TYPE] = glGetUniformLocation(m_programID, "fog[0].type");
	m_parameters[U_FOG_ENABLE] = glGetUniformLocation(m_programID, "fogEnabled");
	//Alpha
	m_parameters[U_OBJECT_ALPHA] = glGetUniformLocation(m_programID, "alpha");

	glUseProgram(m_programID); //Use our own shader

	std::cout << "\nProgram Parameters Initialized" << std::endl;

	return TRUE;
}

void View::SwapBuffers()
{
	glfwSwapBuffers(m_window);
}

// RENDERING FUNCTIONS START HERE ----------------------------------------------------------------

void View::RenderImGui()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void View::RenderDebugInformation()
{
	// Using ImGui
	if (ImGui::Begin("Debug Information", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoBackground))
	{
		ImGui::SetWindowPos(ImVec2(-5, -5), true);
		ImGui::Text("Current State : %s", theModel->GetStateHandler()->GetCurrentRunningState()->toString().c_str());
		ImGui::Text("Camera Pos %f, %f, %f", this->m_cCamera->m_v3CameraPos.x, this->m_cCamera->m_v3CameraPos.y, this->m_cCamera->m_v3CameraPos.z);
		ImGui::Text("Camera Dir %f, %f, %f", this->m_cCamera->m_v3CameraDirection.x, this->m_cCamera->m_v3CameraDirection.y, this->m_cCamera->m_v3CameraDirection.z);
		ImGui::Text("Camera Right %f, %f, %f", this->m_cCamera->m_v3CameraRight.x, this->m_cCamera->m_v3CameraRight.y, this->m_cCamera->m_v3CameraRight.z);
		ImGui::Text("Camera Up %f, %f, %f", this->m_cCamera->m_v3CameraUp.x, this->m_cCamera->m_v3CameraUp.y, this->m_cCamera->m_v3CameraUp.z);
		ImGui::Text("Camera Pitch | Yaw | Roll %f, %f, %f", this->m_cCamera->GetPitch(), this->m_cCamera->GetYaw(), this->m_cCamera->GetRoll());
		ImGui::Text("Mouse Delta %f, %f", InputHandler::getDeltaX(), InputHandler::getDeltaY());
		ImGui::End();
	}
	else
	{
		ImGui::End();
	}
}

void View::RenderFPS()
{
	// Using ImGui
	if (ImGui::Begin("FPS", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoBackground))
	{
		ImGui::SetWindowPos(ImVec2(-5, this->getWindowHeight() - (ImGui::GetWindowHeight() * 0.75f)), true); // Bottom Left
		ImGui::Text("FPS : %f", ImGui::GetIO().Framerate);
		ImGui::End();
	}
	else
	{
		ImGui::End();
	}
}

void View::RenderAxis()
{
	RenderMesh(m_mAxes, false);
}

void View::RenderMesh(Mesh* mesh, bool enableLight)
{
	if (mesh == NULL)
		return;


	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	
	glm::mat4 MVP2 = m_cCamera->m_m4Projection * m_cCamera->m_m4View * m_cCamera->m_m4Model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP2[0][0]);
	//glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	// Alpha
	glUniform1f(m_parameters[U_OBJECT_ALPHA], mesh->alpha);

	if (enableLight && LIGHTS_ENABLED)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		if (mesh->textureArray[i] > 0)
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
		}
		else
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
		}

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
		//
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//
		glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
	}

	mesh->Render();

	glBindTexture(GL_TEXTURE_2D, 0);
}


// END OF RENDERING FUNCTIONS --------------------------------------------------------------------

GLint View::LoadShaders(const char* vertex_file_path, const char* fragment_file_path)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}



	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s ", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("\nCompiling shader : %s", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("\nLinking program");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

int View::getWindowHeight()
{
	return this->m_iWindow_Height;
}

int View::getWindowWidth()
{
	return this->m_iWindow_Width;
}

GLFWwindow * View::getWindow()
{
	return this->m_window;
}

void View::OnResizeWindow()
{
	glfwGetFramebufferSize(m_window, &m_iWindow_Width, &m_iWindow_Height);
	glViewport(0, 0, m_iWindow_Width, m_iWindow_Height);
}

void View::LoadOrthoCamera()
{
	this->m_eViewState = VIEW_ORTHO;

	m_cCamera->m_m4Projection = glm::ortho(0.f, (float)m_viewPort[VIEWPORT_WIDTH], 0.f, (float)m_viewPort[VIEWPORT_HEIGHT], -10.f, 100.f);
	ProjectionM4 = glm::ortho(0.f, (float)m_viewPort[VIEWPORT_WIDTH], 0.f, (float)m_viewPort[VIEWPORT_HEIGHT], -10.f, 100.f);

	Mtx44 Projection;
	Projection.SetToOrtho(0, (float)m_viewPort[VIEWPORT_WIDTH], 0, (float)m_viewPort[VIEWPORT_HEIGHT], -10, 100);
	projectionStack.LoadIdentity();
	projectionStack.LoadMatrix(Projection);
}

void View::LoadPerspectiveCamera(double fov)
{
	this->m_eViewState = VIEW_PERSPECTIVE;

	m_cCamera->m_m4Projection = glm::perspective(glm::radians(45.0f), (float)m_viewPort[VIEWPORT_WIDTH] / (float)m_viewPort[VIEWPORT_HEIGHT], 0.1f, 1000.f);

	ProjectionM4 = glm::perspective(glm::radians(45.0f), (float)m_viewPort[VIEWPORT_WIDTH] / (float)m_viewPort[VIEWPORT_HEIGHT], 0.1f, 1000.f);

	Mtx44 Projection;
	Projection.SetToPerspective(fov, (float)m_viewPort[VIEWPORT_WIDTH] / (float)m_viewPort[VIEWPORT_HEIGHT], 0.1, 15000); // **Note : 10000 should be the standard.
	projectionStack.LoadIdentity();
	projectionStack.LoadMatrix(Projection);
}