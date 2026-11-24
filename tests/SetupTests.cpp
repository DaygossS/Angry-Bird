#include <gtest/gtest.h>

#include "WorldConvention.h"

TEST(Setup, AdditionWorks)
{
    EXPECT_EQ(2 + 3, 5);
}

TEST(Setup, CoreLinksAgainstCyclone)
{
    const cyclone::Vector3 gravity = SiegeLab::GetGravity();
    EXPECT_FLOAT_EQ(static_cast<float>(gravity.y), -9.81f);
}
