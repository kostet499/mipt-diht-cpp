#include "CDeterministicFiniteAutomaton.h"


void CDeterministicFiniteAutomaton::Build
    (const std::vector<std::string> &patterns, const std::vector<int> &patterns_indexes)
{
  reset();
  this->patterns_indexes = patterns_indexes;
  patterns_sizes.resize(patterns.size());
  for (int pattern_num = 0; pattern_num < patterns.size(); pattern_num++)
  {
    patterns_sizes[pattern_num] = static_cast<int>(patterns[pattern_num].size());
    addStates(patterns[pattern_num], pattern_num);
  }
}

void CDeterministicFiniteAutomaton::FindAllEntries (const std::string &text,
                                                    std::vector<int> &entries, int total_pattern_size)
{
  entries.clear();
  if (static_cast<int>(text.size()) - total_pattern_size >= 0)
  {
    precalculation.resize(text.size(), 0);
    int state_ind = 0;
    for (int i = 0; i < text.length(); i++)
    {
      state_ind = computeAutomateMove(state_ind, text[i]);
      checkAccessibleStates(state_ind, i + 1);
    }
    for (int i = 0; i < text.size() - total_pattern_size + 1; i++)
    {
      if (precalculation[i] == patterns_sizes.size())
      {
        entries.emplace_back(i);
      }
    }
  }
}

void CDeterministicFiniteAutomaton::reset ()
{
  states.clear();
  precalculation.clear();
  patterns_sizes.clear();
  patterns_indexes.clear();
  states.emplace_back(CState(ahocorasick::ENTRY_POINT, '\0', false));
  states[ahocorasick::ENTRY_POINT].suffix_link = ahocorasick::ENTRY_POINT;
}

void CDeterministicFiniteAutomaton::addStates (const std::string &word, int word_num)
{
  int prev_state_index = 0;
  for (auto letter: word)
  {
    auto transition = computeTransition(letter);
    if (states[prev_state_index].next_state[transition] == ahocorasick::NULL_NEXT_CSTATE)
    {
      states[prev_state_index].next_state[transition] = static_cast<int>(states.size());
      states.emplace_back(CState(prev_state_index, letter, false));
    }
    prev_state_index = states[prev_state_index].next_state[transition];
  }
  states[prev_state_index].words_end_nums.emplace_back(word_num);
}

int CDeterministicFiniteAutomaton::computeSuffixLink (int state_ind)
{
  if (states[state_ind].suffix_link == ahocorasick::NULL_CSTATE_LINK)
  {
    if (state_ind != ahocorasick::ENTRY_POINT && states[state_ind].prev_state != ahocorasick::ENTRY_POINT)
    {
      states[state_ind].suffix_link = computeAutomateMove(
          computeSuffixLink(states[state_ind].prev_state),
          states[state_ind].transition
      );
    }
    else
    {
      states[state_ind].suffix_link = ahocorasick::ENTRY_POINT;
    }
  }
  return states[state_ind].suffix_link;
}

int CDeterministicFiniteAutomaton::computeSuffixTerminatedLink (int state_ind)
{
  if (states[state_ind].suffix_terminated_link == ahocorasick::NULL_CSTATE_LINK)
  {
    int link = computeSuffixLink(state_ind);
    if (link == ahocorasick::ENTRY_POINT)
    {
      states[state_ind].suffix_terminated_link =  ahocorasick::ENTRY_POINT;
    }
    else if (states[link].HasWordsEnds())
    {
      states[state_ind].suffix_terminated_link = link;
    }
    else
    {
      states[state_ind].suffix_terminated_link = computeSuffixTerminatedLink(link);
    }
  }
  return states[state_ind].suffix_terminated_link;
}

int CDeterministicFiniteAutomaton::computeAutomateMove (int state_ind, char transition_letter)
{
  auto transition = computeTransition(transition_letter);
  if (states[state_ind].automate_move[transition] == ahocorasick::NULL_CSTATE_LINK)
  {
    if (states[state_ind].next_state[transition] != ahocorasick::NULL_NEXT_CSTATE)
    {
      states[state_ind].automate_move[transition] =
          states[state_ind].next_state[transition];
    }
    else
    {
      if (state_ind == ahocorasick::ENTRY_POINT)
      {
        states[state_ind].automate_move[transition] = ahocorasick::ENTRY_POINT;
      }
      else
      {
        states[state_ind].automate_move[transition] =
            computeAutomateMove(computeSuffixLink(state_ind), transition_letter);
      }
    }
  }
  return states[state_ind].automate_move[transition];
}

int CDeterministicFiniteAutomaton::computeTransition (char transition)
{
  auto transition_code = static_cast<int>(transition - 'a');
  if (transition_code >= ahocorasick::ALPHABET_SIZE)
  {
    throw std::length_error("Wrong size of alphabet");
  }
  return transition_code;
}

std::vector<int>* CDeterministicFiniteAutomaton::computeAccessibleStates (int state_ind)
{
  if (!states[state_ind].is_terminal)
  {
    states[state_ind].is_terminal = true;
    int good_link = computeSuffixTerminatedLink(state_ind);
    zipped_links_traversal[state_ind] = *computeAccessibleStates(good_link);
    for (auto visited_state_ind: states[state_ind].words_end_nums)
    {
      zipped_links_traversal[state_ind].emplace_back(visited_state_ind);
    }
  }
  return &zipped_links_traversal[state_ind];
}

void CDeterministicFiniteAutomaton::checkAccessibleStates (int state_ind, int text_ind)
{
  for (auto accessible_state : *computeAccessibleStates(state_ind))
  {
    int suffix_length_delta = text_ind - patterns_sizes[accessible_state]
                              - patterns_indexes[accessible_state];
    if (suffix_length_delta >= 0)
    {
      precalculation[suffix_length_delta]++;
    }
  }
}
