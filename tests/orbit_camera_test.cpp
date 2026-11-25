// Tests C01 à C05 de l'énoncé (GP-T3 — Orbital camera).
// Schéma de chaque test : Prepare -> Run -> Check.
// Aucune fenêtre raylib : on remplit l'InputSnapshot à la main.

#include <gtest/gtest.h>

#include "raylib.h"
#include "raymath.h"

#include "input_snapshot.h"
#include "orbit_camera.h"
#include "test_helpers.h"

namespace siege_lab
{
namespace
{

using test::ExpectVector3Near;
using test::kAngleTolerance;
using test::kPositionTolerance;

// Appelle Update() _frame_count fois avec la même entrée.
void RunFrames(OrbitCamera& _camera, const InputSnapshot& _input,
               float _dt, int _frame_count)
{
  for (int frame = 0; frame < _frame_count; ++frame)
  {
    _camera.Update(_input, _dt);
  }
}

TEST(Camera, TargetAndRadiusRemainStable)
{
  // Prepare
  const Vector3 target = {2.0f, 1.0f, 0.0f};
  OrbitCamera camera(target, 10.0f, 90.0f);

  // Run + Check : position de départ
  camera.SetAngles(0.0f, 20.0f);
  ExpectVector3Near(camera.GetPosition(), {2.0f, 4.4202f, 9.3969f},
                    kPositionTolerance);

  // Run + Check : position après avoir changé les angles
  camera.SetAngles(90.0f, 30.0f);
  ExpectVector3Near(camera.GetPosition(), {10.6603f, 6.0f, 0.0f},
                    kPositionTolerance);
  ExpectVector3Near(camera.GetTarget(), target, kPositionTolerance);
  EXPECT_NEAR(Vector3Distance(camera.GetPosition(), target), 10.0f,
              kPositionTolerance);
}

TEST(Camera, PitchAndZoomAreClamped)
{
  // Prepare
  OrbitCamera camera({0.0f, 0.0f, 0.0f}, 10.0f, 90.0f);
  camera.SetPitchLimits(10.0f, 80.0f);
  camera.SetDistanceLimits(4.0f, 25.0f);

  // Run + Check : valeurs trop petites
  camera.SetAngles(0.0f, -30.0f);
  camera.SetDistance(2.0f);
  EXPECT_NEAR(camera.GetPitch(), 10.0f, kAngleTolerance);
  EXPECT_NEAR(camera.GetDistance(), 4.0f, kPositionTolerance);

  // Run + Check : valeurs trop grandes
  camera.SetAngles(0.0f, 100.0f);
  camera.SetDistance(40.0f);
  EXPECT_NEAR(camera.GetPitch(), 80.0f, kAngleTolerance);
  EXPECT_NEAR(camera.GetDistance(), 25.0f, kPositionTolerance);
}

TEST(Camera, FullTurnReturnsToSamePose)
{
  // Prepare
  OrbitCamera camera({0.0f, 0.0f, 0.0f}, 10.0f, 90.0f);
  camera.SetAngles(30.0f, 20.0f);
  const Vector3 before = camera.GetPosition();

  // Run : un tour complet (30° + 360° = 390°)
  camera.SetAngles(390.0f, 20.0f);
  const Vector3 after = camera.GetPosition();

  // Check
  ExpectVector3Near(before, {4.6985f, 3.4202f, 8.1380f},
                    kPositionTolerance);
  ExpectVector3Near(after, before, 1e-4f);
}

TEST(Camera, RotationSpeedUsesDeltaTime)
{
  // Prepare : deux caméras identiques, vitesse 90°/s
  OrbitCamera camera_a({0.0f, 0.0f, 0.0f}, 10.0f, 90.0f);
  OrbitCamera camera_b({0.0f, 0.0f, 0.0f}, 10.0f, 90.0f);
  camera_a.SetPitchLimits(-80.0f, 80.0f);
  camera_b.SetPitchLimits(-80.0f, 80.0f);
  InputSnapshot input;
  input.orbit_axis_x = 1.0f;

  // Run : même durée totale (0,5 s), découpée différemment
  RunFrames(camera_a, input, 0.5f, 1);
  RunFrames(camera_b, input, 0.25f, 2);

  // Check : 90°/s * 0,5 s = 45°
  EXPECT_NEAR(camera_a.GetYaw(), 45.0f, kAngleTolerance);
  ExpectVector3Near(camera_a.GetPosition(), {7.0711f, 0.0f, 7.0711f},
                    kPositionTolerance);
  ExpectVector3Near(camera_b.GetPosition(), camera_a.GetPosition(),
                    kPositionTolerance);
}

TEST(Camera, MouseDragIsFrameRateIndependent)
{
  // Prepare : sensibilité 0,25 °/pixel, clic droit maintenu
  OrbitCamera camera_60fps({0.0f, 0.0f, 0.0f}, 10.0f, 90.0f);
  OrbitCamera camera_30fps({0.0f, 0.0f, 0.0f}, 10.0f, 90.0f);
  camera_60fps.SetMouseSensitivity(0.25f);
  camera_30fps.SetMouseSensitivity(0.25f);
  InputSnapshot input_60fps;
  input_60fps.orbit_drag_held = true;
  input_60fps.mouse_delta = {2.0f, 0.0f};
  InputSnapshot input_30fps = input_60fps;
  input_30fps.mouse_delta = {4.0f, 0.0f};

  // Run : 120 pixels au total dans les deux cas
  RunFrames(camera_60fps, input_60fps, 1.0f / 60.0f, 60);
  RunFrames(camera_30fps, input_30fps, 1.0f / 30.0f, 30);

  // Check : 120 px * 0,25 °/px = 30°, pitch inchangé
  for (const OrbitCamera* camera : {&camera_60fps, &camera_30fps})
  {
    EXPECT_NEAR(camera->GetYaw(), 30.0f, kAngleTolerance);
    EXPECT_NEAR(camera->GetPitch(), 0.0f, kAngleTolerance);
    ExpectVector3Near(camera->GetPosition(), {5.0f, 0.0f, 8.6603f},
                      kPositionTolerance);
  }
}

}  // namespace
}  // namespace siege_lab
