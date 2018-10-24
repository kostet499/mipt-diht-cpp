#include "prefix_func.h"


static const int ALPHABET_SIZE = 26;


void prefix_func::ConvertStringIntoPrefixFunction
    (const std::string &text, std::vector<int> &prefix_func_values)
{
  int text_size = static_cast<int>(text.size());
  prefix_func_values.resize(static_cast<unsigned long>(text_size), 0);
  for (int i = 1; i < text_size; i++)
  {
    int j = prefix_func_values[i - 1];
    while (j > 0 && text[i] != text[j])
    {
      j = prefix_func_values[j - 1];
    }
    if (text[i] == text[j])
    {
      prefix_func_values[i] = j + 1;
    }
  }
}

std::string prefix_func::ConvertPrefixFunctionIntoString
    (const std::vector<int> &prefix_func_values)
{
  std::string answer;
  std::vector<bool> used(ALPHABET_SIZE);
  auto getcode = [](const char &letter) -> int
  {
    return static_cast<int>(letter - 'a');
  };
  for (int i = 0; i < prefix_func_values.size(); i++)
  {
    if (i == 0)
    {
      answer = 'a';
    }
    else if (prefix_func_values[i] != 0)
    {
      answer.push_back(answer[prefix_func_values[i] - 1]);
    }
    else
    {
      std::fill(used.begin(), used.end(), false);
      for (int j = prefix_func_values[i - 1]; j > 0; j = prefix_func_values[j - 1])
      {
        used[getcode(answer[j])] = true;
      }
      char letter = 'b';
      for (; used[getcode(letter)]; letter++);
      answer += letter;
    }
  }
  return answer;
}
