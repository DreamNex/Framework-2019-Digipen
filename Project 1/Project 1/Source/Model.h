#ifndef MODEL_H
#define MODEL_H

#include "StateHandler.h"
#include "MeshBuilder.h"

#include <iostream>
#include <string>

class Model
{
public:
	Model();
	virtual ~Model(void);

	Model * getInstance() { return this; }

	void Init(StateHandler * stateHandler);
	void Update(double dt);
	StateHandler * GetStateHandler();
private:
	StateHandler* m_cStateHandler;
};

#endif