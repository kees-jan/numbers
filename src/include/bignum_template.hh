#pragma once

#include <ostream>
#include <vector>
// #include "magnitude.hh"

template<typename Base>
class [[nodiscard]] BigNumT
{
public:
  using Me = BigNumT<Base>;

  explicit BigNumT(long v) : isPositive(v >= 0), value(fromLong(static_cast<unsigned long>(std::abs(v)))) {}
  [[maybe_unused]] BigNumT(std::initializer_list<Base> v) : isPositive(true), value(v) {}

  std::ostream &dumpTo(std::ostream &os) const
  {
    os << "BigNum(" << (isPositive ? '+' : '-') << ", ";
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
    return !isPositive;
  }

  [[nodiscard]] bool positive() const
  {
    return isPositive;
  }

  [[nodiscard]] bool operator==(const Me &other) const
  {
    return value == other.value && isPositive == other.isPositive;
  }

  [[nodiscard]] bool operator!=(const Me &other) const
  {
    return !(other == *this);//NOLINT:(Simplify)
  }

  [[nodiscard]] bool operator<(const Me &other) const
  {
    if (isPositive) {
      if (other.isPositive) {
        return magnitude_compare(value, other.value) < 0;
      }
      return false;
    }
    if (other.isPositive) {
      return true;
    }
    return magnitude_compare(value, other.value) > 0;
  }

private:
  bool isPositive;
  std::vector<Base> value;

  [[nodiscard]] static std::vector<Base> fromLong(unsigned long value)
  {
    std::vector<Base> result;
    while (value != 0) {
      result.push_back(toBase(value & std::numeric_limits<Base>::max()));
      value >>= std::numeric_limits<Base>::digits;
    }

    return result;
  }

  [[nodiscard]] static int magnitude_compare(const std::vector<Base> &left, const std::vector<Base> &right)
  {
    auto cleft = left.begin();
    auto cright = right.begin();
    while (cleft != left.end() && cright != right.end() && *cleft == *cright) {
      ++cleft;
      ++cright;
    }
    if (cleft == left.end() && cright == right.end()) {
      return 0;
    }
    if (cleft != left.end() && cright != right.end()) {
      return *cleft - *cright;
    }
    if (cleft != left.end()) {
      if (all_zero(cleft, left.end())) {
        return 0;
      }
      return 1;
    }
    if (all_zero(cright, right.end())) {
      return 0;
    }
    return -1;
  }

  [[nodiscard]] static bool all_zero(typename std::vector<Base>::const_iterator cur, typename std::vector<Base>::const_iterator end)
  {
    while (cur != end && *cur == 0) {
      ++cur;
    }
    return cur == end;
  }

  template<typename T>
  [[nodiscard]] static Base toBase(const T v)
  {
    return static_cast<Base>(v);
  }
};

template<typename Base>
bool operator==(const BigNumT<Base> &left, long right) { return left == BigNumT<Base>(right); }

template<typename Base>
bool operator==(long left, const BigNumT<Base> &right) { return right == left; }

template<typename Base>
bool operator!=(const BigNumT<Base> &left, long right) { return !(left == right); }

template<typename Base>
bool operator!=(long left, const BigNumT<Base> &right) { return right != left; }