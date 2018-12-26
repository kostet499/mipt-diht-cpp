#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#include <iostream>
#include <string>
#include <vector>


/*
    Contest ID: 18204042
*/

using std::istream;
using std::ostream;
using std::string;
using std::vector;


class BigInteger
{
public:

  BigInteger ();

  BigInteger (int);

  explicit BigInteger (string);

  BigInteger (const BigInteger &);

  BigInteger (BigInteger &&) noexcept;

  BigInteger &operator= (const BigInteger &);

  BigInteger &operator= (BigInteger &&) noexcept;

  friend BigInteger operator+ (const BigInteger &, const BigInteger &);

  BigInteger &operator+= (const BigInteger &);

  friend BigInteger operator- (const BigInteger &, const BigInteger &);

  BigInteger &operator-= (const BigInteger &);

  BigInteger operator- () const;

  friend BigInteger operator* (const BigInteger &, const BigInteger &);

  BigInteger &operator*= (const BigInteger &);

  friend BigInteger operator/ (const BigInteger &, const BigInteger &);

  BigInteger &operator/= (const BigInteger &);

  friend BigInteger operator% (const BigInteger &, const BigInteger &);

  BigInteger &operator%= (const BigInteger &);

  bool operator< (const BigInteger &) const;

  bool operator> (const BigInteger &) const;

  bool operator<= (const BigInteger &) const;

  bool operator>= (const BigInteger &) const;

  bool operator== (const BigInteger &) const;

  bool operator!= (const BigInteger &) const;

  BigInteger &operator++ ();

  const BigInteger operator++ (int);

  BigInteger &operator-- ();

  const BigInteger operator-- (int);

  explicit operator bool () const;

  explicit operator int () const;

  friend istream &operator>> (istream &, BigInteger &);

  friend ostream &operator<< (ostream &, const BigInteger &);

  string toString () const;

  BigInteger abs () const;

private:

  size_t size () const;

  int operator[] (size_t) const;

  vector<int>::iterator begin ();

  vector<int>::iterator end ();

  vector<int>::reverse_iterator rbegin ();

  vector<int>::reverse_iterator rend ();

  vector<int>::const_iterator cbegin () const;

  vector<int>::const_iterator cend () const;

  vector<int>::const_reverse_iterator crbegin () const;

  vector<int>::const_reverse_iterator crend () const;

  void removeLeadingZeros ();

  bool isEqualToZero () const;

  friend BigInteger computeMultiplicationNaive (const BigInteger &, const BigInteger &);

  friend std::pair<BigInteger, BigInteger> computeDivMod
      (const BigInteger &, const BigInteger &);

private:

  vector<int> big_int;

  bool is_negative = false;

  static const int BIG_INT_BASE = 1'000'000'000;
};


BigInteger::BigInteger ()
{
  big_int.push_back(0);
}

BigInteger::BigInteger (int number) :
    BigInteger(std::to_string(number))
{}

BigInteger::BigInteger (string number)
{
  if (!number.empty() && number[0] == '-')
  {
    is_negative = true;
    number.erase(number.begin());
  }
  else
  {
    is_negative = false;
  }
  for (int i = static_cast<int>(number.length()); i > 0; i -= 9)
  {
    if (i >= 9)
    {
      big_int.push_back(
          stoi(number.substr(static_cast<size_t>(i - 9), 9))
      );
    }
    else
    {
      big_int.push_back(
          stoi(number.substr(0, static_cast<size_t>(i)))
      );
    }
  }
  removeLeadingZeros();
}

BigInteger::BigInteger (const BigInteger &other)
{
  is_negative = other.is_negative;
  std::copy_backward(other.big_int.begin(), other.big_int.end(), big_int.begin());
}

BigInteger::BigInteger (BigInteger &&other) noexcept:
    big_int(std::move(other.big_int)),
    is_negative(other.is_negative)
{}

BigInteger &BigInteger::operator= (const BigInteger &other)
{
  if (this != &other)
  {
    is_negative = other.is_negative;
    std::copy_backward(other.big_int.begin(), other.big_int.end(), big_int.begin());
  }
  return *this;
}

BigInteger &BigInteger::operator= (BigInteger &&other) noexcept
{
  if (this != &other)
  {
    is_negative = other.is_negative;
    big_int = std::move(other.big_int);
  }
  return *this;
}

string BigInteger::toString () const
{
  string str_repr;
  for (int digit : big_int)
  {
    string base_digit = std::to_string(digit);
    base_digit = string(base_digit.rbegin(), base_digit.rend());
    while (base_digit.size() != 9)
    {
      base_digit += '0';
    }
    str_repr += base_digit;
  }
  while (str_repr.size() > 1 && str_repr.back() == '0')
  {
    str_repr.pop_back();
  }
  str_repr = string(str_repr.rbegin(), str_repr.rend());
  if (is_negative && str_repr != "0")
  {
    str_repr = '-' + str_repr;
  }
  return str_repr;
}

