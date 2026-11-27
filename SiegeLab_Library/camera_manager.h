#ifndef SIEGE_LAB_CAMERA_MANAGER_H_
#define SIEGE_LAB_CAMERA_MANAGER_H_

#include "raylib.h"

#include "i_camera_controller.h"
#include "input_snapshot.h"

namespace siege_lab
{
class CameraManager
{
 public:

  void SetActive(ICameraController* _controller);
  void Update(const InputSnapshot& _input, float _dt);
  Camera3D GetCamera() const;
  bool HasActive() const;

 private:
  ICameraController* m_active = nullptr;
};

} 

#endif  // SIEGE_LAB_CAMERA_MANAGER_H_
