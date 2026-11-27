#ifndef SIEGE_LAB_CAMERA_MANAGER_H_
#define SIEGE_LAB_CAMERA_MANAGER_H_

#include "raylib.h"

#include "i_camera_controller.h"
#include "input_snapshot.h"

namespace siege_lab
{

// Garde la caméra active et lui transmet les entrées (énoncé M1, CAM06).
// Le manager NE POSSÈDE PAS les caméras : elles sont créées ailleurs
// (dans GameController, ou dans un test) et il garde juste un pointeur
// vers celle qui est active.
class CameraManager
{
 public:
  // Pointeur (et pas référence) : les conventions interdisent de garder
  // une référence reçue en paramètre après la fin de la fonction.
  void SetActive(ICameraController* _controller);
  void Update(const InputSnapshot& _input, float _dt);
  Camera3D GetCamera() const;
  bool HasActive() const;

 private:
  ICameraController* m_active = nullptr;
};

}  // namespace siege_lab

#endif  // SIEGE_LAB_CAMERA_MANAGER_H_
