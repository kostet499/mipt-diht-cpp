#ifndef TASK_B_CROSSFUNCTRANSFORMATION_H
#define TASK_B_CROSSFUNCTRANSFORMATION_H


#include <algorithm> // std::max
#include <vector>    // std::vector


namespace cross_transform
{
  void ConvertZFunctionIntoPrefixFunction
      (const std::vector<int> &z_func_values,
       std::vector<int> &prefix_func_values);

  void ConvertPrefixFunctionIntoZFunction
      (const std::vector<int> &prefix_func_values,
       std::vector<int> &z_func_values);
};

#endif //TASK_B_CROSSFUNCTRANSFORMATION_H
