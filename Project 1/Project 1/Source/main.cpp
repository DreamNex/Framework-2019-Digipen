// To Check for memory leaks
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Model.h"
#include "View.h"
#include "Controller.h"

int main(int, char**)
{
	bool LoopTheGame = true;

	Model * theModel = new Model();
	View * theView = new View(theModel);
	Controller * theController = new Controller(theModel,theView);


	theController->RunGameLoop();

	if (theController != NULL)
	{
		delete theController;
		theController = NULL;
	}

	if (theView != NULL)
	{
		delete theView;
		theView = NULL;
	}

	if (theModel != NULL)
	{
		delete theModel;
		theModel = NULL;
	}

	
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	delete theView;

	_CrtDumpMemoryLeaks();

	return 0;
}
