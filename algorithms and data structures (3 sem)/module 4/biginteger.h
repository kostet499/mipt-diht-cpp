//
// Created by tsion on 04.07.2018.
//

#ifndef PROGTASKSHUB_BIGINTEGER_H
#define PROGTASKSHUB_BIGINTEGER_H

#include <iostream>
#include <string>
#include <vector>


using std::istream;
using std::ostream;
using std::string;
using std::vector;


class BigInteger
{

  friend BigInteger;

public:

  explicit BigInteger () :
      BigInteger(0) {}

  explicit BigInteger (int simple_int)
  {
    buffer.clear();
    if (simple_int < 0)
    {
      is_negative = true;
      simple_int = -simple_int;
    }
    do
    {
      buffer.emplace_back(simple_int % 10);
    } while (simple_int != 0);
  }

  explicit BigInteger (const string &strbuffer)
  {
    buffer.clear();
    if (*strbuffer.begin() == '-')
    {
      is_negative = true;
    }
    size_t index = is_negative ? 1 : 0;
    for (; index < strbuffer.size(); index++)
    {
      buffer.emplace_back((int) strbuffer[index] - 48);
    }
  }

  BigInteger (const BigInteger &rhs)
  {
    is_negative = rhs.is_negative;
    buffer.assign(rhs.buffer.begin(), rhs.buffer.end());
  }

  BigInteger (const BigInteger &&rhs) noexcept
      : BigInteger::BigInteger(rhs) {}

  BigInteger &operator= (const BigInteger &rhs)
  {
    is_negative = rhs.is_negative;
    buffer.assign(rhs.buffer.begin(), rhs.buffer.end());
    return *this;
  }

  ~BigInteger ()
  {
    buffer.clear();
  }


  BigInteger operator- () const
  {
    BigInteger tmp(*this);
    tmp.is_negative ^= true;
    return tmp;
  }

  BigInteger operator+ (const BigInteger &rhs) const
  {
    return BigInteger();
  }

  BigInteger operator- (const BigInteger &rhs) const
  {
    return BigInteger();
  }

  BigInteger operator* (const BigInteger &rhs) const
  {
    return BigInteger();
  }

  BigInteger operator/ (const BigInteger &rhs) const
  {
    return BigInteger();
  }

  BigInteger operator% (const BigInteger &rhs) const
  {

  }


  BigInteger &operator+= (const BigInteger &rhs) {}

  BigInteger &operator-= (const BigInteger &rhs) {}

  BigInteger &operator*= (const BigInteger &rhs) {}

  BigInteger &operator/= (const BigInteger &rhs) {}

  BigInteger &operator%= (const BigInteger &rhs) {}


  BigInteger &abs ()
  {
    is_negative = false;
    return *this;
  }


  BigInteger &operator++ ()
  {
    increaseByRest();
    return *this;
  }

  const BigInteger operator++ (int)
  {
    BigInteger tmp(*this);
    ++(*this);
    return tmp;
  }

  BigInteger &operator-- () {}

  const BigInteger operator-- (int) {}


  const string toString () const
  {
    string strbuffer = is_negative ? "-" : "";
    for (size_t idx = 0; idx < size(); idx++)
    {
      int digit = buffer[size() - 1 - idx];
      strbuffer.push_back((char) (digit + 48));
    }
    return strbuffer;
  }

  explicit operator bool () const
  {
    for (auto digit: buffer)
    {
      if (digit > 0)
      {
        return true;
      }
    }
    return false;
  }


  friend istream &operator>> (istream &is, BigInteger &lhs);

  friend ostream &operator<< (ostream &os, const BigInteger &lhs);


private:

  size_t size () const
  {
    return buffer.size();
  }

  void increaseByRest (int rest = 1, size_t idx = 0)
  {
    if (rest == 0)
    {
      return;
    }
    else if (idx >= size())
    {
      buffer.emplace_back(0);
    }
    buffer[idx] += rest;
    increaseByRest(buffer[idx] / 10, ++idx);
  }

  void decreaseByRest (int rest = 1, size_t idx = 0)
  {

  }


private:

  bool is_negative{};

  vector<int> buffer;

};


istream &operator>> (istream &is, BigInteger &lhs)
{
  string str_buffer;
  is >> str_buffer;
  lhs = BigInteger(str_buffer);
  return is;
}


ostream &operator<< (ostream &os, const BigInteger &lhs)
{
  std::string str_buffer;
  str_buffer = lhs.toString();
  os << str_buffer;
  return os;
}


#endif //PROGTASKSHUB_BIGINTEGER_H
