#ifndef TASK_C_TRIE_H
#define TASK_C_TRIE_H

#include <string> // std::string
#include <vector> // std::vector

#include "ahocorasick.h"


template<class INodeType>
class Trie
{
public:

  INodeType& GetNode (int index)
  {
    return trie[index];
  }

  int AddNodes (const std::string &word)
  {
    int prev_state_index = 0;
    for (auto letter: word)
    {
      auto transition = ahocorasick::ComputeTransition(letter);
      if (trie[prev_state_index].next_state[transition] == ahocorasick::NULL_NEXT_CSTATE)
      {
        trie[prev_state_index].next_state[transition] = static_cast<int>(trie.size());
        trie.emplace_back(INodeType(prev_state_index, letter, false));
      }
      prev_state_index = trie[prev_state_index].next_state[transition];
    }
    return prev_state_index;
  }

  void Reset ()
  {
    trie.clear();
    trie.emplace_back(INodeType(ahocorasick::ENTRY_POINT, '\0', false));
  }

private:

  std::vector<INodeType> trie;
};
#endif //TASK_C_TRIE_H