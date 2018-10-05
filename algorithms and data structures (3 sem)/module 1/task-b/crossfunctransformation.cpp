#include "crossfunctransformation.h"


void crossfunctransformation::ConvertZFunctionIntoPrefixFunction
    (const std::vector<int> &z_func_values,
     std::vector<int> &prefix_func_values)
{
  prefix_func_values.resize(z_func_values.size(), 0);
  for (int i = 1; i < z_func_values.size(); i++)
  {
    for (int j = z_func_values[i] - 1; j >= 0 && prefix_func_values[i + j] == 0; j--)
    {
      prefix_func_values[i + j] = j + 1;
    }
  }
}

void crossfunctransformation::ConvertPrefixFunctionIntoZFunction
    (const std::vector<int> &prefix_func_values,
     std::vector<int> &z_func_values)
{
  z_func_values.resize(prefix_func_values.size(), 0);
  for (int i = 0, z_position = 0; i < prefix_func_values.size(); i++)
  {
    z_position = i - std::max(prefix_func_values[i] - 1, 0);
    z_func_values[z_position] = std::max(z_func_values[z_position], prefix_func_values[i]);
  }
}

