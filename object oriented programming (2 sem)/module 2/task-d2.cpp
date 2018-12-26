#include <algorithm>
#include <cassert>
#include <iostream>


using namespace std;


const short MAX_RECURSION_DEPTH = 60;


inline unsigned char ComputeRow (unsigned char position)
{
  return position >> 2;
}

inline unsigned char ComputeColumn (unsigned char position)
{
  return position & 3;
}


class Condition
{

public:

  unsigned long long layout;

  unsigned char empty_position;

  short heuristic;


  Condition () :
      layout(0LL), heuristic(0) {}

  Condition (const Condition &condition) :
      layout(condition.layout),
      heuristic(condition.heuristic),
      empty_position(condition.empty_position) {}

  inline Condition Swipe (short move) const
  {
    unsigned char next_position = this->empty_position + move;
    unsigned char plate = GetPlate(next_position);
    Condition result(*this);
    result.SetPlate(next_position, 0);
    result.SetPlate(this->empty_position, plate);
    result.empty_position = next_position;
    result.heuristic = this->heuristic - this->ComputePlateHeuristic(next_position) +
                       result.ComputePlateHeuristic(this->empty_position);
    return result;
  }

  inline short ComputeHeuristic () const
  {
    short heuristic = 0;
    for (short i = 0; i < 16; i++)
    {
      if (GetPlate(i) == 0)
      {
        continue;
      }
      heuristic += abs(ComputeColumn(i) - (ComputeColumn(GetPlate(i) - 1)))
                   + abs(ComputeRow(i) - ComputeRow(GetPlate(i) - 1));
    }
    for (short i = 0; i < 16; i++)
    {
      if (i == empty_position)
      {
        continue;
      }
      for (short j = i + 1; j < 16; j++)
      {
        if (j == empty_position)
        {
          continue;
        }
        if (ComputeRow(i) == ComputeRow(GetPlate(i) - 1) &&
            ComputeRow(i) == ComputeRow(j) &&
            ComputeRow(j) == ComputeRow(GetPlate(j) - 1) &&
            GetPlate(i) > GetPlate(j))
        {
          heuristic += 2;
        }
        if (ComputeColumn(i) == ComputeColumn(GetPlate(i) - 1) &&
            ComputeColumn(i) == ComputeColumn(j) &&
            ComputeColumn(j) == ComputeColumn(GetPlate(j) - 1) &&
            GetPlate(i) > GetPlate(j))
        {
          heuristic += 2;
        }
      }
    }

    return heuristic;
  }

  inline short ComputePlateHeuristic (unsigned char place) const
  {
    short heuristic = abs(ComputeColumn(place) - ComputeColumn(GetPlate(place) - 1)) +
                      abs(ComputeRow(place) - ComputeRow(GetPlate(place) - 1));

    short plate_place = GetPlate(place);
    for (short i = 0; i < 4; i++)
    {
      short horizontal_position = place - ComputeColumn(place) + i;
      short vertical_position = (i << 2) + ComputeColumn(place);

      if (ComputeColumn(GetPlate(place) - 1) == ComputeColumn(place) &&
          vertical_position != empty_position &&
          ComputeColumn(vertical_position) == ComputeColumn(GetPlate(vertical_position) - 1))
      {
        if (vertical_position < place && GetPlate(vertical_position) > plate_place)
        {
          heuristic += 2;
        }
        else if (vertical_position > place && GetPlate(vertical_position) < plate_place)
        {
          heuristic += 2;
        }
      }
      if (ComputeRow(GetPlate(place) - 1) == ComputeRow(place) &&
          horizontal_position != empty_position &&
          ComputeRow(horizontal_position) == ComputeRow(GetPlate(horizontal_position) - 1))
      {
        if (horizontal_position < place && GetPlate(horizontal_position) > plate_place)
        {
          heuristic += 2;
        }
        else if (horizontal_position > place && GetPlate(horizontal_position) < plate_place)
        {
          heuristic += 2;
        }
      }
    }
    return heuristic;
  }

