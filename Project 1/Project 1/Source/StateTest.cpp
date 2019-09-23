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

	// Create Cubes
	auto newEntity = new Entity("Cube");
	newEntity->SetMesh(MeshBuilder::GenerateCube("Cube", Mesh::Color(0.f, 0.f, 1.f), 5.f));
	newEntity->GetMesh()->alpha = 0.5f;
	theModel->AddToEntityList(newEntity);

	newEntity = new Entity("Cube 2");
	newEntity->SetMesh(MeshBuilder::GenerateCube("Cube 2", Mesh::Color(0.f, 1.f, 0.f), 10.f));
	newEntity->SetPosition(10.f, 10.f, 0.f);
	theModel->AddToEntityList(newEntity);

	newEntity = new Entity("Cube 3");
	newEntity->SetMesh(MeshBuilder::GenerateCube("Cube 3", Mesh::Color(0.f, 1.f, 0.f), 10.f));
	newEntity->SetPosition(10.f, 10.f, 0.f);
	theModel->AddToEntityList(newEntity);
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
	for (auto entity : theModel->GetEntityList())
	{
		theView->RenderEntity(entity.second);
	}
}

void StateTest::RenderBackground()
{

}
