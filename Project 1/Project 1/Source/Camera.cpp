#include "Camera.h"
#include "InputHandler.h"

#include <iostream>

#define MOUSE_SENSITIVITY 1.f
#define CAMERA_SENSITIVITY 0.5f

Camera::Camera(View* view)
	: m_cView(view)
	, m_eCameraMode(CAMERA_MODE::CM_FREE_CAMERA)
	, m_m4Model(glm::mat4(1.0f))
	, m_m4View(glm::mat4(1.0f))
	, m_m4Projection(glm::mat4(1.0f))
	, m_v3CameraPos(glm::vec3(0.f, 20.f, 0.f))
	, m_v3CameraTarget(glm::vec3(0.f,0.f,0.f))
	, m_v3CameraUp(glm::vec3(0.f, 1.f, 0.f))
	, m_v3CameraRight(glm::vec3(0.f,0.f,0.f))
	, m_v3CameraDirection(glm::vec3(0.f,0.f, 1.f))
	, m_fPitch(-35.f)
	, m_fYaw(225.f)
	, m_fRoll(0.f)
{
	std::cout << "Camera Created" << std::endl;
	m_m4View = glm::lookAt(m_v3CameraPos, m_v3CameraPos + m_v3CameraDirection, m_v3CameraUp);
}

Camera::~Camera()
{

}

void Camera::Update(double dt)
{
	switch (m_cView->GetViewState())
	{
	case VIEW_STATE::VIEW_ORTHO:
		m_m4View = glm::lookAt(m_v3CameraPos, m_v3CameraPos + m_v3CameraDirection, m_v3CameraUp);
		break;
	case VIEW_STATE::VIEW_PERSPECTIVE:
			m_v3CameraDirection.x = cos(glm::radians(m_fPitch)) * cos(glm::radians(m_fYaw));
			m_v3CameraDirection.y = sin(glm::radians(m_fPitch));
			m_v3CameraDirection.z = cos(glm::radians(m_fPitch)) * sin(glm::radians(m_fYaw));
			m_v3CameraDirection = glm::normalize(m_v3CameraDirection);
			m_m4View = glm::lookAt(m_v3CameraPos, m_v3CameraPos + m_v3CameraDirection, m_v3CameraUp);
		break;
	default:
		break;
	}

	Move();
}

void Camera::Move()
{
	if (InputHandler::IsKeyPressed(GLFW_KEY_W)) // Up
	{
		switch (m_cView->GetViewState())
		{
		case VIEW_STATE::VIEW_ORTHO:
			m_v3CameraPos.y += CAMERA_SENSITIVITY;
			break;
		case VIEW_STATE::VIEW_PERSPECTIVE:
			switch (m_eCameraMode)
			{
			case CM_FREE_CAMERA:
				m_v3CameraPos += m_v3CameraDirection * CAMERA_SENSITIVITY;
				break;
			case CM_FIRST_PERSON:
				m_v3CameraPos.x += m_v3CameraDirection.x * CAMERA_SENSITIVITY;
				m_v3CameraPos.z += m_v3CameraDirection.z * CAMERA_SENSITIVITY;
				break;
			}
			break;
		}
	}
	if (InputHandler::IsKeyPressed(GLFW_KEY_S)) // Down
	{
		switch (m_cView->GetViewState())
		{
		case VIEW_STATE::VIEW_ORTHO:
			m_v3CameraPos.y -= 1.f;
			break;
		case VIEW_STATE::VIEW_PERSPECTIVE:
			switch (m_eCameraMode)
			{
			case CM_FREE_CAMERA:
				m_v3CameraPos -= m_v3CameraDirection * CAMERA_SENSITIVITY;
				break;
			case CM_FIRST_PERSON:
				m_v3CameraPos.x -= m_v3CameraDirection.x * CAMERA_SENSITIVITY;
				m_v3CameraPos.z -= m_v3CameraDirection.z * CAMERA_SENSITIVITY;
				break;
			}
			break;
		}
	}
	if (InputHandler::IsKeyPressed(GLFW_KEY_A)) // Left
	{
		m_v3CameraPos -= glm::normalize(glm::cross(m_v3CameraDirection, m_v3CameraUp) * CAMERA_SENSITIVITY);
	}
	if (InputHandler::IsKeyPressed(GLFW_KEY_D)) // Right
	{
		m_v3CameraPos += glm::normalize(glm::cross(m_v3CameraDirection, m_v3CameraUp) * CAMERA_SENSITIVITY);
	}
	if (InputHandler::IsKeyPressed(GLFW_KEY_Q)) // Rotate Left
	{
		m_fRoll += 1.f;
	}
	if (InputHandler::IsKeyPressed(GLFW_KEY_E)) // Rotate Right
	{
		m_fRoll -= 1.f;
	}
	if (InputHandler::IsKeyPressed(GLFW_KEY_SPACE)) // Rotate Right
	{
		m_v3CameraPos.y += CAMERA_SENSITIVITY;
	}

	switch (m_cView->GetViewState())
	{
	case VIEW_STATE::VIEW_ORTHO:
		break;
	case VIEW_STATE::VIEW_PERSPECTIVE:
		glm::vec2 MouseDelta = glm::vec2(InputHandler::getInstance().getDeltaX(), InputHandler::getInstance().getDeltaY());

		//if (InputHandler::IsKeyPressed(GLFW_MOUSE_BUTTON_1))
		{
			UpdateYaw(-MOUSE_SENSITIVITY * MouseDelta.x);
			UpdatePitch(MOUSE_SENSITIVITY * MouseDelta.y);
		}

		m_v2MousePos = glm::vec2(InputHandler::getInstance().getMouseX(), InputHandler::getInstance().getMouseY());
		glfwSetCursorPos(m_cView->getWindow(), m_cView->getWindowWidth() / 2, m_cView->getWindowHeight() / 2);
		break;
	}
}

void Camera::SetCameraPos(glm::vec3 CameraPos)
{
	m_v3CameraPos = CameraPos;
}

void Camera::SetCameraTarget(glm::vec3 CameraTarget)
{
	m_v3CameraTarget = CameraTarget;
}

void Camera::SetCameraUp(glm::vec3 CameraUp)
{
	m_v3CameraUp = CameraUp;
}

void Camera::UpdatePitch(float degrees)
{
	m_fPitch += degrees;

	// Limit Pitch
	if (m_fPitch > 89.9f)
	{
		m_fPitch = 89.9f;
	}
	else if (m_fPitch < -89.9f)
	{
		m_fPitch = -89.9f;
	}
}

void Camera::UpdateYaw(float degrees)
{
	if (m_fPitch > 89.9f && m_fPitch < 270.f || (m_fPitch < -89.9f && m_fPitch > -89.9f))
	{
		m_fYaw -= degrees;
	}
	else
	{
		m_fYaw += degrees;
	}

	// Limit Yaw
	if (m_fYaw > 360.f)
	{
		m_fYaw -= 360.f;
	}
	else if (m_fYaw < -360.f)
	{
		m_fYaw += 360.f;
	}
}

void Camera::UpdateRoll(float degrees)
{
}

