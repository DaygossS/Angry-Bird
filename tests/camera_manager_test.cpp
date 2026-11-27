// Test CAM06 de l'énoncé + un test bonus sur CameraManager.
// On crée de FAUSSES caméras qui héritent de ICameraController :
// c'est comme ça qu'on « trouve » une interface à passer en paramètre.

#include <gtest/gtest.h>

#include "raylib.h"

#include "camera_manager.h"
#include "camera_utils.h"
#include "i_camera_controller.h"
#include "input_snapshot.h"
#include "orbit_camera.h"
#include "test_helpers.h"

namespace siege_lab
{
namespace
{

using test::ExpectVector3Near;
using test::kPositionTolerance;

// Fausse caméra de l'énoncé : renvoie toujours la position (5, 2, 5).
class FakeCameraController : public ICameraController
{
 public:
  void Update(const InputSnapshot&, float) override {}
  Camera3D GetCamera() const override
  {
    return MakePerspectiveCamera({5.0f, 2.0f, 5.0f}, {0.0f, 0.0f, 0.0f});
  }
  void Reset() override {}
};

// Fausse caméra « espion » : compte combien de fois Update() est appelée.
class SpyCameraController : public FakeCameraController
{
 public:
  void Update(const InputSnapshot&, float) override { ++m_update_count; }
  int GetUpdateCount() const { return m_update_count; }

 private:
  int m_update_count = 0;
};

TEST(Camera, ManagerCanReplaceController)
{
  // Prepare
  OrbitCamera orbit({0.0f, 0.0f, 0.0f}, 10.0f, 90.0f);
  FakeCameraController fake;
  CameraManager manager;
  manager.SetActive(&orbit);
  // TODO (quand le Launcher de la branche S1 sera fusionné) : créer un
  // Launcher azimut 20°, élévation 30° et vérifier que sa direction vaut
  // (0.8138, 0.5000, 0.2962) avant ET après le changement de caméra.

  // Run
  manager.SetActive(&fake);
  const Camera3D camera = manager.GetCamera();

  // Check
  ExpectVector3Near(camera.position, {5.0f, 2.0f, 5.0f},
                    kPositionTolerance);
}

// Test bonus (pas dans l'énoncé).
TEST(Camera, ManagerForwardsUpdateToActiveController)
{
  // Prepare
  SpyCameraController active;
  SpyCameraController inactive;
  CameraManager manager;
  manager.SetActive(&active);

  // Run
  manager.Update(InputSnapshot{}, 1.0f / 60.0f);
  manager.Update(InputSnapshot{}, 1.0f / 60.0f);

  // Check : seule la caméra active reçoit les Update()
  EXPECT_EQ(active.GetUpdateCount(), 2);
  EXPECT_EQ(inactive.GetUpdateCount(), 0);
}

}  // namespace
}  // namespace siege_lab
