#include "Controller.h"

#include "StateSplash.h"
#include "StateTest.h"

using namespace std::chrono;

Controller::Controller(Model * theModel, View * theView)
: theModel(theModel)
, theView(theView)
{
	std::cout << "Controller Created" << std::endl;
	m_cStateHandler = nullptr;
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
	std::chrono::nanoseconds lag(0ns);
	auto time_start = high_resolution_clock::now();

	m_cStateHandler = new StateHandler();
	m_cStateHandler->Init();
	
	if (theView->CreateGLWindow("Project 1", 1280, 720, 32))
	{
		std::cout << "OpenGL Window Created" << std::endl;
		theModel->Init(m_cStateHandler);
	}
	else
	{
		std::cout << "Failed to create OpenGL Context" << std::endl;
	}

	InputHandler::getInstance(); // Create InputHandler Singleton

	// #TESTING
	bool show_test_window = true;
	bool show_another_window = true;

	// Setting the first state to render to be the Splash Screen #TODO
	m_cStateHandler->ChangeState(new StateTest("Test State", theView, theModel));

	while(LoopTheGame)
	{
		auto current_time = high_resolution_clock::now();
		auto delta_time = current_time - time_start;
		time_start = current_time;
		//duration<double> delta_time = high_resolution_clock::now() - time_start;
		//time_start = high_resolution_clock::now();
		lag += duration_cast<std::chrono::nanoseconds>(delta_time);

		// Main Thread
		InputHandler::MouseUpdate(theView, (double)delta_time.count());
		InputHandler::KeyboardUpdate(theView, (double)delta_time.count());
		m_cStateHandler->HandleEvents();

		//ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		while (lag >= timestep)
		{
			lag -= timestep;
			// Fixed Timestep
			m_cStateHandler->Update(0.16);
		}

		// Secondary Thread
		// ###Loading Objects?

		// TESTING
		ImGui::ShowDemoWindow();

		// Rendering
		m_cStateHandler->Draw();
		theView->RenderDebugInformation();
		theView->RenderFPS();
		theView->RenderImGui();
		theView->SwapBuffers();

		// Time Between Frames
		auto alpha = (float)lag.count() / timestep.count();
		//#TODO INTERPOLATION BETWEEN STATES

		// For Quitting Purposes (Temporarily)
		if (InputHandler::IsKeyPressed(GLFW_KEY_ESCAPE) || glfwWindowShouldClose(theView->getWindow()))
		{
			LoopTheGame = false;
		}
	}
}