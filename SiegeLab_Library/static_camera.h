#ifndef SIEGE_LAB_STATIC_CAMERA_H_
#define SIEGE_LAB_STATIC_CAMERA_H_

#include "raylib.h"

#include "i_camera_controller.h"
#include "input_snapshot.h"

namespace siege_lab
{

class StaticCamera : public ICameraController
{
 public:
  StaticCamera(Vector3 _position, Vector3 _target);

  void Update(const InputSnapshot& _input, float _dt) override;
  Camera3D GetCamera() const override;
  void Reset() override;

 private:
  Vector3 m_position;
  Vector3 m_target;
};

}

#endif  // SIEGE_LAB_STATIC_CAMERA_H_
