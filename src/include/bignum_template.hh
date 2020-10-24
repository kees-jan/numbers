#pragma once

#include <ostream>

template<typename Base>
class [[nodiscard]] BigNumT {
public:
  using Me = BigNumT<Base>;

  constexpr BigNumT(long v) : positive(v>=0), value(static_cast<Base>(v)) {};
  [[nodiscard]] constexpr Base toBase() const { return value; }

  std::ostream &dumpTo(std::ostream & os) const
  {
    return os << "BigNum(" << (positive?'+':'-') << ", " << static_cast<long>(value) << ')';
  }

  constexpr bool operator==(const Me &other) const
  {
    return positive == other.positive && value == other.value;
  }

private:
  bool positive;
  Base value;
};

template<typename Base>
constexpr bool operator==(const BigNumT<Base> &left, long right)
{
  return left == BigNumT<Base>(right);
}

template<typename Base>
constexpr bool operator==(long left, const BigNumT<Base> &right)
{
  return right == left;
}

template<typename Base>
constexpr bool operator!=(const BigNumT<Base> &left, long right) { return !(left == right); }

template<typename Base>
constexpr bool operator!=(long left, const BigNumT<Base> &right) { return right != left; }