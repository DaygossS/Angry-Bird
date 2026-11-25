#include "pch.h"
#include "camera_manager.h"

namespace siege_lab
{

void CameraManager::SetActive(ICameraController* _controller)
{
  // TODO (CAM06) : retenir _controller comme caméra active.
}

void CameraManager::Update(const InputSnapshot& _input, float _dt)
{
  // TODO : s'il y a une caméra active, appeler SON Update().
  // C'est le polymorphisme qui choisit la bonne caméra : aucun if de type.
}

Camera3D CameraManager::GetCamera() const
{
  // TODO (CAM06) : renvoyer la caméra de la caméra active.
  // Sans caméra active : renvoyer une Camera3D vide ({}).
  return Camera3D{};
}

bool CameraManager::HasActive() const
{
  return m_active != nullptr;
}

}  // namespace siege_lab
