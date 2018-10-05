#ifndef TASK_A_CZFUNCTOR_H
#define TASK_A_CZFUNCTOR_H


#include <iostream>  // std::istream, std::ostream


#include "IStringFunctor.h"


class CZFunctor : public IStringFunctor
{

public:

  void operator() (std::istream &is, std::ostream &os,
                   const std::string &separator) const override;

protected:

  void computeFunction (const std::string &pattern, const std::string &source,
                        std::vector<std::size_t> &function_values) const override;

  void computeMatchings (const std::string &pattern, const std::string &source,
                         const std::vector<std::size_t> &function_values,
                         std::vector<std::size_t> &matchings) const override;
};


#endif //TASK_A_CZFUNCTOR_H
