#include "Entity.h"

Entity::Entity(std::string name)
	: m_eEntityType(ENTITY_NONE)
	, m_cMesh(nullptr)
	, m_sName(name)
	, m_v3Position(glm::vec3(0.f, 0.f, 0.f))
	, m_v3Rotation(glm::vec3(0.f, 0.f, 0.f))
	, m_v3Scale(glm::vec3(1.f, 1.f, 1.f))
{

}

Entity::~Entity()
{
	if (m_cMesh != nullptr)
	{
		delete m_cMesh;
		m_cMesh = nullptr;
	}
}

void Entity::Update(double dt)
{

}

void Entity::SetEntityType(ENTITY_TYPE entityType)
{
	m_eEntityType = entityType;
}

void Entity::SetMesh(Mesh* mesh)
{
	m_cMesh = mesh;
}

void Entity::SetPosition(float x, float y, float z)
{
	m_v3Position = glm::vec3(x, y, z);
}

glm::vec3 Entity::GetPosition()
{
	return m_v3Position;
}

void Entity::SetRotation(float x, float y, float z)
{
	m_v3Rotation = glm::vec3(x, y, z);
}

glm::vec3 Entity::GetRotation()
{
	return m_v3Rotation;
}

void Entity::SetScale(float x, float y, float z)
{
	m_v3Scale = glm::vec3(x, y, z);
}

glm::vec3 Entity::GetScale()
{
	return m_v3Scale;
}
