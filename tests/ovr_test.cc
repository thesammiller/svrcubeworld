#include <gtest/gtest.h>

#include "../OVR_Bridge.h"

TEST(RandomFloatTest, BasicAssertions) {
  float random_a = RandomFloat();
  float random_b = RandomFloat();

  ASSERT_FALSE(random_a == random_b);
}

