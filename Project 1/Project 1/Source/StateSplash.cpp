#include "StateSplash.h"
#include "View.h"

StateSplash::~StateSplash()
{

}

State * StateSplash::getInstance()
{
	return this;
}

void StateSplash::Init()
{
	// Splash Screen is 2D
	theView->LoadOrthoCamera();

	m_bStartFadeOut = false;
	m_dFadeDelay = 0.0;
}

void StateSplash::Update(StateHandler * stateHandler, double dt)
{
	m_dFadeDelay += dt;

	if (m_dFadeDelay > 1.5)
	{
		m_bStartFadeOut = true;
	}

	if (m_bStartFadeOut)
	{
		FadeOutEffect(dt, stateHandler);
	}

	theView->Update(dt);
}

void StateSplash::HandleEvents(StateHandler * stateHandler)
{

}

void StateSplash::Cleanup()
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

void StateSplash::Pause()
{

}

void StateSplash::Resume()
{

}

void StateSplash::Draw(StateHandler * stateHandler)
{
}

void StateSplash::RenderBackground()
{
}

void StateSplash::FadeOutEffect(double dt, StateHandler * stateHandler)
{
	/*for (Mesh * mesh : m_meshList)
	{
		float newAlpha = Math::SmoothDamp(mesh->alpha, -1.f, 10.f, 0.001f, 10.f, (float)dt);
		mesh->alpha = newAlpha;
	}

	if (m_meshList[0]->alpha < 0.f)
	{
		stateHandler->ChangeState(new StateMenu("Menu State", theView, true));
	}*/
}