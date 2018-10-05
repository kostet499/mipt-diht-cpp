#ifndef TASK_B_PREFIXFUNCTRANSFORMATION_H
#define TASK_B_PREFIXFUNCTRANSFORMATION_H


#include <algorithm> // std::fill
#include <string>    // std::string
#include <vector>    // std::vector


namespace prefixfunctransformation
{
  void ConvertStringIntoPrefixFunction
      (const std::string &text, std::vector<int> &prefix_func_values);

  std::string ConvertPrefixFunctionIntoString
      (const std::vector<int> &prefix_func_values);
};

#endif //TASK_B_PREFIXFUNCTRANSFORMATION_H
