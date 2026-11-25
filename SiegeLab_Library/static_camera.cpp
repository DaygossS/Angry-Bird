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
  // Rien à faire : une caméra fixe ignore les entrées.
}

Camera3D StaticCamera::GetCamera() const
{
  // TODO : construire la caméra avec MakePerspectiveCamera().
  return Camera3D{};
}

void StaticCamera::Reset()
{
  // Rien à faire : la caméra n'a jamais bougé.
}

}  // namespace siege_lab
