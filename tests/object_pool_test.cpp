// Tests P01 à P05 de l'énoncé (GP-T1 — Generic pool).
// Chaque test suit le schéma de l'énoncé :
//   Prepare (préparer) -> Run (agir) -> Check (vérifier).

#include <gtest/gtest.h>

#include "entity_id.h"
#include "object_pool.h"

namespace siege_lab
{
namespace
{

TEST(Pool, CapacityLimit)
{
  // Prepare
  ObjectPool<int, 3> pool;

  // Run
  const EntityId id_a = pool.Create(10);
  const EntityId id_b = pool.Create(20);
  const EntityId id_c = pool.Create(30);
  const EntityId id_d = pool.Create(40);

  // Check
  ASSERT_TRUE(id_a.IsValid());
  ASSERT_TRUE(id_b.IsValid());
  ASSERT_TRUE(id_c.IsValid());
  EXPECT_FALSE(id_d.IsValid());
  EXPECT_EQ(*pool.Get(id_a), 10);
  EXPECT_EQ(*pool.Get(id_b), 20);
  EXPECT_EQ(*pool.Get(id_c), 30);
  EXPECT_EQ(pool.GetLiveCount(), 3);
}

TEST(Pool, RecycledSlotChangesGeneration)
{
  // Prepare
  ObjectPool<int, 1> pool;
  const EntityId id_a = pool.Create(7);

  // Run
  pool.Destroy(id_a);
  const EntityId id_b = pool.Create(42);

  // Check
  EXPECT_EQ(id_b.slot, 0);
  EXPECT_EQ(id_b.slot, id_a.slot);
  EXPECT_NE(id_b.generation, id_a.generation);
  EXPECT_NE(id_b, id_a);
}

TEST(Pool, StaleHandleRejected)
{
  // Prepare
  ObjectPool<int, 1> pool;
  const EntityId id_a = pool.Create(7);
  pool.Destroy(id_a);
  const EntityId id_b = pool.Create(42);

  // Run
  const int* found_a = pool.Get(id_a);
  const int* found_b = pool.Get(id_b);

  // Check
  EXPECT_EQ(found_a, nullptr);
  ASSERT_NE(found_b, nullptr);
  EXPECT_EQ(*found_b, 42);
}

TEST(Pool, LiveAddressStable)
{
  // Prepare
  ObjectPool<int, 3> pool;
  const EntityId id_a = pool.Create(10);
  const int* address_before = pool.Get(id_a);
  pool.Create(20);
  pool.Create(30);

  // Run
  const int* address_after = pool.Get(id_a);

  // Check
  EXPECT_EQ(address_after, address_before);
  EXPECT_EQ(*address_after, 10);
}

TEST(Pool, ClearInvalidatesHandles)
{
  // Prepare
  ObjectPool<int, 2> pool;
  const EntityId id_a = pool.Create(10);
  const EntityId id_b = pool.Create(20);

  // Run
  pool.Clear();
  const bool a_still_valid = pool.Get(id_a) != nullptr;
  const bool b_still_valid = pool.Get(id_b) != nullptr;
  const int live_after_clear = pool.GetLiveCount();
  const EntityId id_c = pool.Create(30);

  // Check
  EXPECT_FALSE(a_still_valid);
  EXPECT_FALSE(b_still_valid);
  EXPECT_EQ(live_after_clear, 0);
  ASSERT_TRUE(id_c.IsValid());
  EXPECT_EQ(*pool.Get(id_c), 30);
}

}  // namespace
}  // namespace siege_lab
