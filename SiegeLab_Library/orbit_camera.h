#ifndef SIEGE_LAB_ORBIT_CAMERA_H_
#define SIEGE_LAB_ORBIT_CAMERA_H_

#include "raylib.h"

#include "i_camera_controller.h"
#include "input_snapshot.h"

namespace siege_lab
{

// Caméra qui tourne autour d'une cible fixe (énoncé M1, tests C01 à C05).
// Convention de l'énoncé : (yaw, pitch) = (0°, 0°) place la caméra sur +Z
// par rapport à la cible ; un yaw positif tourne vers +X ; un pitch
// positif monte. Tous les angles sont en DEGRÉS.
class OrbitCamera : public ICameraController
{
 public:
  OrbitCamera(Vector3 _target, float _distance, float _speed_degrees);

  void Update(const InputSnapshot& _input, float _dt) override;
  Camera3D GetCamera() const override;
  void Reset() override;

  void SetAngles(float _yaw_degrees, float _pitch_degrees);
  void SetDistance(float _distance);
  void SetPitchLimits(float _min_degrees, float _max_degrees);
  void SetDistanceLimits(float _min_distance, float _max_distance);
  void SetMouseSensitivity(float _degrees_per_pixel);

  float GetYaw() const;
  float GetPitch() const;
  float GetDistance() const;
  Vector3 GetTarget() const;
  Vector3 GetPosition() const;

 private:
  Vector3 m_target;
  float m_initial_distance;
  float m_distance;
  float m_yaw = 0.0f;
  float m_pitch = 0.0f;
  float m_speed_degrees;              // vitesse au clavier, en °/s
  float m_mouse_sensitivity = 0.25f;  // degrés par pixel de souris
  float m_max_mouse_speed = 360.0f;   // °/s maximum à la souris
  float m_zoom_step = 1.0f;           // mètres par cran de molette
  float m_min_pitch = -80.0f;
  float m_max_pitch = 80.0f;
  float m_min_distance = 2.0f;
  float m_max_distance = 50.0f;
};

}  // namespace siege_lab

#endif  // SIEGE_LAB_ORBIT_CAMERA_H_
