#include "zfunctransformation.h"


void zfunctransformation::ConvertStringIntoZFunction
    (const std::string &text, std::vector<int> &z_func_values)
{
  int text_size = static_cast<int>(text.size())
  , left = 0
  , right = 0;
  z_func_values.resize(static_cast<std::size_t>(text_size), 0);
  z_func_values[0] = text_size;
  for (int i = 1; i < text_size; i++)
  {
    if (i <= right)
    {
      z_func_values[i] = std::min(right - i + 1, z_func_values[i - left]);
    }
    while (i + z_func_values[i] < text_size
           && text[z_func_values[i]] == text[i + z_func_values[i]])
    {
      z_func_values[i]++;
    }
    if (i + z_func_values[i] - 1 > right)
    {
      left = i;
      right = i + z_func_values[i] - 1;
    }
  }
}

std::string zfunctransformation::ConvertZFunctionIntoString
    (const std::vector<int> &z_func_values)
{
  std::vector<int> prefix_func_values;
  crossfunctransformation::ConvertZFunctionIntoPrefixFunction(
      z_func_values, prefix_func_values
  );
  return prefixfunctransformation::ConvertPrefixFunctionIntoString(
      prefix_func_values
  );
}
