#include <algorithm> // std::min
#include <cstddef>   // std::size_t
#include <fstream>   // std::ifstream, std::ofstream
#include <iostream>  // std::cin, std::cout
#include <stdexcept> // std::invalid_argument
#include <string>    // std::string
#include <vector>    // std::vector


class IStringFunctor
{

public:

  virtual void operator() (std::vector<std::size_t> &matching) const = 0;

  virtual void operator() (const std::string &pattern,
                           const std::string &source,
                           std::vector<std::size_t> &matching);

  virtual void Reset (const std::string &pattern, const std::string &source);

protected:

  virtual void validate () const;

protected:

  std::string pattern = "";
  std::string source = "";

  static std::string STRING_DIVIDER;

};


std::string IStringFunctor::STRING_DIVIDER = "$";


class CPrefixFunctor : public IStringFunctor
{

public:

  void operator() (std::vector<std::size_t> &matching) const override;
};


class CZetaFunctor : public IStringFunctor
{

public:

  void operator() (std::vector<std::size_t> &matching) const override;

};


void
HandleRequest (std::istream &is, std::ostream &os, IStringFunctor &functor);


int main ()
{
  CPrefixFunctor functor;
  //CZetaFunctor functor;
  //HandleRequest(std::cin, std::cout, functor);
  std::ifstream ifs;
  ifs.open("input.txt");
  std::ofstream ofs;
  ofs.open("output.txt");
  HandleRequest(ifs, ofs, functor);
  ifs.close();
  ofs.close();
  return 0;
}


void
IStringFunctor::operator() (const std::string &pattern,
                            const std::string &source,
                            std::vector<std::size_t> &matching)
{
  Reset(pattern, source);
  this->operator()(matching);
}

void
IStringFunctor::Reset (const std::string &pattern, const std::string &source)
{
  this->pattern = pattern;
  this->source = source;
}

void IStringFunctor::validate () const
{
  if (pattern.empty() || source.empty())
  {
    throw std::invalid_argument(R"("pattern" & "source" should be not empty)");
  }
}


void CPrefixFunctor::operator() (std::vector<std::size_t> &matching) const
{
  IStringFunctor::validate();

  std::string s = pattern + IStringFunctor::STRING_DIVIDER + source;
  std::size_t n = s.size();
  std::vector<std::size_t> pi(n, 0);
  for (std::size_t i = 1; i < n; i++)
  {
    std::size_t j = pi[i - 1];
    while (j > 0 && s[i] != s[j])
    {
      j = pi[j - 1];
    }
    if (s[i] == s[j])
    {
      pi[i] = j + 1;
    }
  }
  for (std::size_t i = pattern.size(); i < n; i++)
  {
    if (pi[i] == pattern.size())
    {
      matching.emplace_back(i - 2 * pattern.size());
    }
  }
}


void CZetaFunctor::operator() (std::vector<std::size_t> &matching) const
{
  IStringFunctor::validate();

  std::string s = pattern + IStringFunctor::STRING_DIVIDER + source;
  std::size_t n = s.size();
  std::vector<std::size_t> zeta(n, 0);
  std::size_t l = 0, r = 0;
  for (std::size_t i = 1; i < n; i++)
  {
    if (i <= r)
    {
      zeta[i] = std::min(r - i + 1, zeta[i - l]);
    }
    while (i + zeta[i] < n && s[zeta[i]] == s[i + zeta[i]])
    {
      zeta[i] += 1;
    }
    if (i + zeta[i] - 1 > r)
    {
      l = i;
      r = i + zeta[i] - 1;
    }
  }
  for (std::size_t i = 0; i < zeta.size(); i++)
  {
    if (zeta[i] == pattern.size())
    {
      matching.emplace_back(i - pattern.size() - 1);
    }
  }
}


void HandleRequest (std::istream &is, std::ostream &os, IStringFunctor &functor)
{
  std::string pattern;
  std::string source;
  is >> pattern >> source;
  std::vector<std::size_t> matching;
  functor.Reset(pattern, source);
  functor(pattern, source, matching);
  for (const auto &position: matching)
  {
    os << position << " ";
  }
}
