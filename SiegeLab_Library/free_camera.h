#ifndef SIEGE_LAB_FREE_CAMERA_H_
#define SIEGE_LAB_FREE_CAMERA_H_

#include "raylib.h"

#include "i_camera_controller.h"
#include "input_snapshot.h"

namespace siege_lab
{


class FreeCamera : public ICameraController
{
 public:
  FreeCamera(Vector3 _position, float _yaw_degrees, float _pitch_degrees,
             float _move_speed);

  void Update(const InputSnapshot& _input, float _dt) override;
  Camera3D GetCamera() const override;
  void Reset() override;

  void SetMouseSensitivity(float _degrees_per_pixel);

  Vector3 GetPosition() const;
  Vector3 GetForward() const;

 private:
  Vector3 m_initial_position;
  Vector3 m_position;
  float m_initial_yaw;
  float m_yaw;
  float m_initial_pitch;
  float m_pitch;
  float m_move_speed;                 
  float m_mouse_sensitivity = 0.25f;  
  float m_min_pitch = -89.0f;
  float m_max_pitch = 89.0f;
};

}  // namespace siege_lab

#endif  // SIEGE_LAB_FREE_CAMERA_H_
