#include "ahocorasick.h"


ahocorasick::CState::CState (int prev_state, char letter, bool is_terminal) :
    prev_state(prev_state),
    transition(letter),
    is_terminal(is_terminal),
    suffix_link(NULL_CSTATE_LINK),
    suffix_terminated_link(NULL_CSTATE_LINK)
{
  std::fill(automate_move, automate_move + ALPHABET_SIZE, NULL_CSTATE_LINK);
  std::fill(next_state, next_state + ALPHABET_SIZE, NULL_NEXT_CSTATE);
}

bool ahocorasick::CState::HasWordsEnds ()
{
  return !words_end_nums.empty();
}

int ahocorasick::ComputeTransition (char transition)
{
  auto transition_code = static_cast<int>(transition - 'a');
  if (transition_code >= ahocorasick::ALPHABET_SIZE)
  {
    throw std::length_error("Wrong size of alphabet");
  }
  return transition_code;
}