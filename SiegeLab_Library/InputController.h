#pragma once
#include "raylib.h"

class InputController {

public:

	void Selecting(const Camera3D& camera, Vector3 spherePosition, float sphereRadius);
	bool IsSelected() const;
	Ray GetRay() const;

private:

	Ray ray = { 0 };
	RayCollision collision = { 0 };



};