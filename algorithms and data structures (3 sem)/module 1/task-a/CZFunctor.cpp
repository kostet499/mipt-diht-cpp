#include "CZFunctor.h"


void CZFunctor::operator() (std::istream &is, std::ostream &os,
                            const std::string &separator) const
{
  std::string pattern;
  is >> pattern;
  std::size_t pattern_size = pattern.size();
  std::vector<std::size_t> pattern_z_function(pattern_size, 0);
  computeFunction(pattern, "", pattern_z_function);
  std::vector<int> cyclic_buffer(pattern_size, 0);
  std::vector<char> source_substring(pattern_size);

  int source_index = 0;
  int cyclic_buffer_index = 0;
  std::size_t next_restored_z_value = pattern_size - 1;
  int left = 0, right = 0;

  for (std::size_t i = 0; i < pattern_size - 1; i++)
  {
    if (is.eof())
    {
      return;
    }
    is >> source_substring[i];
  }

  auto finding_max_matching = [&] () -> bool
  {
    bool is_suitable_size = cyclic_buffer[cyclic_buffer_index] < pattern_size;
    auto source_ind = (source_index + cyclic_buffer[cyclic_buffer_index]) % pattern_size;
    auto pattern_ind = cyclic_buffer[cyclic_buffer_index];
    bool last_char_match = (source_substring[source_ind] == pattern[pattern_ind]);
    return is_suitable_size && last_char_match;
  };

  while (is >> source_substring[next_restored_z_value])
  {
    cyclic_buffer[next_restored_z_value] = 0;
    cyclic_buffer_index = static_cast<int>(source_index % pattern_size);
    if (source_index <= right)
    {
      cyclic_buffer[cyclic_buffer_index] = std::min(
          static_cast<int>(pattern_z_function[source_index - left]),
          right - source_index + 1
      );
    }
    while (finding_max_matching())
    {
      cyclic_buffer[cyclic_buffer_index]++;
    }
    if (right < source_index + cyclic_buffer[cyclic_buffer_index])
    {
      left = source_index;
      right = source_index + cyclic_buffer[cyclic_buffer_index] - 1;
    }
    if (cyclic_buffer[cyclic_buffer_index] == pattern_size)
    {
      os << source_index << separator;
    }
    source_index++;
    next_restored_z_value++;
    next_restored_z_value %= pattern_size;
  }
}

void CZFunctor::computeFunction (const std::string &pattern, const std::string &source,
                                 std::vector<std::size_t> &function_values) const
{
  std::string text = pattern + IStringFunctor::STRING_DIVIDER + source;
  std::size_t text_size = text.size();
  std::size_t left = 0, right = 0;
  for (std::size_t i = 1; i < text_size; i++)
  {
    if (i <= right)
    {
      function_values[i] = std::min(right - i + 1, function_values[i - left]);
    }
    while (i + function_values[i] < text_size
           && text[function_values[i]] == text[i + function_values[i]])
    {
      function_values[i]++;
    }
    if (i + function_values[i] - 1 > right)
    {
      left = i;
      right = i + function_values[i] - 1;
    }
  }
}

void CZFunctor::computeMatchings (const std::string &pattern, const std::string &source,
                                  const std::vector<std::size_t> &function_values,
                                  std::vector<std::size_t> &matchings) const
{
  for (std::size_t i = 0; i < function_values.size(); i++)
  {
    if (function_values[i] == pattern.size())
    {
      matchings.emplace_back(i - pattern.size() - 1);
    }
  }
}