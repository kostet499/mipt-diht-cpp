#ifndef TASK_B_ZFUNCTRANSFORMATION_H
#define TASK_B_ZFUNCTRANSFORMATION_H


#include <algorithm> // std::min
#include <string>    // std::string
#include <vector>    // std::vector


#include "crossfunctransformation.h"
#include "prefixfunctransformation.h"


namespace zfunctransformation
{
  void ConvertStringIntoZFunction
      (const std::string &text, std::vector<int> &z_func_values);

  std::string ConvertZFunctionIntoString
      (const std::vector<int> &z_func_values);
};

#endif //TASK_B_ZFUNCTRANSFORMATION_H
