#ifndef SIEGE_LAB_CAMERA_UTILS_H_
#define SIEGE_LAB_CAMERA_UTILS_H_

#include "raylib.h"

namespace siege_lab
{

// Construit une Camera3D raylib en perspective (axe Y vers le haut).
// Toutes les caméras s'en servent dans leur GetCamera().
Camera3D MakePerspectiveCamera(Vector3 _position, Vector3 _target);

}  // namespace siege_lab

#endif  // SIEGE_LAB_CAMERA_UTILS_H_
