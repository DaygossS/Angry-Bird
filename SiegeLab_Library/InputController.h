#pragma once
#include "GameController.h"
//#include "../src/rcore."

class InputController {

public :

	InputController inputController;

	Ray ray = {0};
	RayCollision collision = { 0 };

	void Jeclick();

};

