#ifndef SIEGE_LAB_INPUT_SNAPSHOT_H_
#define SIEGE_LAB_INPUT_SNAPSHOT_H_

#include "raylib.h"

namespace siege_lab
{

// « Photo » des entrées du joueur pour UNE frame.
// L'InputController (branche S1) la remplit avec raylib (IsKeyDown,
// GetMouseDelta...). Les caméras ne lisent QUE cette struct : on peut donc
// les tester sans fenêtre, en remplissant la struct à la main.
struct InputSnapshot
{
  bool orbit_drag_held = false;          // clic droit maintenu
  Vector2 mouse_delta = {0.0f, 0.0f};    // déplacement souris en pixels
  float wheel_move = 0.0f;               // crans de molette (+ = avant)
  float orbit_axis_x = 0.0f;             // clavier, -1 (gauche) à +1
  float orbit_axis_y = 0.0f;             // clavier, -1 (bas) à +1
  float move_forward = 0.0f;             // caméra libre : -1 à +1
  float move_right = 0.0f;               // caméra libre : -1 à +1
  float move_up = 0.0f;                  // caméra libre : -1 à +1
  bool next_camera_pressed = false;      // changer de caméra
};

}  // namespace siege_lab

#endif  // SIEGE_LAB_INPUT_SNAPSHOT_H_
