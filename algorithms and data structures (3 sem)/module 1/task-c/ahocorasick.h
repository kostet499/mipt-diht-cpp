#ifndef TASK_C_AHOCORASICK_H
#define TASK_C_AHOCORASICK_H

#include <stdexcept>  // std::length_error
#include <string>     // std::string
#include <vector>     // std::vector


namespace ahocorasick
{
  static const int ALPHABET_SIZE = 26;
  static const int NULL_NEXT_CSTATE = -1;
  static const int NULL_CSTATE_LINK = -1;
  static const int ENTRY_POINT = 0;

  struct CState
  {
    explicit CState (int prev_state, char letter, bool is_terminal);
    bool HasWordsEnds ();
    bool is_terminal;
    char transition;
    int prev_state;
    int suffix_link;
    int suffix_terminated_link;
    int automate_move[ALPHABET_SIZE]{};
    int next_state[ALPHABET_SIZE]{};
    std::vector<int> words_end_nums;
  };

  int ComputeTransition (char transition);
};

#endif //TASK_C_AHOCORASICK_H