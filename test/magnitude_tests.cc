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

namespace std {// NOLINT: cert-dcl58-cpp
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
    CHECK(mi != MagChar(i + 0x100000000U));
    CHECK_FALSE(mi == MagChar(i + 0x100000000U));

    CHECK_FALSE(mi < mi);
    CHECK(mi <= mi);
    CHECK_FALSE(mi > mi);
    CHECK(mi >= mi);

    const auto zero = 0U;
    const auto one = 1U;
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
    CHECK(mi - mzero == mi);
    CHECK(mj - mone == mi);
    CHECK(mj - mi == mone);
    CHECK(mi - mi == mzero);
    {
      auto copy(mi);
      CHECK(&++copy == &copy);
      CHECK(copy == mj);
    }
    {
      auto copy(mi);
      CHECK(copy++ == mi);
      CHECK(copy == mj);
    }
    {
      auto copy(mi);
      CHECK(&(copy += mone) == &copy);
      CHECK(copy == mj);
    }
    {
      auto copy(mj);
      CHECK(&--copy == &copy);
      CHECK(copy == mi);
    }
    {
      auto copy(mj);
      CHECK(copy-- == mj);
      CHECK(copy == mi);
    }
    {
      auto copy(mj);
      CHECK(&(copy -= mone) == &copy);
      CHECK(copy == mi);
    }
  }
}

SCENARIO("Magnitude multiplication")
{
  const MagChar mzero(0);
  const MagChar mone(1);
  const MagChar mtwo(2);

  CHECK(mzero * mzero == mzero);
  CHECK(mzero * mone == mzero);
  CHECK(mone * mzero == mzero);
  CHECK(mone * mone == mone);
  CHECK(mone * mtwo == mtwo);
  CHECK(mtwo * mone == mtwo);
  CHECK(mtwo * mtwo == MagChar(4));

  CHECK(MagChar(128) * mtwo == MagChar(256));
  CHECK(MagChar(256) * MagChar(256) == MagChar(256 * 256));
}