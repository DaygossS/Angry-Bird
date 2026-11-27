#include "pch.h"
#include "InputController.h"

void InputController::Selecting(const Camera3D& camera, Vector3 spherePosition, float sphereRadius) {

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

		if (!collision.hit) {
			ray = GetScreenToWorldRay(GetMousePosition(), camera);
			collision = GetRayCollisionSphere(ray, spherePosition, sphereRadius);
		}
		else {
			collision.hit = false;
		}
	}
}

bool InputController::IsSelected() const {
	return collision.hit;
}

Ray InputController::GetRay() const {
	return ray;
}

static char const* GetGestureName(int gesture)
{
	switch (gesture) { 
	case 0: return "None"; break;
	case 1: return "Tap"; break;
	case 2: return "Double Tap"; break;
	case 4: return "Hold"; break;
	case 8: return "Drag"; break;