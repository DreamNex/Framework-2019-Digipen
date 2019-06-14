#include "View.h"

View::View(Model * theModel)
	: theModel(theModel)
{
	std::cout << "View Created" << std::endl;
}

View::~View()
{
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

	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
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


	return TRUE;
}

void View::Update(double dt)
{
	this->OnResizeWindow();
}

BOOL View::InitGL()
{
	//glShadeModel(GL_SMOOTH);
	glClearColor(0.45f, 0.55f, 0.60f, 1.f); //Initialize Background Colour
	glEnable(GL_DEPTH_TEST); //Enable Depth Test
	glDepthFunc(GL_LEQUAL); //If the image is closer to camera, allow it to be ontop
	glEnable(GL_CULL_FACE); //Allow culling of faces
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Allows the interior of polygon to be filled
	glEnable(GL_BLEND); //Allow blending of textures with alpha
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Alpha stuff
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //Nicest Perspective that can be calculated

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	/*if (!InitProgramInfo())
	{
		std::cout << "Failed to Load Program Parameters" << std::endl;
	}
	if (!InitLightsInfo())
	{
		std::cout << "Failed to Load Lights" << std::endl;
	}*/

	this->theModel->Init();

	return TRUE;
}

void View::SwapBuffers()
{
	glfwSwapBuffers(m_window);
}

// RENDERING FUNCTIONS START HERE ----------------------------------------------------------------

void View::RenderImGui()
{
	glfwGetFramebufferSize(m_window, &m_iWindow_Width, &m_iWindow_Height);
	glViewport(0, 0, m_iWindow_Width, m_iWindow_Height);
	glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void View::RenderFPS()
{
	// Using ImGui
}


// END OF RENDERING FUNCTIONS --------------------------------------------------------------------

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
	glGetIntegerv(GL_VIEWPORT, m_viewPort);
	m_iWindow_Width = m_viewPort[VIEWPORT_WIDTH];
	m_iWindow_Height = m_viewPort[VIEWPORT_HEIGHT];
}