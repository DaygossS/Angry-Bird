#include "pch.h"
#include "free_camera.h"

#include "raymath.h"

#include "camera_utils.h"

namespace siege_lab
{

FreeCamera::FreeCamera(Vector3 _position, float _yaw_degrees,
                       float _pitch_degrees, float _move_speed)
    : m_initial_position(_position),
      m_position(_position),
      m_initial_yaw(_yaw_degrees),
      m_yaw(_yaw_degrees),
      m_initial_pitch(_pitch_degrees),
      m_pitch(_pitch_degrees),
      m_move_speed(_move_speed)
{
}

void FreeCamera::Update(const InputSnapshot& _input, float _dt)
{
  // TODO : si orbit_drag_held -> tourner yaw/pitch avec mouse_delta
  //        (pixels * sensibilité, sans _dt) et bloquer le pitch.
  // TODO : avancer de GetForward() * move_forward * m_move_speed * _dt.
  // TODO : pareil sur le côté (move_right) et en hauteur (move_up).
  // Indice : le vecteur « droite » = produit vectoriel de forward et du
  // haut (0, 1, 0) -> Vector3CrossProduct() dans raymath.h.
}

Camera3D FreeCamera::GetCamera() const
{
  return MakePerspectiveCamera(m_position,
                               Vector3Add(m_position, GetForward()));
}

void FreeCamera::Reset()
{
  // TODO : remettre position, yaw et pitch de départ.
}

void FreeCamera::SetMouseSensitivity(float _degrees_per_pixel)
{
  m_mouse_sensitivity = _degrees_per_pixel;
}

Vector3 FreeCamera::GetPosition() const
{
  return m_position;
}

Vector3 FreeCamera::GetForward() const
{
  // TODO : direction à partir de yaw et pitch (en radians).
  // yaw = 0, pitch = 0 -> (1, 0, 0) ; yaw = 90 -> (0, 0, 1).
  // C'est le même calcul que la direction du lanceur (test L01).
  return Vector3{0.0f, 0.0f, 0.0f};
}

}  // namespace siege_lab
