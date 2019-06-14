#include "InputHandler.h"
#include "View.h"

bool InputHandler::m_pressedKeys[348];
bool InputHandler::m_triggeredKeys[348];
bool InputHandler::m_repeatedKeys[348];

double InputHandler::mX = 0.0;
double InputHandler::mY = 0.0;
double InputHandler::dX = 0.0;
double InputHandler::dY = 0.0;
double InputHandler::last_x = 0.0;
double InputHandler::last_y = 0.0;
double InputHandler::dScroll = 0.0;

bool InputHandler::m_bMouseEnabled = true;
bool InputHandler::m_bKeyboardEnabled = true;
double InputHandler::m_dClickDelay = 0.0;
double InputHandler::m_dPressDelay = 0.0;

InputHandler::InputHandler()
{
	std::cout << "InputHandler Created" << std::endl;

	for (int i = 0; i < GLFW_KEY_LAST; i++)
	{
		m_pressedKeys[i] = false;
		m_triggeredKeys[i] = false;
		m_repeatedKeys[i] = false;
	}

	ImGuiIO& io = ImGui::GetIO();
	io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;                         // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
	io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
	io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
	io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
	io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
	io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
	io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
	io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
	io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
	io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
	io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
	io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
	io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
	io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
	io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;
}

void InputHandler::Key_Callback(int key, int scancode, int action, int mods)
{
	if (m_bKeyboardEnabled)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (action == GLFW_PRESS)
		{
			{
				io.KeysDown[key] = true;
				PressKey(key);
			}
		}
		else if (action == GLFW_RELEASE)
		{
			{
				io.KeysDown[key] = false;
				PressKey(key, false);
			}
		}
		else if (action == GLFW_REPEAT)
		{
			{
				PressKey(key);
			}
		}
		io.KeyCtrl = m_pressedKeys[GLFW_KEY_LEFT_CONTROL] || m_pressedKeys[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = m_pressedKeys[GLFW_KEY_LEFT_SHIFT] || m_pressedKeys[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = m_pressedKeys[GLFW_KEY_LEFT_ALT] || m_pressedKeys[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = m_pressedKeys[GLFW_KEY_LEFT_SUPER] || m_pressedKeys[GLFW_KEY_RIGHT_SUPER];
	}
}

void InputHandler::Mouse_Callback(int button, int action, int mods)
{
	if (m_bMouseEnabled)
	{
		if (action == GLFW_PRESS)
		{
			PressKey(button);
		}
		else if (action == GLFW_RELEASE)
		{
			PressKey(button, false);
		}
		else if (action == GLFW_REPEAT)
		{
			PressKey(button);
		}
	}
}

void InputHandler::Scroll_Callback(double /*xoffset*/, double yoffset)
{
	dScroll += (float)yoffset; // Use fractional mouse wheel.
}

void InputHandler::Char_Callback(unsigned char c)
{
	
}

void InputHandler::PressKey(int key, bool status)
{
	if (status)
	{
		// Check if key is pressed
		if (m_pressedKeys[key] == false)
		{
			// Set to true
			m_pressedKeys[key] = true;
			if (m_triggeredKeys[key] == false)
			{
				m_triggeredKeys[key] = true;
				//std::cout << key << " has been triggered" << std::endl;
			}
			else
			{
				m_triggeredKeys[key] = false;
				//std::cout << key << " has been un-triggered" << std::endl;
			}
		}
		else
		{
			// Key is still being pressed
			m_repeatedKeys[key] = true;
			//std::cout << key << " is being pressed repeatedly" << std::endl;
		}
	}
	else
	{
		// Key is not pressed anymore
		m_pressedKeys[key] = false;
		m_repeatedKeys[key] = false;
		//std::cout << "Setting " << key << " to false" << std::endl;
	}
}

void InputHandler::KeyboardUpdate(View * theView, double dt)
{
	m_dPressDelay += dt;
}

void InputHandler::MouseUpdate(View * theView, double dt)
{
	if (m_dClickDelay > 0.0)
	{
		m_dClickDelay -= dt;
	}

	glfwGetCursorPos(theView->getWindow(), &mX, &mY);

	float screenXmid = (float)theView->getWindowWidth() * 0.5f;
	float screenYmid = (float)theView->getWindowHeight() * 0.5f;

	dX = (screenXmid - mX);
	dY = (screenYmid - (ceil)(mY));

	dX *= 0.15;
	dY *= 0.15;

	if (m_bMouseEnabled == false)
	{
		glfwSetInputMode(theView->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(theView->getWindow(), screenXmid, screenYmid);
	}
	else
	{
		glfwSetInputMode(theView->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

bool InputHandler::IsKeyPressed(int key)
{
	return m_pressedKeys[key];
}

bool InputHandler::IsKeyTriggered(int key)
{
	return m_triggeredKeys[key];
}

bool InputHandler::IsKeyRepeating(int key)
{
	return m_repeatedKeys[key];
}

bool InputHandler::isKeyboardEnabled()
{
	return m_bKeyboardEnabled;
}

void InputHandler::setKeyboardEnabled(bool status)
{
	m_bKeyboardEnabled = status;
}

void InputHandler::setMouseX(double newX)
{
	mX = newX;
}

void InputHandler::setMouseY(double newY)
{
	mY = newY;
}

void InputHandler::setDeltaScroll(double newDs)
{
	dScroll = newDs;
}

void InputHandler::setClickDelay(double delay)
{
	m_dClickDelay = delay;
}

void InputHandler::setPressDelay(double delay)
{
	m_dPressDelay = delay;
}

double InputHandler::getDeltaX()
{
	return dX;
}

double InputHandler::getDeltaY()
{
	return dY;
}

double InputHandler::getDeltaScroll()
{
	return dScroll;
}

bool InputHandler::isMouseEnabled()
{
	return m_bMouseEnabled;
}

void InputHandler::setMouseEnabled(bool status)
{
	m_bMouseEnabled = status;
}

double InputHandler::getMouseX()
{
	return mX;
}

double InputHandler::getMouseY()
{
	return mY;
}

double InputHandler::getClickDelay()
{
	return m_dClickDelay;
}

double InputHandler::getPressDelay()
{
	return m_dPressDelay;
}

void InputHandler::resetMousePosition(View * theView)
{
	glfwSetCursorPos(theView->getWindow(), theView->getWindowWidth() * 0.5, theView->getWindowHeight() * 0.5);
}