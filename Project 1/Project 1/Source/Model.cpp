#include "Model.h"
#include "Entity.h"

Model::Model()
{
	std::cout << "Model Created" << std::endl;
	m_cStateHandler = nullptr;
}

Model::~Model()
{
	if (!m_cEntityList.empty())
	{
		for (auto entry : m_cEntityList)
		{
			delete entry.second;
		}
		m_cEntityList.clear();
	}

	if (!m_cMeshList.empty())
	{
		for (int i = 0; i < m_cMeshList.size(); i++)
		{
			delete m_cMeshList[i];
		}
		m_cMeshList.clear();
	}
	m_cMeshList.clear();
	m_cMeshList.~vector();
}

void Model::Init(StateHandler * stateHandler)
{
	m_cStateHandler = stateHandler;
}

void Model::Update(double dt)
{

}

StateHandler * Model::GetStateHandler()
{
	return m_cStateHandler;
}

void Model::AddToMeshList(Mesh* mesh)
{
	m_cMeshList.push_back(mesh);
}

void Model::RemoveFromMeshList(Mesh* mesh)
{

}

void Model::AddToEntityList(Entity* entity)
{
	m_cEntityList.insert(std::make_pair(entity->toString(), entity));
}

void Model::RemoveFromEntityList(std::string entityName)
{
	// Removing by Key
	m_cEntityList.erase(entityName);
}
