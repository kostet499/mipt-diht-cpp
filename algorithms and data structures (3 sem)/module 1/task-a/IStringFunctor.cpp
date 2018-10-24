#include "IStringFunctor.h"

std::string IStringFunctor::STRING_DIVIDER = "$";

void IStringFunctor::operator() (const std::string &pattern,
                                 const std::string &source,
                                 std::vector<std::size_t> &matchings) const
{
  std::size_t text_size = pattern.size()
                          + IStringFunctor::STRING_DIVIDER.size() + source.size();
  std::vector<std::size_t> function_values(text_size, 0);
  computeFunction(pattern, source, function_values);
  matchings.clear();
  computeMatchings(pattern, source, function_values, matchings);
}