istream &operator>> (istream &in, BigInteger &bigint)
{
  string str_repr;
  in >> str_repr;
  bool is_negative = false;
  if (str_repr[0] == '-')
  {
    str_repr.erase(str_repr.begin());
    is_negative = true;
  }
  bigint = BigInteger(str_repr);
  bigint.is_negative = is_negative;
  return in;
}

ostream &operator<< (ostream &out, const BigInteger &bigint)
{
  out << bigint.toString();
  return out;
}

BigInteger operator+ (const BigInteger &lhs, const BigInteger &rhs)
{
  BigInteger result(lhs);
  result += rhs;
  result.removeLeadingZeros();
  return result;
}

BigInteger &BigInteger::operator+= (const BigInteger &other)
{
  if (!is_negative && other.is_negative)
  {
    return *this -= -other;
  }
  if (is_negative && !other.is_negative)
  {
    return *this = other - -(*this);
  }
  if (is_negative && other.is_negative)
  {
    *this = -((-(*this)) + (-other));
    return *this;
  }
  int carry = 0;
  auto it1 = big_int.begin();
  auto it2 = other.big_int.cbegin();

  for (; it1 != big_int.end() || it2 != other.big_int.cend() || carry; ++it1)
  {
    if (it1 == big_int.end())
    {
      big_int.push_back(0);
      it1 = big_int.end() - 1;
    }
    *it1 += carry + (it2 != other.big_int.cend() ? *(it2++) : 0);
    carry = *it1 >= BIG_INT_BASE;
    if (carry)
    {
      *it1 -= BIG_INT_BASE;
    }
  }
  removeLeadingZeros();
  return *this;
}

BigInteger operator- (const BigInteger &lhs, const BigInteger &rhs)
{
  BigInteger result(lhs);
  result -= rhs;
  result.removeLeadingZeros();
  return result;
}

BigInteger &BigInteger::operator-= (const BigInteger &other)
{
  if (!is_negative && other.is_negative)
  {
    return *this = *this + (-other);
  }
  if (is_negative && !other.is_negative)
  {
    *this = -((-(*this)) + (other));
    return *this;
  }
  if (is_negative && other.is_negative)
  {
    return *this = (*this) + (-other);
  }
  if (other > *this)
  {
    return *this = -(other - (*this));
  }
  int carry = 0;

  int other_len = static_cast<int>(other.big_int.size());
  for (int i = 0; i < other_len || carry; ++i)
  {
    big_int[i] -= carry + (i < other_len ? other[i] : 0);
    carry = big_int[i] < 0;
    if (carry)
    {
      big_int[i] += BIG_INT_BASE;
    }
  }
  removeLeadingZeros();
  return *this;
}

BigInteger BigInteger::operator- () const
{
  BigInteger res(*this);
  res.is_negative ^= true;
  return res;
}

BigInteger operator* (const BigInteger &lhs, const BigInteger &rhs)
{
  return computeMultiplicationNaive(lhs, rhs);
}

BigInteger &BigInteger::operator*= (const BigInteger &other)
{
  *this = *this * other;
  removeLeadingZeros();
  return *this;
}

BigInteger operator/ (const BigInteger &lhs, const BigInteger &rhs)
{
  return computeDivMod(lhs, rhs).first;
}

BigInteger &BigInteger::operator/= (const BigInteger &other)
{
  *this = *this / other;
  removeLeadingZeros();
  return *this;
}

BigInteger operator% (const BigInteger &lhs, const BigInteger &rhs)
{
  return computeDivMod(lhs, rhs).second;
}

BigInteger &BigInteger::operator%= (const BigInteger &other)
{
  *this = *this % other;
  removeLeadingZeros();
  return *this;
}

bool BigInteger::operator> (const BigInteger &other) const
{
  if (this->isEqualToZero() && other.isEqualToZero())
  {
    return false;
  }
  if (is_negative && other.is_negative)
  {
    return (-other) > (-(*this));
  }
  if (is_negative)
  {
    return false;
  }
  if (other.is_negative)
  {
    return true;
  }
  if (big_int.size() > other.big_int.size())
  {
    return true;
  }
  if (big_int.size() < other.big_int.size())
  {
    return false;
  }
  for (int i = (int) big_int.size() - 1; i >= 0; --i)
  {
    if (big_int[i] > other[i])
    {
      return true;
    }
    if (big_int[i] < other[i])
    {
      return false;
    }
  }
  return false;
}

bool BigInteger::operator< (const BigInteger &other) const
{
  return other > *this;
}

bool BigInteger::operator<= (const BigInteger &other) const
{
  return !(*this > other);
}

bool BigInteger::operator>= (const BigInteger &other) const
{
  return !(*this < other);
}

bool BigInteger::operator== (const BigInteger &other) const
{
  return !(*this < other) && !(*this > other);
}

bool BigInteger::operator!= (const BigInteger &other) const
{
  return !(*this == other);
}

