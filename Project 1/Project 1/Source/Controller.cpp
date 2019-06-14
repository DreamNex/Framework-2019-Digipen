#include "Controller.h"

#include "StateSplash.h"

Controller::Controller(Model * theModel, View * theView)
: theModel(theModel)
, theView(theView)
{
	std::cout << "Controller Created" << std::endl;
}

Controller::~Controller()
{
	this->theModel = nullptr;
	this->theView = nullptr;

	delete m_cStateHandler;
	m_cStateHandler = nullptr;
}

void Controller::RunGameLoop()
{
	bool LoopTheGame = true;

	// Setup Timer
	using clock = std::chrono::high_resolution_clock;
	std::chrono::nanoseconds lag(0ns);
	auto time_start = clock::now();

	m_cStateHandler = new StateHandler();
	m_cStateHandler->Init();
	
	if (theView->CreateGLWindow("Project 1", 1280, 720, 32))
	{
		std::cout << "OpenGL Window Created" << std::endl;
	}
	else
	{
		std::cout << "Failed to create OpenGL Context" << std::endl;
	}

	InputHandler::getInstance(); // Create InputHandler Singleton

	// #TESTING
	bool show_test_window = true;
	bool show_another_window = true;

	// Setting the first state to render to be the Splash Screen
	m_cStateHandler->ChangeState(new StateSplash("Splash State", theView));

	while(LoopTheGame)
	{
		auto delta_time = clock::now() - time_start;
		time_start = clock::now();
		lag += std::chrono::duration_cast<std::chrono::nanoseconds>(delta_time);

		// Main Thread
		InputHandler::MouseUpdate(theView, (double) delta_time.count());
		InputHandler::KeyboardUpdate(theView, (double)delta_time.count());
		m_cStateHandler->HandleEvents();

		//ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		while (lag >= timestep)
		{
			lag -= timestep;
			m_cStateHandler->Update((double)delta_time.count());
		}

		// Secondary Thread
		// ###Loading Objects?

		// TESTING
		ImGui::ShowDemoWindow();

		if (ImGui::Begin("Information", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground))
		{
			ImGui::SetWindowPos(ImVec2(-5, theView->getWindowHeight() - (ImGui::GetWindowHeight() * 0.75f)), true);
			ImGui::Text("FPS : %f", ImGui::GetIO().Framerate);
			ImGui::End();
		}
		else
		{
			ImGui::End();
		}

		// Rendering
		m_cStateHandler->Draw();
		theView->RenderImGui();
		theView->SwapBuffers();

		// Time Between Frames
		auto alpha = (float)lag.count() / timestep.count();

		// For Quitting Purposes (Temporarily)
		if (InputHandler::IsKeyPressed(GLFW_KEY_ESCAPE) || glfwWindowShouldClose(theView->getWindow()))
		{
			LoopTheGame = false;
		}
	}
}