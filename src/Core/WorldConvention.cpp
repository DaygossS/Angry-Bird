#include "WorldConvention.h"

namespace SiegeLab
{
    cyclone::Vector3 GetGravity()
    {
        return cyclone::Vector3(0, static_cast<cyclone::real>(-9.81), 0);
    }

    cyclone::Vector3 GetLauncherPosition()
    {
        return cyclone::Vector3(-8, 1, 0);
    }
}
