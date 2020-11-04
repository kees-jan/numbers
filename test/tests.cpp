#include <iostream>
#include <catch2/catch.hpp>

#include "bignum_template.hh"

using BigChar = BigNumT<unsigned char>;

static std::ostream &operator<<(std::ostream &os, const BigChar &b)
{
  return b.dumpTo(os);
}

TEST_CASE("BigNums can be initialized")
{
  REQUIRE(BigChar(0) == 0);
  REQUIRE(BigChar(1) == 1);
  REQUIRE(BigChar(-1) == -1);
  REQUIRE(BigChar(-1) != 255);
  REQUIRE(BigChar(-1) != 1);

  REQUIRE_FALSE(BigChar(1) == 0);
  REQUIRE_FALSE(BigChar(1) != 1);
}

TEST_CASE("Negative values")
{
  REQUIRE(BigChar(-1).negative());
  REQUIRE_FALSE(BigChar(-1).positive());

  REQUIRE(BigChar(0).positive());
  REQUIRE_FALSE(BigChar(0).negative());

  REQUIRE(BigChar(1).positive());
  REQUIRE_FALSE(BigChar(1).negative());
}

TEST_CASE("BigNums equal longs")
{
  auto i = GENERATE(take(10, random(std::numeric_limits<long>::min(), std::numeric_limits<long>::max())));

  REQUIRE(BigChar(i) == i);
  REQUIRE(BigChar(i) == BigChar(i));
  REQUIRE(BigChar(i) != BigChar(i + 0x100000000l));
}

SCENARIO("Binary operations")
{
  GIVEN("A long")
  {
    auto i = GENERATE(take(100, random(std::numeric_limits<long>::min() + 1, std::numeric_limits<long>::max())));

    CHECK(BigChar(i) == BigChar(i));
    CHECK_FALSE(BigChar(i) != BigChar(i));
    CHECK_FALSE(BigChar(i) < BigChar(i));

    AND_GIVEN("A smaller long")
    {
      auto j = GENERATE_COPY(take(100, random(std::numeric_limits<long>::min(), i - 1)));

      CHECK_FALSE(BigChar(i) == BigChar(j));
      CHECK(BigChar(i) != BigChar(j));
      //      CHECK(BigChar(j) < BigChar(i));
      //      CHECK_FALSE(BigChar(i) < BigChar(j));
    }
  }
}
