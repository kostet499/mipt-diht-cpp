#ifndef TASK_C_CDETERMINISTICFINITEAUTOMATON_H
#define TASK_C_CDETERMINISTICFINITEAUTOMATON_H

#include <unordered_map> // std::unordered_map

#include "ahocorasick.h"
#include "trie.h"


class CDeterministicFiniteAutomaton
{
public:

  void Build (const std::vector<std::string> &patterns, const std::vector<int> &patterns_indexes);
  void FindAllEntries (const std::string &text, std::vector<int> &entries, int total_pattern_size);

private:

  void reset ();
  void addStates (const std::string &word, int word_num);
  int computeSuffixLink (int state_ind);
  int computeSuffixTerminatedLink (int state_ind);
  int computeAutomateMove (int state_ind, char transition_letter);
  std::vector<int>* computeAccessibleStates (int state_ind);
  void checkAccessibleStates (int state_ind, int text_ind);

private:

  Trie<ahocorasick::CState> trie;
  std::vector<unsigned short> precalculation;
  std::vector<int> patterns_sizes;
  std::vector<int> patterns_indexes;
  std::unordered_map<int, std::vector<int>> zipped_links_traversal;
};
#endif //TASK_C_CDETERMINISTICFINITEAUTOMATON_H