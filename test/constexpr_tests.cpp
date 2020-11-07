#include <ostream>

#include <catch2/catch.hpp>

#include <bignum_template.hh>
#include <iostream>

using BigChar = BigNumT<unsigned char>;

static std::ostream &operator<<(std::ostream &os, const BigChar &b)
{
  return b.dumpTo(os);
}

TEST_CASE("BigNums can be initialized", "[Bignum]")
{
  REQUIRE(BigChar(0) == 0);
  REQUIRE(BigChar(1) == 1);
  REQUIRE(BigChar(-1) == -1);
  REQUIRE(BigChar(-1) != 255);
  REQUIRE(BigChar(-1) != 1);

  REQUIRE_FALSE(BigChar(1) == 0);
  REQUIRE_FALSE(BigChar(1) != 1);
}

TEST_CASE("BigNums equal longs")
{
  auto i = GENERATE(take(10, random(std::numeric_limits<long>::min(), std::numeric_limits<long>::max())));
  std::cout << i << '\n';

  REQUIRE(BigChar(i) == i);
  REQUIRE(BigChar(i) == BigChar(i));
  REQUIRE(BigChar(i) != BigChar(i + 0x100000000l));
}

TEST_CASE("If the longs don't equal, neither do the BigNums")
{
  auto i = GENERATE(take(100, random(std::numeric_limits<long>::min(), std::numeric_limits<long>::max())));
  auto j = GENERATE(take(100, random(std::numeric_limits<long>::min(), std::numeric_limits<long>::max())));

  REQUIRE((BigChar(i) == BigChar(j)) == (i == j));
}
