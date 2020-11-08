#include <iostream>
#include <catch2/catch.hpp>

#include "magnitude.hh"

using MagChar = Detail::Magnitude<unsigned char>;

namespace Detail {
template<typename Base>
static std::ostream &operator<<(std::ostream &os, const Magnitude<Base> &b)
{
  return b.dumpTo(os);
}
}// namespace Detail

namespace std {
template<typename T>
static std::ostream &operator<<(std::ostream &os, const std::vector<T> &c)
{
  os << "{";
  auto cur = c.begin();
  if (cur != c.end()) {
    os << *cur;
    cur++;
  }
  while (cur != c.end()) {
    os << ", " << *cur;
    cur++;
  }
  return os << "}";
}
}// namespace std

SCENARIO("Magnitude binary operations")
{
  GIVEN("An int")
  {
    const auto i = GENERATE(0xFFu, 0x100u, 0xFFFFu, 0x10000u, 0xFF0FFFu, 0xFFFFFFu);
    const MagChar mi(i);

    CHECK(mi == mi);
    CHECK_FALSE(mi != mi);
    CHECK(mi != MagChar(i + 0x100000000u));
    CHECK_FALSE(mi == MagChar(i + 0x100000000u));

    CHECK_FALSE(mi < mi);
    CHECK(mi <= mi);
    CHECK_FALSE(mi > mi);
    CHECK(mi >= mi);

    const auto zero = 0u;
    const auto one = 1u;
    const auto j = i + one;

    const MagChar mzero(zero);
    const MagChar mone(one);
    const MagChar mj(j);

    CHECK_FALSE(mi == mj);
    CHECK(mi != mj);

    CHECK(mi < mj);
    CHECK_FALSE(mj < mi);
    CHECK(mi <= mj);
    CHECK_FALSE(mj <= mi);
    CHECK_FALSE(mi > mj);
    CHECK(mj > mi);
    CHECK_FALSE(mi >= mj);
    CHECK(mj >= mi);

    CHECK(mi + mzero == mi);
    CHECK(mi + mone == mj);
    CHECK(mzero + mi == mi);
    CHECK(mone + mi == mj);
    {
      auto copy(mi);
      // CHECK(++copy == mj);
    }
  }
}
