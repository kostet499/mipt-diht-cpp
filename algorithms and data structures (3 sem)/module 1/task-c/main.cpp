/* ContestID: 12356949 (2018, not fixed) */
/* ContestID: 12596266 (2017, fixed)     */

#include <iostream>                        // std::cin, std::cout

#include "CDeterministicFiniteAutomaton.h" // CDeterministicFiniteAutomaton


void ReadPatterns (std::vector<std::string> &patterns,
                   std::vector<int> &patterns_indexes,
                   int &total_pattern_size);

int main ()
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int total_pattern_size;
  std::vector<std::string> patterns;
  std::vector<int> patterns_indexes;
  ReadPatterns(patterns, patterns_indexes, total_pattern_size);
  CDeterministicFiniteAutomaton automaton;
  automaton.Build(patterns, patterns_indexes);
  std::string text;
  std::cin >> text;
  std::vector<int> entries;
  automaton.FindAllEntries(text, entries, total_pattern_size);
  for (auto position: entries)
  {
    std::cout << position << " ";
  }
  return 0;
}

void ReadPatterns (std::vector<std::string> &patterns,
                   std::vector<int> &patterns_indexes,
                   int &total_pattern_size)
{
  std::string full_pattern;
  std::cin >> full_pattern;
  total_pattern_size = static_cast<int>(full_pattern.size());
  full_pattern += '?';
  std::string sub_pattern;
  for (int i = 0; i < full_pattern.size(); i++)
  {
    if (full_pattern[i] == '?')
    {
      if (!sub_pattern.empty())
      {
        patterns.push_back(sub_pattern);
        patterns_indexes.emplace_back(i - sub_pattern.size());
      }
      sub_pattern = "";
    }
    else
    {
      sub_pattern += full_pattern[i];
    }
  }
}