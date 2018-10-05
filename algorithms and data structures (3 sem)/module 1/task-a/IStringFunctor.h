#ifndef TASK_A_ISTRINGFUNCTOR_H
#define TASK_A_ISTRINGFUNCTOR_H


#include <algorithm> // std::min
#include <cstddef>   // std::size_t
#include <string>    // std::string
#include <vector>    // std::vector


class IStringFunctor
{

public:

  virtual void operator() (const std::string &pattern,
                           const std::string &source,
                           std::vector<std::size_t> &matchings) const;

  virtual void operator() (std::istream &is, std::ostream &os,
                           const std::string &separator) const = 0;

protected:

  virtual void computeFunction (const std::string &pattern,
                                const std::string &source,
                                std::vector<std::size_t> &function_values) const = 0;

  virtual void computeMatchings (const std::string &pattern,
                                 const std::string &source,
                                 const std::vector<std::size_t> &function_values,
                                 std::vector<std::size_t> &matchings) const = 0;

protected:

  static std::string STRING_DIVIDER;

};


#endif //TASK_A_ISTRINGFUNCTOR_H
