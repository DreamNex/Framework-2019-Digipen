#ifndef CONTROLLER_H
#define CONTROLLER_H

// To Check for memory leaks
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Model.h"
#include "View.h"

// Timer Optimization
#include <chrono>
using namespace std::chrono_literals;
constexpr std::chrono::nanoseconds timestep(16ms); // 60fps

// Singleton Handlers
#include "InputHandler.h"
#include "StateHandler.h"

class Controller
{
public:
	Controller(Model * theModel, View * theView);
	virtual ~Controller(void);

	void RunGameLoop();

	Model * theModel;
	View * theView;
private:
	StateHandler * m_cStateHandler;
};

#endif
