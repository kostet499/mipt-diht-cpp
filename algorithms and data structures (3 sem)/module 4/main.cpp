#include <iostream>

#include "biginteger.h"

int main ()
{
  BigInteger a();
  BigInteger b(5);
  BigInteger c("123243531325653758475367563656375625324134325326344786");
  std::cout << a << std::endl << b << std::endl << c;
  return 0;
}