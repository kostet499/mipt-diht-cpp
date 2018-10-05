/* ContestID: 12358641 (2A) */
/* ContestID: 12358650 (2B) */

#include <iostream>  // std::cin, std::cout, std::cerr
#include <stdexcept> // std::invalid_argument
#include <vector>    // std::vector


#include "prefixfunctransformation.h"
#include "zfunctransformation.h"


void Read (std::vector<int> &func_values);

int main ()
{
  std::vector<int> func_values;
  Read(func_values);
  //std::cout << prefixfunctransformation::ConvertPrefixFunctionIntoString(func_values);
  //std::cout << zfunctransformation::ConvertZFunctionIntoString(func_values);
  return 0;
}

void Read (std::vector<int> &func_values)
{
  int function_value = 0;
  while (std::cin >> function_value)
  {
    if (function_value < 0)
    {
      std::cerr << "Value of [prefix-|z-]function can not be negative: "
                << function_value << "\n";
      throw std::invalid_argument("Negative value of [prefix-|z-]function");
    }
    func_values.emplace_back(function_value);
  }
}








