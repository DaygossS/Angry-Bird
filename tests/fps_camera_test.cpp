// Tests bonus (pas dans l'énoncé) : caméra qui suit le projectile.

#include <gtest/gtest.h>

#include "raylib.h"

#include "fps_camera.h"
#include "test_helpers.h"

namespace siege_lab
{
namespace
{

using test::ExpectVector3Near;
using test::kPositionTolerance;

TEST(FpsCamera, PlacedBehindAndAboveProjectile)
{
  // Prepare : 3 m derrière, 0,5 m au-dessus
  FpsCamera camera(3.0f, 0.5f);

  // Run : projectile en (0, 5, 0) qui file vers +X
  camera.SetFollowTarget({0.0f, 5.0f, 0.0f}, {10.0f, 0.0f, 0.0f});

  // Check
  ExpectVector3Near(camera.GetPosition(), {-3.0f, 5.5f, 0.0f},
                    kPositionTolerance);
  ExpectVector3Near(camera.GetTarget(), {0.0f, 5.0f, 0.0f},
                    kPositionTolerance);
}

TEST(FpsCamera, ZeroVelocityKeepsLastDirection)
{
  // Prepare : le projectile file d'abord vers +Z
  FpsCamera camera(3.0f, 0.5f);
  camera.SetFollowTarget({0.0f, 5.0f, 0.0f}, {0.0f, 0.0f, 10.0f});

  // Run : il s'arrête net (vitesse nulle)
  camera.SetFollowTarget({1.0f, 5.0f, 1.0f}, {0.0f, 0.0f, 0.0f});

  // Check : la caméra reste derrière, côté -Z (pas de division par zéro)
  ExpectVector3Near(camera.GetPosition(), {1.0f, 5.5f, -2.0f},
                    kPositionTolerance);
}

TEST(FpsCamera, ResetRestoresDefaultDirection)
{
  // Prepare
  FpsCamera camera(3.0f, 0.5f);
  camera.SetFollowTarget({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 10.0f});

  // Run
  camera.Reset();

  // Check : direction de départ (+X), donc caméra côté -X
  ExpectVector3Near(camera.GetPosition(), {-3.0f, 0.5f, 0.0f},
                    kPositionTolerance);
}

}  // namespace
}  // namespace siege_lab
