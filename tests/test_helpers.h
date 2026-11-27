#ifndef SIEGE_LAB_TESTS_TEST_HELPERS_H_
#define SIEGE_LAB_TESTS_TEST_HELPERS_H_

#include <gtest/gtest.h>

#include "raylib.h"

namespace siege_lab
{
namespace test
{

// Tolérances de l'énoncé (tests caméra) : 1e-3 m pour les positions.
constexpr float kPositionTolerance = 1e-3f;
constexpr float kAngleTolerance = 1e-3f;

// Compare deux Vector3 composante par composante.
inline void ExpectVector3Near(Vector3 _actual, Vector3 _expected,
                              float _tolerance)
{
  EXPECT_NEAR(_actual.x, _expected.x, _tolerance);
  EXPECT_NEAR(_actual.y, _expected.y, _tolerance);
  EXPECT_NEAR(_actual.z, _expected.z, _tolerance);
}

}  // namespace test
}  // namespace siege_lab

#endif  // SIEGE_LAB_TESTS_TEST_HELPERS_H_
