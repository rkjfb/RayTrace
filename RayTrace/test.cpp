#include "pch.h"
#include "Tuple.h"

TEST(TupleTest, IsVec) {
	Tuple t(4.3f, -4.2f, 3.1f, 1.0f);
	EXPECT_EQ(t.x, 4.3f);
  EXPECT_TRUE(true);
}