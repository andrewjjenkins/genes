#include <gtest/gtest.h>

#include <common/basepair.h>

TEST(Basepair, toBasepair) {
  EXPECT_EQ(toBasepair('A'), Basepair::A);
}

TEST(Basepair, toChar) {
  EXPECT_EQ(toChar(Basepair::A), 'A');
}

TEST(Basepair, complement) {
  EXPECT_EQ(complement(Basepair::A), Basepair::T);
  EXPECT_EQ(complement(Basepair::T), Basepair::A);
  EXPECT_EQ(complement(Basepair::C), Basepair::G);
  EXPECT_EQ(complement(Basepair::G), Basepair::C);
  EXPECT_EQ(complement(complement(Basepair::G)), Basepair::G);
}
