#pragma once

#include <ostream>
#include <vector>

template<typename Base>
class [[nodiscard]] BigNumT
{
public:
  using Me = BigNumT<Base>;

  constexpr explicit BigNumT(long v) : value(fromLong(v)) {}
  constexpr BigNumT(std::initializer_list<Base> v) : value(v) {}

  [[nodiscard]] constexpr Base toBase() const { return value[0]; }

  std::ostream &dumpTo(std::ostream &os) const
  {
    os << "BigNum(";
    auto c = value.begin();
    if (c != value.end()) {
      os << static_cast<unsigned long>(*c);
      c++;
    }
    while (c != value.end()) {
      os << ", " << static_cast<unsigned long>(*c);
      c++;
    }

    return os << ')';
  }

  [[nodiscard]] bool negative() const
  {
    return *value.rbegin() >> (std::numeric_limits<Base>::digits - 1);
  }

  [[nodiscard]] bool positive() const { return !negative(); }

  [[nodiscard]] constexpr bool operator==(const Me &other) const
  {
    return value == other.value;
  }

  [[nodiscard]] constexpr bool operator!=(const Me &other) const
  {
    return !(other == *this);
  }

private:
  std::vector<Base> value;

  static std::vector<Base> fromLong(unsigned long value, bool positive)
  {
    std::vector<Base> result;
    while (value != 0) {
      result.push_back(toBase(value & std::numeric_limits<Base>::max()));
      value >>= std::numeric_limits<Base>::digits;
    }
    result.push_back(toBase(std::numeric_limits<Base>::max() * (1 - positive)));

    return result;
  }

  static std::vector<Base> fromLong(unsigned long value)
  {
    return fromLong(value, true);
  }

  static std::vector<Base> fromLong(long value)
  {
    return fromLong(static_cast<unsigned long>(value), value >= 0);
  }

  template<typename T>
  [[nodiscard]] constexpr static Base toBase(const T v)
  {
    return static_cast<Base>(v);
  }
};

template<typename Base>
constexpr bool operator==(const BigNumT<Base> &left, long right) { return left == BigNumT<Base>(right); }

template<typename Base>
constexpr bool operator==(long left, const BigNumT<Base> &right) { return right == left; }

template<typename Base>
constexpr bool operator!=(const BigNumT<Base> &left, long right) { return !(left == right); }

template<typename Base>
constexpr bool operator!=(long left, const BigNumT<Base> &right) { return right != left; }