  inline bool IsSolvable () const
  {
    short inversions = 0;
    for (short i = 0; i < 16; i++)
      for (short j = i + 1; j < 16; j++)
        if (GetPlate(i) != 0 && GetPlate(j) != 0 && GetPlate(j) < GetPlate(i))
          inversions++;
    return ((inversions & 1) == 0) == ((empty_position >> 2) & 1 == 1);
  }

  inline bool IsSolved () const
  {
    return layout == 1147797409030816545LL;
  }

  inline void SetPlate (short place, short value)
  {
    layout = (layout & (~((1LL * 0xF) << (place << 2)))) |
             (static_cast<unsigned long long>(value) << (place << 2));
  }

  inline short GetPlate (short place) const
  {
    return static_cast<short>((layout >> ((1LL * place) << 2)) & 0xF);
  }
};


class Puzzle
{

  Condition start_condition;

  short limit_to_updated;

  string solution;

public:

  Puzzle(): limit_to_updated(INT16_MAX), solution("") {}

  string ComputeSolution ()
  {
    if (!start_condition.IsSolvable())
    {
      return "-1";
    }
    else
    {
      idaStar_();
      return solution;
    }
  }

  friend istream &operator>> (istream &is, Puzzle &puzzle);

private:

  inline void idaStar_ ()
  {
    string path = "";
    short limit = start_condition.heuristic;
    bool finished = false;
    do
    {
      limit_to_updated = INT16_MAX;
      dfs_(start_condition, limit, 0, path);
      limit = limit_to_updated;
      finished = !solution.empty();
    } while (!finished);
  }

  bool checkCondition_ (const Condition &condition, short potential_limit, short swipes, string &path)
  {
    if (!condition.IsSolved())
    {
      short potential = swipes + condition.heuristic;
      if (potential <= potential_limit)
      {
        dfs_(condition, potential_limit, swipes + 1, path);
      }
      else
      {
        limit_to_updated = min(limit_to_updated, potential);
      }
      return false;
    }
    else
    {
      if (solution.empty() || path.size() < solution.size())
      {
        solution = path;
      }
      return true;
    }
  }

  void dfs_ (const Condition &condition, short potential_limit, short swipes, string &path)
  {
    if (swipes == MAX_RECURSION_DEPTH)
    {
      return;
    }
    else
    {
      char possible_previous_move = '\0', next_move = '\0';
      bool correct_position = false;
      for (short move: {-1, +1, -4, +4})
      {
        switch(move)
        {
          case -1:
            possible_previous_move = 'L';
            next_move = 'R';
            correct_position = ComputeColumn(condition.empty_position) != 0;
            break;
          case +1:
            possible_previous_move = 'R';
            next_move = 'L';
            correct_position = ComputeColumn(condition.empty_position) != 3;
            break;
          case -4:
            possible_previous_move = 'U';
            next_move = 'D';
            correct_position = ComputeRow(condition.empty_position) != 0;
            break;
          case +4:
            possible_previous_move = 'D';
            next_move = 'U';
            correct_position = ComputeRow(condition.empty_position) != 3;
            break;
        }
        if (correct_position && path.back() != possible_previous_move)
        {
          path.push_back(next_move);
          if (checkCondition_(condition.Swipe(move), potential_limit, swipes, path))
            return;
          path.pop_back();
        }
      }
    }
  }
};


istream &operator>> (istream &is, Puzzle &puzzle)
{
  short buffer = 0;
  for (short i = 0; i < 16; i++)
  {
    cin >> buffer;
    puzzle.start_condition.SetPlate(i, buffer);
    if (buffer == 0)
    {
      puzzle.start_condition.empty_position = i;
    }
  }
  puzzle.start_condition.heuristic = puzzle.start_condition.ComputeHeuristic();
}


int main ()
{
  Puzzle puzzle;
  cin >> puzzle;
  string answer = puzzle.ComputeSolution();
  if (answer == "-1")
  {
    cout << answer;
  }
  else
  {
    cout << answer.size() << "\n" << answer << "\n";
  }
  return 0;
}

