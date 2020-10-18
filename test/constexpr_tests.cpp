#include <ostream>

#include <catch2/catch.hpp>

#include <bignum_template.hh>

using BigChar = BigNumT<unsigned char>;

static std::ostream& operator<<(std::ostream& os, const BigChar& b)
{
  return b.dumpTo(os);
}


TEST_CASE("BigNums can be initialized", "[Bignum]")
{
  REQUIRE(BigChar(0).toBase() == 0);
  REQUIRE(BigChar(1).toBase() == 1);

  REQUIRE(BigChar(0) == 0);
  REQUIRE(BigChar(1) == 1);
  REQUIRE(BigChar(-1) == -1);
  REQUIRE(BigChar(-1) != 255);
  REQUIRE(BigChar(-1) != 1);

  REQUIRE_FALSE(BigChar(1) == 0);
  REQUIRE_FALSE(BigChar(1) != 1);
}


constexpr unsigned int Factorial(unsigned int number)
{
  return number <= 1 ? number : Factorial(number - 1) * number;
}

TEST_CASE("Factorials are computed with constexpr", "[factorial]")
{
  STATIC_REQUIRE(Factorial(1) == 1);
  STATIC_REQUIRE(Factorial(2) == 2);
  STATIC_REQUIRE(Factorial(3) == 6);
  STATIC_REQUIRE(Factorial(10) == 3628800);
}
