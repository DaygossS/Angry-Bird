#include "pch.h"
#include "fps_camera.h"

#include "raymath.h"

#include "camera_utils.h"

namespace siege_lab
{

FpsCamera::FpsCamera(float _back_distance, float _height)
    : m_back_distance(_back_distance), m_height(_height)
{
}

void FpsCamera::Update(const InputSnapshot& _input, float _dt)
{
  // Rien à faire : c'est SetFollowTarget() qui fait bouger cette caméra.
}

Camera3D FpsCamera::GetCamera() const
{
  return MakePerspectiveCamera(GetPosition(), GetTarget());
}

void FpsCamera::Reset()
{
  // TODO : revenir à la direction de départ (1, 0, 0).
}

void FpsCamera::SetFollowTarget(Vector3 _position, Vector3 _velocity)
{
  m_follow_position = _position;
  // TODO : si la vitesse n'est pas nulle, m_direction = vitesse normalisée
  //        (Vector3Normalize). Si elle est nulle (objet arrêté), GARDER
  //        l'ancienne direction, sinon on diviserait par zéro.
  //        Indice : Vector3Length() pour tester si elle est nulle.
}

Vector3 FpsCamera::GetPosition() const
{
  // TODO : position suivie - direction * m_back_distance + (0, m_height, 0).
  return m_follow_position;
}

Vector3 FpsCamera::GetTarget() const
{
  return m_follow_position;
}

}  // namespace siege_lab
