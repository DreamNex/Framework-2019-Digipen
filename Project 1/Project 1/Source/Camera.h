#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "View.h"
#include "glm/glm.hpp"

class Camera
{
public:
	enum CAMERA_MODE
	{
		CM_FIRST_PERSON,
		CM_THIRD_PERSON,
		CM_THIRD_PERSON_FIXED_OFFSET,
		CM_THIRD_PERSON_FOLLOW,
		CM_FREE_CAMERA // LAST
	};
public:
	Camera(View * view);
	virtual ~Camera();

	virtual void Update(double dt);
	virtual void Move();

	void SetCameraPos(glm::vec3 CameraPos);
	void SetCameraTarget(glm::vec3 CameraTarget);
	void SetCameraUp(glm::vec3 CameraUp);

	void UpdatePitch(float degrees);
	void UpdateYaw(float degrees);
	void UpdateRoll(float degrees);

	float GetPitch() { return m_fPitch; }
	float GetYaw() { return m_fYaw; }
	float GetRoll() { return m_fRoll; }

	//glm::mat4 GetModel() { return m_m4Model; }
	//glm::mat4 GetView() { return m_m4View; }
	//glm::mat4 GetProjection() { return m_m4Projection; }

	glm::mat4 m_m4MVP;
	glm::mat4 m_m4Model;
	glm::mat4 m_m4View;
	glm::mat4 m_m4Projection;
	
	glm::vec3 m_v3CameraPos;
	glm::vec3 m_v3CameraTarget;
	glm::vec3 m_v3CameraUp;
	glm::vec3 m_v3CameraDirection;
	glm::vec3 m_v3CameraRight;
protected:
	float m_fPitch;
	float m_fYaw;
	float m_fRoll;

	glm::vec2 m_v2MousePos;

	CAMERA_MODE m_eCameraMode;
private:
	View* m_cView;
};

#endif