BigInteger &BigInteger::operator++ ()
{
  *this += 1;
  removeLeadingZeros();
  return *this;
}

const BigInteger BigInteger::operator++ (int)
{
  BigInteger tmp(*this);
  ++(*this);
  removeLeadingZeros();
  return tmp;
}

BigInteger &BigInteger::operator-- ()
{
  *this -= 1;
  removeLeadingZeros();
  return *this;
}

const BigInteger BigInteger::operator-- (int)
{
  BigInteger tmp(*this);
  --(*this);
  removeLeadingZeros();
  return tmp;
}

BigInteger::operator bool () const
{
  for (auto digit: big_int)
  {
    if (digit != 0)
    {
      return true;
    }
  }
  return false;
}

BigInteger::operator int () const
{
  int result = 0;
  for (auto digit: big_int)
  {
    result *= BigInteger::BIG_INT_BASE;
    result += digit;
  }
  return result * (is_negative ? -1 : 1);
}

BigInteger BigInteger::abs () const
{
  BigInteger other(*this);
  other.is_negative = false;
  return other;
}

size_t BigInteger::size () const
{
  return big_int.size();
}

int BigInteger::operator[] (size_t index) const
{
  return big_int[index];
}

vector<int>::iterator BigInteger::begin ()
{
  return big_int.begin();
}

vector<int>::iterator BigInteger::end ()
{
  return big_int.end();
}

vector<int>::reverse_iterator BigInteger::rbegin ()
{
  return big_int.rbegin();
}

vector<int>::reverse_iterator BigInteger::rend ()
{
  return big_int.rend();
}

vector<int>::const_iterator BigInteger::cbegin () const
{
  return big_int.cbegin();
}

vector<int>::const_iterator BigInteger::cend () const
{
  return big_int.cend();
}

vector<int>::const_reverse_iterator BigInteger::crbegin () const
{
  return big_int.crbegin();
}

vector<int>::const_reverse_iterator BigInteger::crend () const
{
  return big_int.crend();
}

void BigInteger::removeLeadingZeros ()
{
  while (big_int.size() > 1 && big_int.back() == 0)
  {
    big_int.pop_back();
  }
}

bool BigInteger::isEqualToZero () const
{
  return !bool(*this);
}

BigInteger computeMultiplicationNaive (const BigInteger &lhs, const BigInteger &rhs)
{
  BigInteger result;
  size_t len_lhs = lhs.size();
  size_t len_rhs = rhs.size();
  result.is_negative = lhs.is_negative ^ rhs.is_negative;
  result.big_int.resize(len_lhs + len_rhs);
  for (size_t i = 0; i < len_lhs; ++i)
  {
    for (size_t j = 0, carry = 0; j < len_rhs || carry; ++j)
    {
      auto current = static_cast<long long>(
          result[i + j] + lhs[i] * 1LL * (j < len_rhs ? rhs[j] : 0) + carry
      );
      result.big_int[i + j] = static_cast<int>(
          current % BigInteger::BIG_INT_BASE
      );
      carry = static_cast<size_t>(current / BigInteger::BIG_INT_BASE);
    }
  }
  result.removeLeadingZeros();
  return result;
}

std::pair<BigInteger, BigInteger> computeDivMod
    (const BigInteger &lhs, const BigInteger &rhs)
{
  BigInteger lhs_abs(std::move(lhs.abs()));
  BigInteger rhs_abs(std::move(rhs.abs()));
  if (lhs_abs < rhs_abs)
  {
    return std::make_pair(0, lhs);
  }
  BigInteger division;
  size_t lhs_size = lhs.size(), rhs_size = rhs.size();
  size_t length_diff = lhs_size - rhs_size;
  BigInteger carry;
  carry.big_int.clear();
  for (size_t i = 1; i < rhs.size(); ++i)
  {
    carry.big_int.push_back(lhs[lhs_size - rhs_size + i]);
  }
  if (carry.size() == 0)
  {
    carry = 0;
  }
  int i = static_cast<int>(length_diff);
  do
  {
    carry *= BigInteger::BIG_INT_BASE;
    carry += lhs[i];
    int min_scale = 0;
    int max_scale = BigInteger::BIG_INT_BASE;
    while (max_scale - min_scale > 1)
    {
      int scale = (min_scale + max_scale) / 2;
      if (rhs_abs * scale > carry)
      {
        max_scale = scale;
      }
      else
      {
        min_scale = scale;
      }
    }
    carry -= min_scale * rhs_abs;
    division *= BigInteger::BIG_INT_BASE;
    division += min_scale;
    --i;
  } while (i >= 0);
  division.removeLeadingZeros();
  carry.removeLeadingZeros();
  division.is_negative = lhs.is_negative ^ rhs.is_negative;
  carry.is_negative = lhs.is_negative;
  return std::make_pair(division, carry);
}

#endif //BIGINTEGER_H
