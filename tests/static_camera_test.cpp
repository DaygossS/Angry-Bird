// Test bonus (pas dans l'énoncé) : la caméra fixe ne bouge jamais.

#include <gtest/gtest.h>

#include "raylib.h"

#include "input_snapshot.h"
#include "static_camera.h"
#include "test_helpers.h"

namespace siege_lab
{
namespace
{

using test::ExpectVector3Near;
using test::kPositionTolerance;

TEST(StaticCamera, IgnoresInputAndKeepsPose)
{
  // Prepare
  StaticCamera camera({0.0f, 15.0f, 20.0f}, {2.0f, 0.0f, 0.0f});
  InputSnapshot input;
  input.orbit_drag_held = true;
  input.mouse_delta = {50.0f, 30.0f};
  input.move_forward = 1.0f;

  // Run
  camera.Update(input, 1.0f);
  const Camera3D result = camera.GetCamera();

  // Check
  ExpectVector3Near(result.position, {0.0f, 15.0f, 20.0f},
                    kPositionTolerance);
  ExpectVector3Near(result.target, {2.0f, 0.0f, 0.0f}, kPositionTolerance);
  ExpectVector3Near(result.up, {0.0f, 1.0f, 0.0f}, kPositionTolerance);
  EXPECT_GT(result.fovy, 0.0f);
}

}  // namespace
}  // namespace siege_lab
