#ifndef SIEGE_LAB_FPS_CAMERA_H_
#define SIEGE_LAB_FPS_CAMERA_H_

#include "raylib.h"

#include "i_camera_controller.h"
#include "input_snapshot.h"

namespace siege_lab
{

// Caméra « à la place du projectile » : placée juste derrière l'objet
// suivi, dans le sens inverse de sa vitesse, un peu au-dessus, et elle
// regarde l'objet. À utiliser quand les projectiles existeront (M3).
class FpsCamera : public ICameraController
{
 public:
  FpsCamera(float _back_distance, float _height);

  void Update(const InputSnapshot& _input, float _dt) override;
  Camera3D GetCamera() const override;
  void Reset() override;

  // Appelée à chaque frame par le jeu avec la position et la vitesse du
  // projectile (lues dans son RigidBody Cyclone et converties en Vector3).
  void SetFollowTarget(Vector3 _position, Vector3 _velocity);

  Vector3 GetPosition() const;
  Vector3 GetTarget() const;

 private:
  float m_back_distance;                     // mètres derrière l'objet
  float m_height;                            // mètres au-dessus
  Vector3 m_follow_position = {0.0f, 0.0f, 0.0f};
  Vector3 m_direction = {1.0f, 0.0f, 0.0f};  // dernière direction connue
};

}  // namespace siege_lab

#endif  // SIEGE_LAB_FPS_CAMERA_H_
