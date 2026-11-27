#ifndef SIEGE_LAB_OBJECT_POOL_H_
#define SIEGE_LAB_OBJECT_POOL_H_

#include <array>
#include <cstddef>
#include <optional>
#include <utility>

#include "entity_id.h"

namespace siege_lab
{

template <typename T, int kCapacity>
class ObjectPool
{
  static_assert(kCapacity > 0, "ObjectPool capacity must be positive");

 public:
  ObjectPool() = default;
  
  ObjectPool(const ObjectPool&) = delete;
  ObjectPool& operator=(const ObjectPool&) = delete;

  template <typename... Args>
  EntityId Create(Args&&... _args)
  {
    for (int slot = 0; slot < kCapacity; ++slot)
    {
      Slot& current = m_slots[ToIndex(slot)];
      if (!current.value.has_value())
      {
        current.value.emplace(std::forward<Args>(_args)...);
        ++m_live_count;
        return EntityId{slot, current.generation};
      }
    }
    return EntityId{};
  }

 
  T* Get(EntityId _id)
  {
    Slot* found = FindSlot(_id);
    return found != nullptr ? &*found->value : nullptr;
  }

  const T* Get(EntityId _id) const
  {
    const Slot* found = FindSlot(_id);
    return found != nullptr ? &*found->value : nullptr;
  }

  // false si l'id ne correspond à aucun objet vivant.
  bool Destroy(EntityId _id)
  {
    Slot* found = FindSlot(_id);
    if (found == nullptr)
    {
      return false;
    }
    found->value.reset();
    ++found->generation;
    --m_live_count;
    return true;
  }

  void Clear()
  {
    for (Slot& current : m_slots)
    {
      if (current.value.has_value())
      {
        current.value.reset();
        ++current.generation;
      }
    }
    m_live_count = 0;
  }

  template <typename Function>
  void ForEach(Function&& _function)
  {
    for (Slot& current : m_slots)
    {
      if (current.value.has_value())
      {
        _function(*current.value);
      }
    }
  }

  int GetLiveCount() const { return m_live_count; }
  static constexpr int GetCapacity() { return kCapacity; }

 private:
  struct Slot
  {
    std::optional<T> value;
    int generation = 0;
  };

  static std::size_t ToIndex(int _slot)
  {
    return static_cast<std::size_t>(_slot);
  }

  bool IsLive(EntityId _id) const
  {
    if (_id.slot < 0 || _id.slot >= kCapacity)
    {
      return false;
    }
    const Slot& current = m_slots[ToIndex(_id.slot)];
    return current.value.has_value()
        && current.generation == _id.generation;
  }

  Slot* FindSlot(EntityId _id)
  {
    return IsLive(_id) ? &m_slots[ToIndex(_id.slot)] : nullptr;
  }

  const Slot* FindSlot(EntityId _id) const
  {
    return IsLive(_id) ? &m_slots[ToIndex(_id.slot)] : nullptr;
  }

  std::array<Slot, static_cast<std::size_t>(kCapacity)> m_slots{};
  int m_live_count = 0;
};

}  // namespace siege_lab

#endif  // SIEGE_LAB_OBJECT_POOL_H_
