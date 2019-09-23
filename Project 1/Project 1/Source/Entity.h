#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <iostream>
#include <string>

#include "Mesh.h"
#include "glm/glm.hpp"

class Entity
{
public:
	enum ENTITY_TYPE
	{
		ENTITY_PLAYER,
		ENTITY_NPC,
		ENTITY_OBJECT,
		ENTITY_STATIC_OBJECT,
		ENTITY_RIGIDBODY,
		ENTITY_NONE,
	};
public:
	Entity(std::string name = "");
	virtual ~Entity();

	virtual void Update(double dt);

	//#TODO -> Component System Maybe? 

	std::string toString() { return m_sName; }

	// Generic Functions
	void SetEntityType(ENTITY_TYPE entityType = ENTITY_NONE);
	ENTITY_TYPE GetEntityType() { return m_eEntityType; }
	void SetMesh(Mesh* mesh);
	Mesh* GetMesh() { return m_cMesh; }

	// Transformation
	void SetPosition(float x, float y, float z);
	glm::vec3 GetPosition();
	void SetRotation(float x, float y, float z);
	glm::vec3 GetRotation();
	void SetScale(float x, float y, float z);
	glm::vec3 GetScale();

protected:
	Mesh* m_cMesh;

private:
	std::string m_sName;
	
	ENTITY_TYPE m_eEntityType;

	// Maybe use v4 for Orientation? (Quaternions)
	glm::vec3 m_v3Position;
	glm::vec3 m_v3Rotation;
	glm::vec3 m_v3Scale;
};

#endif