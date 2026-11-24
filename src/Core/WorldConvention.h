#pragma once

#include <cyclone/core.h>

// Fixed world convention shared by gameplay and tests (statement 00).
// +X toward tower, +Y up, +Z depth, ground at y = 0.
namespace SiegeLab
{
    inline constexpr cyclone::real kFixedDeltaTime = static_cast<cyclone::real>(1.0 / 60.0);

    cyclone::Vector3 GetGravity();
    cyclone::Vector3 GetLauncherPosition();
}
