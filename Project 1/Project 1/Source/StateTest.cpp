#include "StateTest.h"
#include "View.h"

StateTest::~StateTest()
{

}

State * StateTest::getInstance()
{
	return this;
}

void StateTest::Init()
{
	//theView->LoadOrthoCamera();
	theView->LoadPerspectiveCamera(45.0f);
}

void StateTest::Update(StateHandler * stateHandler, double dt)
{
	theView->Update(dt);
}

void StateTest::HandleEvents(StateHandler * stateHandler)
{

}

void StateTest::Cleanup()
{
	/*if (!m_meshList.empty())
	{
		for (int i = 0; i < m_meshList.size(); i++)
		{
			delete m_meshList[i];
		}
	}
	m_meshList.clear();
	m_meshList.~vector();*/
}

void StateTest::Pause()
{

}

void StateTest::Resume()
{

}

void StateTest::Draw(StateHandler * stateHandler)
{
	theView->RenderAxis();
}

void StateTest::RenderBackground()
{

}
