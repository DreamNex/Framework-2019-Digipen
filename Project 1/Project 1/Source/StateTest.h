#ifndef STATE_TEST_H
#define STATE_TEST_H

#include "State.h"
//#include "MeshBuilder.h"

class StateTest : public State
{
public:
	StateTest(std::string name, View * theView) : State(name, theView)
	{
		this->theView = theView;
	}

	~StateTest();

	State* getInstance();
	void Init();
	void Update(StateHandler * stateHandler, double dt);
	void Cleanup();
	void Pause();
	void Resume();
	void HandleEvents(StateHandler * stateHandler);
	void Draw(StateHandler * stateHandler);

	// Rendering Functions
	void RenderBackground();

private:
	View * theView;
	//std::vector<Mesh*> m_meshList;
};

#endif