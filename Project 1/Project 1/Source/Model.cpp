#include "Model.h"

Model::Model()
{
	std::cout << "Model Created" << std::endl;
	m_cStateHandler = nullptr;
}

Model::~Model()
{

}

void Model::Init(StateHandler * stateHandler)
{
	// Setting up StateHandler pointer
	m_cStateHandler = stateHandler;
}

void Model::Update(double dt)
{

}

StateHandler * Model::GetStateHandler()
{
	return m_cStateHandler;
}