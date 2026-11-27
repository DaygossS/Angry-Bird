#ifndef SIEGE_LAB_I_CAMERA_CONTROLLER_H_
#define SIEGE_LAB_I_CAMERA_CONTROLLER_H_

#include "raylib.h"

#include "input_snapshot.h"

namespace siege_lab
{

// Interface commune à toutes les caméras (énoncé M1).
// « Interface » = classe qui ne contient que des fonctions virtuelles
// pures (= 0) : elle dit CE QUE fait une caméra, pas COMMENT.
// CameraManager ne connaît que cette interface : il peut changer de
// caméra sans savoir laquelle c'est (pas de if, pas de dynamic_cast).
class ICameraController
{
 public:
  // Destructeur virtuel : obligatoire dans une classe de base, sinon
  // détruire une caméra via un ICameraController* oublie la classe fille.
  virtual ~ICameraController() = default;

  // Appelée une fois par frame avec les entrées du joueur.
  virtual void Update(const InputSnapshot& _input, float _dt) = 0;
  // Caméra raylib à donner à BeginMode3D().
  virtual Camera3D GetCamera() const = 0;
  // Remet la caméra dans son état de départ.
  virtual void Reset() = 0;
};

}  // namespace siege_lab

#endif  // SIEGE_LAB_I_CAMERA_CONTROLLER_H_
