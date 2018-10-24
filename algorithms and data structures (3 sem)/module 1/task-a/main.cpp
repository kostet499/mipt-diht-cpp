/* ContestID: 12224903 */

#include <iostream>         // std::ios, std::cin, std::cout

#include "IStringFunctor.h" // IStringFunctor
#include "CZFunctor.h"      // CZFunctor


void HandleRequest (std::istream &is, std::ostream &os,
                    const IStringFunctor &functor, bool optimized = false);

int main ()
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  CZFunctor functor;
  HandleRequest(std::cin, std::cout, functor, true);
  return 0;
}

void HandleRequest (std::istream &is, std::ostream &os,
                    const IStringFunctor &functor, bool optimized)
{
  std::string separator = " ";
  if (optimized)
  {
    functor(is, os, separator);
  }
  else
  {
    std::string pattern;
    std::string source;
    is >> pattern >> source;
    std::vector<std::size_t> matchings;
    functor(pattern, source, matchings);
    for (auto position: matchings)
    {
      os << position << separator;
    }
  }
}