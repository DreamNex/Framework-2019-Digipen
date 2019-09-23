#ifndef MODEL_H
#define MODEL_H

#include "StateHandler.h"
#include "MeshBuilder.h"

#include <iostream>
#include <string>
#include <unordered_map>

class Entity;

class Model
{
public:
	Model();
	virtual ~Model(void);

	void Init(StateHandler * stateHandler);
	void Update(double dt);
	
	// Probably UnNeeded
	StateHandler * GetStateHandler();

	// Mesh Functions
	void AddToMeshList(Mesh * mesh);
	void RemoveFromMeshList(Mesh * mesh);
	std::vector<Mesh*> GetMeshList() { return m_cMeshList; }

	// EntityList Functions
	void AddToEntityList(Entity* entity);
	void RemoveFromEntityList(std::string entityName);
	std::unordered_map<std::string, Entity*> GetEntityList() { return m_cEntityList; }

private:
	std::unordered_map<std::string, Entity*> m_cEntityList;
	std::vector<Mesh*> m_cMeshList;
	StateHandler * m_cStateHandler;
};

#endif