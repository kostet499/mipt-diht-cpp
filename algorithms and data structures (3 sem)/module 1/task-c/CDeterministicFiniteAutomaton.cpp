#include "CDeterministicFiniteAutomaton.h"


using ahocorasick::CState;
using ahocorasick::ComputeTransition;
using ahocorasick::ENTRY_POINT;
using ahocorasick::NULL_CSTATE_LINK;
using ahocorasick::NULL_NEXT_CSTATE;


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

void CDeterministicFiniteAutomaton::FindAllEntries
  (const std::string &text, std::vector<int> &entries, int total_pattern_size)
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
  precalculation.clear();
  patterns_sizes.clear();
  patterns_indexes.clear();
  trie.Reset();
  trie.GetNode(ENTRY_POINT).suffix_link = ENTRY_POINT;
}

void CDeterministicFiniteAutomaton::addStates (const std::string &word, int word_num)
{
  int prev_state_index = trie.AddNodes(word);
  trie.GetNode(prev_state_index).words_end_nums.emplace_back(word_num);
}

int CDeterministicFiniteAutomaton::computeSuffixLink (int state_ind)
{
  CState& state = trie.GetNode(state_ind);
  if (state.suffix_link == NULL_CSTATE_LINK)
  {
    if (state_ind != ENTRY_POINT && state.prev_state != ENTRY_POINT)
    {
      state.suffix_link = computeAutomateMove(
          computeSuffixLink(state.prev_state),
          state.transition
      );
    }
    else
    {
      state.suffix_link = ENTRY_POINT;
    }
  }
  return state.suffix_link;
}

int CDeterministicFiniteAutomaton::computeSuffixTerminatedLink (int state_ind)
{
  CState& state = trie.GetNode(state_ind);
  if (state.suffix_terminated_link == NULL_CSTATE_LINK)
  {
    int link = computeSuffixLink(state_ind);
    if (link == ENTRY_POINT)
    {
      state.suffix_terminated_link =  ENTRY_POINT;
    }
    else if (trie.GetNode(link).HasWordsEnds())
    {
      state.suffix_terminated_link = link;
    }
    else
    {
      state.suffix_terminated_link = computeSuffixTerminatedLink(link);
    }
  }
  return state.suffix_terminated_link;
}

int CDeterministicFiniteAutomaton::computeAutomateMove (int state_ind, char transition_letter)
{
  int transition = ComputeTransition(transition_letter);
  CState& state = trie.GetNode(state_ind);
  if (state.automate_move[transition] == NULL_CSTATE_LINK)
  {
    if (state.next_state[transition] != NULL_NEXT_CSTATE)
    {
      state.automate_move[transition] = state.next_state[transition];
    }
    else
    {
      if (state_ind == ENTRY_POINT)
      {
        state.automate_move[transition] = ENTRY_POINT;
      }
      else
      {
        state.automate_move[transition] = computeAutomateMove(
            computeSuffixLink(state_ind), transition_letter
        );
      }
    }
  }
  return state.automate_move[transition];
}


std::vector<int>* CDeterministicFiniteAutomaton::computeAccessibleStates (int state_ind)
{
  CState& state = trie.GetNode(state_ind);
  if (!state.is_terminal)
  {
    state.is_terminal = true;
    int good_link = computeSuffixTerminatedLink(state_ind);
    zipped_links_traversal[state_ind] = *computeAccessibleStates(good_link);
    for (auto visited_state_ind: state.words_end_nums)
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