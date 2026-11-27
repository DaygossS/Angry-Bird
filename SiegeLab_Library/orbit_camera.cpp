#include "pch.h"
#include "orbit_camera.h"

#include "raymath.h"

#include "camera_utils.h"

namespace siege_lab
{

OrbitCamera::OrbitCamera(Vector3 _target, float _distance,
                         float _speed_degrees)
    : m_target(_target),
      m_initial_distance(_distance),
      m_distance(_distance),
      m_speed_degrees(_speed_degrees)
{
}

void OrbitCamera::Update(const InputSnapshot& _input, float _dt)
{
  // TODO (C04) : clavier -> yaw += orbit_axis_x * m_speed_degrees * _dt
  //              (pareil pour le pitch avec orbit_axis_y).
  // TODO (C05) : si orbit_drag_held -> yaw += mouse_delta.x * sensibilité.
  //              PAS de _dt ici : les pixels sont déjà « par frame ».
  //              Bonus : limiter la rotation à m_max_mouse_speed * _dt.
  // TODO : molette -> SetDistance(m_distance - wheel_move * m_zoom_step).
  // Astuce : passe par SetAngles() et SetDistance() pour garder le clamp.
}

Camera3D OrbitCamera::GetCamera() const
{
  return MakePerspectiveCamera(GetPosition(), m_target);
}

void OrbitCamera::Reset()
{
    m_yaw = 0.f;
    m_pitch = 0.f;
    SetDistance(m_initial_distance);
}

void OrbitCamera::SetAngles(float _yaw_degrees, float _pitch_degrees)
{
  m_yaw = _yaw_degrees;
  // TODO (C02) : bloquer le pitch entre m_min_pitch et m_max_pitch.
  // Indice : Clamp() dans raymath.h.
  m_pitch = _pitch_degrees;
}

void OrbitCamera::SetDistance(float _distance)
{
  
  // TODO (C02) : bloquer la distance entre m_min_distance et m_max_distance.
  m_distance = _distance;
}

void OrbitCamera::SetPitchLimits(float _min_degrees, float _max_degrees)
{
  m_min_pitch = _min_degrees;
  m_max_pitch = _max_degrees;
}

void OrbitCamera::SetDistanceLimits(float _min_distance,
                                    float _max_distance)
{
  m_min_distance = _min_distance;
  m_max_distance = _max_distance;
}

void OrbitCamera::SetMouseSensitivity(float _degrees_per_pixel)
{
  m_mouse_sensitivity = _degrees_per_pixel;
}

float OrbitCamera::GetYaw() const
{
  return m_yaw;
}

float OrbitCamera::GetPitch() const
{
  return m_pitch;
}

float OrbitCamera::GetDistance() const
{
  return m_distance;
}

Vector3 OrbitCamera::GetTarget() const
{
  return m_target;
}

Vector3 OrbitCamera::GetPosition() const
{
  // TODO (C01, C03) : coordonnées sphériques autour de m_target.
  // - convertir yaw et pitch en radians (DEG2RAD) ;
  // - yaw = 0 et pitch = 0 doit donner m_target + (0, 0, m_distance) ;
  // - yaw = 90 et pitch = 0 doit donner m_target + (m_distance, 0, 0) ;
  // - à toi de trouver où vont sinf() et cosf() !
  return m_target;
}

}  // namespace siege_lab
