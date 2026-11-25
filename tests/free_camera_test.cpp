// Tests bonus (pas dans l'énoncé) : caméra libre.

#include <gtest/gtest.h>

#include "raylib.h"

#include "free_camera.h"
#include "input_snapshot.h"
#include "test_helpers.h"

namespace siege_lab
{
namespace
{

using test::ExpectVector3Near;
using test::kPositionTolerance;

TEST(FreeCamera, ForwardFollowsLauncherConvention)
{
  // Prepare
  const FreeCamera looking_x({0.0f, 0.0f, 0.0f}, 0.0f, 0.0f, 5.0f);
  const FreeCamera looking_z({0.0f, 0.0f, 0.0f}, 90.0f, 0.0f, 5.0f);
  const FreeCamera launcher_aim({0.0f, 0.0f, 0.0f}, 20.0f, 30.0f, 5.0f);

  // Run + Check : mêmes valeurs que les tests L01 et L04 du lanceur
  ExpectVector3Near(looking_x.GetForward(), {1.0f, 0.0f, 0.0f}, 1e-4f);
  ExpectVector3Near(looking_z.GetForward(), {0.0f, 0.0f, 1.0f}, 1e-4f);
  ExpectVector3Near(launcher_aim.GetForward(),
                    {0.8138f, 0.5000f, 0.2962f}, 1e-4f);
}

TEST(FreeCamera, MovesForwardWithSpeedAndDeltaTime)
{
  // Prepare : 5 m/s vers +X
  FreeCamera camera({-8.0f, 2.0f, 0.0f}, 0.0f, 0.0f, 5.0f);
  InputSnapshot input;
  input.move_forward = 1.0f;

  // Run : 2 frames de 0,5 s = 1 s
  camera.Update(input, 0.5f);
  camera.Update(input, 0.5f);

  // Check : 5 m/s * 1 s = 5 m parcourus
  ExpectVector3Near(camera.GetPosition(), {-3.0f, 2.0f, 0.0f},
                    kPositionTolerance);
}

TEST(FreeCamera, ResetRestoresStartPose)
{
  // Prepare
  FreeCamera camera({-8.0f, 2.0f, 0.0f}, 0.0f, 0.0f, 5.0f);
  InputSnapshot input;
  input.move_forward = 1.0f;
  camera.Update(input, 1.0f);

  // Run
  camera.Reset();

  // Check
  ExpectVector3Near(camera.GetPosition(), {-8.0f, 2.0f, 0.0f},
                    kPositionTolerance);
  ExpectVector3Near(camera.GetForward(), {1.0f, 0.0f, 0.0f}, 1e-4f);
}

}  // namespace
}  // namespace siege_lab
