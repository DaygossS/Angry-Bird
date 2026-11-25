#include "pch.h"
#include "camera_utils.h"

namespace siege_lab
{

namespace
{
constexpr float kFieldOfViewDegrees = 45.0f;
}  // namespace

Camera3D MakePerspectiveCamera(Vector3 _position, Vector3 _target)
{
  Camera3D camera = {};
  camera.position = _position;
  camera.target = _target;
  camera.up = {0.0f, 1.0f, 0.0f};
  camera.fovy = kFieldOfViewDegrees;
  camera.projection = CAMERA_PERSPECTIVE;
  return camera;
}

}  // namespace siege_lab
