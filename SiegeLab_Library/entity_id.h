#ifndef SIEGE_LAB_ENTITY_ID_H_
#define SIEGE_LAB_ENTITY_ID_H_

namespace siege_lab
{

constexpr int kInvalidSlot = -1;

// Poignée (handle) vers un objet rangé dans un ObjectPool.
// La génération change à chaque réutilisation d'une case : un vieux
// handle ne pointe donc jamais vers le nouvel objet de la même case.
struct EntityId
{
  int slot = kInvalidSlot;
  int generation = 0;

  bool IsValid() const { return slot != kInvalidSlot; }
  bool operator==(const EntityId& _other) const = default;
};

}  // namespace siege_lab

#endif  // SIEGE_LAB_ENTITY_ID_H_
