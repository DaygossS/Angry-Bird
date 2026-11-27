#include "pch.h"
#include "static_camera.h"

#include "camera_utils.h"

namespace siege_lab
{

StaticCamera::StaticCamera(Vector3 _position, Vector3 _target)
    : m_position(_position), m_target(_target)
{
}

void StaticCamera::Update(const InputSnapshot& _input, float _dt)
{

}

Camera3D StaticCamera::GetCamera() const
{
    Camera3D camera = {};
    camera.position = { 10.0f, 10.0f, 10.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.f;
    camera.projection = CAMERA_PERSPECTIVE;
  MakePerspectiveCamera(camera.position, camera.target);
  return Camera3D{};
}

void StaticCamera::Reset()
{

}

} 
