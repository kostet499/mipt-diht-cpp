#include <cstddef>   // std::size_t
#include <iostream>  // std::cin, std::cout
#include <stdexcept> // std::runtime_error
#include <string>    // std::string
#include <vector>    // std::vector


class IStringFunctor
{

public:

  virtual void operator() (std::vector<std::size_t> &matching) const = 0;

  virtual void operator() (const std::string &pattern,
                           const std::string &source,
                           std::vector<std::size_t> &matching) = 0;

  virtual void Reset (const std::string &pattern, const std::string &source) = 0;

protected:

  std::string pattern = "";
  std::string source = "";

};


class CPrefixFunctor : public IStringFunctor
{

public:

  void operator() (std::vector<std::size_t> &matching) const override;

  void operator() (const std::string &pattern,
                   const std::string &source,
                   std::vector<std::size_t> &matching) override;

  void Reset (const std::string &pattern, const std::string &source) override;
};


class CZetaFunctor : public IStringFunctor
{

public:

  void operator() (std::vector<std::size_t> &matching) const override;

  void operator() (const std::string &pattern,
                   const std::string &source,
                   std::vector<std::size_t> &matching) override;

  void Reset (const std::string &pattern, const std::string &source) override;
};


void HandleRequest (std::istream &is, std::ostream &os, IStringFunctor &functor);


int main ()
{
  CPrefixFunctor functor;
  // CZetaFunctor functor;
  HandleRequest(std::cin, std::cout, functor);
  return 0;
}


void CPrefixFunctor::operator() (std::vector<std::size_t> &matching) const
{
  if (pattern.empty() || source.empty())
  {
    throw std::invalid_argument(R"("pattern" & "source" should be not empty)");
  }

}

void CPrefixFunctor::operator() (const std::string &pattern,
                                 const std::string &source,
                                 std::vector<std::size_t> &matching)
{
  Reset(pattern, source);
  this->operator()(matching);
}

void CPrefixFunctor::Reset (const std::string &pattern, const std::string &source)
{
  this->pattern = pattern;
  this->source = source;
}


void CZetaFunctor::operator() (std::vector<std::size_t> &matching) const
{
  if (pattern.empty() || source.empty())
  {
    throw std::invalid_argument(R"("pattern" & "source" should be not empty)");
  }
}

void CZetaFunctor::operator() (const std::string &pattern,
                            const std::string &source,
                            std::vector<std::size_t> &matching)
{
  Reset(pattern, source);
  this->operator()(matching);
}

void CZetaFunctor::Reset (const std::string &pattern, const std::string &source)
{
  this->pattern = pattern;
  this->source = source;
}


void HandleRequest (std::istream &is, std::ostream &os, IStringFunctor &functor)
{
  std::string pattern;
  std::string source;
  is >> pattern >> source;
  pattern = "";
  std::vector<std::size_t> matching;
  functor.Reset(pattern, source);
  functor(pattern, source, matching);
  for (auto position: matching)
  {
    os << position << " ";
  }
}
