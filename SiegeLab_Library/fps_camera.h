#ifndef SIEGE_LAB_FPS_CAMERA_H_
#define SIEGE_LAB_FPS_CAMERA_H_

#include "raylib.h"

#include "i_camera_controller.h"
#include "input_snapshot.h"

namespace siege_lab
{

class FpsCamera : public ICameraController
{
 public:
  FpsCamera(float _back_distance, float _height);

  void Update(const InputSnapshot& _input, float _dt) override;
  Camera3D GetCamera() const override;
  void Reset() override;
  void SetFollowTarget(Vector3 _position, Vector3 _velocity);

  Vector3 GetPosition() const;
  Vector3 GetTarget() const;

 private:
  float m_back_distance;                     
  float m_height;                           
  Vector3 m_follow_position = {0.0f, 0.0f, 0.0f};
  Vector3 m_direction = {1.0f, 0.0f, 0.0f};  
};

} 

#endif  // SIEGE_LAB_FPS_CAMERA_H_
