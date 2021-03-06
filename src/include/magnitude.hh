#pragma once

#include <ostream>
#include <vector>
#include <utility>

#include <boost/operators.hpp>

namespace Detail {

template<typename Base>
class [[nodiscard]] Magnitude : public boost::addable<Magnitude<Base>>
  , public boost::subtractable<Magnitude<Base>>
  , public boost::multipliable<Magnitude<Base>>
{
public:
  using Me = Magnitude<Base>;

  Magnitude() { assert(isNormalized()); }// Initialize as 0
  explicit Magnitude(unsigned long v) : value(fromLong(v)) { assert(isNormalized()); }

  Magnitude(std::initializer_list<Base> i) : value(i)
  {
    normalize();
    assert(isNormalized());
  }

  std::ostream &dumpTo(std::ostream &os) const
  {
    auto c = value.begin();
    if (c != value.end()) {
      os << static_cast<unsigned long>(*c);
      c++;
    } else {
      os << 0;
    }
    while (c != value.end()) {
      os << ", " << static_cast<unsigned long>(*c);
      c++;
    }

    return os;
  }

  [[nodiscard]] long operator<=>(const Me &other) const
  {
    return compare(*this, other);
  }

  [[nodiscard]] bool operator==(const Me &other) const
  {
    assert(isNormalized());
    assert(other.isNormalized());

    return value == other.value;
  }

  [[nodiscard]] bool operator!=(const Me &other) const
  {
    return !(other == *this);//NOLINT:(Simplify)
  }

  Me &operator++()
  {
    unsigned long long carry = 1;
    for (auto &v : value) {
      const unsigned long long r = carry + v;
      v = static_cast<Base>(r & mask);
      carry = r >> size;
    }

    // We possibly have a carry
    if (carry != 0U) {
      assert(carry <= mask);
      value.push_back(static_cast<Base>(carry));
    }

    return *this;
  }

  Me &operator--()
  {
    assert(isNormalized() && !value.empty());
    long long carry = -1;
    for (auto &v : value) {
      long long r = carry + v;
      v = static_cast<Base>(r & mask);
      carry = r >> size;
    }

    normalize();

    return *this;
  }

  [[nodiscard]] Me operator++(int) &// NOLINT: cert-dcl21-cpp
  {
    Me result(*this);
    ++*this;
    return result;
  }

  [[nodiscard]] Me operator--(int) &// NOLINT: cert-dcl21-cpp
  {
    Me result(*this);
    --*this;
    return result;
  }

  Me &operator+=(const Me &other)
  {
    value.reserve(std::max(value.size(), 1 + other.value.size()));

    unsigned long long carry = 0;
    auto ileft = value.begin();
    auto iright = other.value.begin();
    for (; ileft != value.end() && iright != other.value.end(); ++ileft, ++iright) {
      const unsigned long long r = carry + *ileft + *iright;
      *ileft = static_cast<Base>(r & mask);
      carry = r >> size;
    }

    // Either ileft or iright is at its end
    for (; ileft != value.end() && carry != 0; ++ileft) {
      const unsigned long long r = carry + *ileft;
      *ileft = static_cast<Base>(r & mask);
      carry = r >> size;
    }
    for (; iright != other.value.end(); ++iright) {
      const unsigned long long r = carry + *iright;
      value.push_back(static_cast<Base>(r & mask));
      carry = r >> size;
    }

    // We possibly have a carry
    if (carry != 0U) {
      assert(carry <= mask);
      value.push_back(static_cast<Base>(carry));
    }

    return *this;
  }

  Me &operator-=(const Me &other)
  {
    assert(other <= *this);

    long long carry = 0;
    auto ileft = value.begin();
    auto iright = other.value.begin();
    for (; iright != other.value.end(); ++ileft, ++iright) {
      assert(ileft != value.end());

      const long long r = *ileft - *iright + carry;
      *ileft = static_cast<Base>(r & mask);
      carry = r >> size;
    }

    for (; ileft != value.end() && carry != 0; ++ileft) {
      const long long r = *ileft + carry;
      *ileft = static_cast<Base>(r & mask);
      carry = r >> size;
    }

    // right <= left, so no carry at this point.
    assert(carry == 0);

    normalize();

    return *this;
  }

  Me &operator*=(const Me &right)
  {
    const Me left(std::move(*this));
    value.clear();
    value.reserve(left.value.size() + right.value.size() + 1);

    for (unsigned i = 0; i < right.value.size(); i++) {
      *this += mul(left, right.value[i], i);
    }

    return *this;
  }

private:
  std::vector<Base> value;
  static constexpr const Base mask = std::numeric_limits<Base>::max();
  static constexpr const Base size = std::numeric_limits<Base>::digits;

  explicit Magnitude(std::vector<Base> &&v) : value(std::move(v)) { assert(isNormalized()); }

  static long compare(const Me &left, const Me &right)
  {
    if (left.value.size() != right.value.size()) {
      return static_cast<long>(left.value.size()) - static_cast<long>(right.value.size());
    }

    auto ileft = left.value.rbegin();
    auto iright = right.value.rbegin();
    while (ileft != left.value.rend() && *ileft == *iright) {
      ++ileft;
      ++iright;
    }

    if (ileft == left.value.rend()) {
      return 0;
    }

    return *ileft - *iright;
  }

  Me &normalize()
  {
    while (!value.empty() && *value.rbegin() == 0) {
      value.pop_back();
    }

    return *this;
  }

  [[nodiscard]] bool isNormalized() const
  {
    return value.empty() || *value.rbegin() != 0;
  }

  [[nodiscard]] static std::vector<Base> fromLong(unsigned long value)
  {
    std::vector<Base> result;
    while (value != 0) {
      result.push_back(toBase(value & mask));
      value >>= size;
    }

    return result;
  }

  template<typename T>
  [[nodiscard]] static Base toBase(const T v)
  {
    return static_cast<Base>(v);
  }

  static Me mul(const Me &left, const Base right, const unsigned offset)
  {
    Me result;

    if (right != 0) {
      result.value.reserve(offset + left.value.size() + 1);
      fill_n(back_inserter(result.value), offset, 0);

      unsigned long long carry = 0;
      for (auto v : left.value) {
        const unsigned long long r = carry + v * right;
        result.value.push_back(r & mask);
        carry = r >> size;
      }
      while (carry != 0U) {
        result.value.push_back(carry & mask);
        carry >>= size;
      }
    }

    return result;
  }
};

}// namespace Detail
