/*



 ПЯТНАШКИ (А)

 Написать алгоритм для решения игры в “пятнашки”.
 Решением задачи является приведение к виду:
    [ 1 2 3 4 ]
    [ 5 6 7 8 ]
    [ 9 10 11 12]
    [ 13 14 15 0 ],
 где 0 задает пустую ячейку. Достаточно найти хотя бы
 какое-то решение. Число перемещений костяшек не обязано
 быть минимальным.



 */



#include <algorithm>
#include <inttypes.h>
#include <iostream>
#include <list>
#include <queue>
#include <unordered_map>
#include <vector>


enum SwipeDirection
{
  UP = -4
  , DOWN = 4
  , RIGHT = 1
  , LEFT = -1
};

enum SwipeEncoding
{
  DOWN_ENCODING = 'U'
  , UP_ENCODING = 'D'
  , LEFT_ENCODING = 'R'
  , RIGHT_ENCODING = 'L'
  , ERROR_ENCODING = '\0'
};


class Condition
{

public:

  int32_t empty_pos;

  int32_t total_swipes;

  char direction;

  uint64_t layout;

  Condition () :
          direction('\0'),
          empty_pos(-1),
          layout(0),
          total_swipes(0)
  {}

  Condition (const Condition & condition) :
          empty_pos(condition.empty_pos),
          direction(condition.direction),
          layout(condition.layout),
          total_swipes(condition.total_swipes)
  {}

  int32_t GetPlateNumber (int32_t position) const;

  int32_t ComputeInversions () const;

  int32_t ComputeHeuristic () const;

  bool IsSolvable () const;

  bool IsSolved () const;

  void Swap (int32_t first_pos, int32_t second_pos);

  static SwipeEncoding EncodeSwipe (const Condition & cond, SwipeDirection move);
};


class Comparator
{

public:

  bool operator() (const std::pair<int32_t, Condition> & arg0,
                   const std::pair<int32_t, Condition> & arg1)
  {
    return arg0.first >= arg1.first;
  }
};


class Puzzle
{

  Condition condition;

public:

  void ShowSolution () const;

  friend std::istream & operator>> (std::istream & is, Puzzle & puzzle);

private:

  void aStar_ (std::list<char> & answer) const;

  void restoreSwipes_ (std::list<char> & answer, uint64_t current_layout,
                       const std::unordered_map<uint64_t, std::pair<char, uint64_t> > & path) const;

  Condition swipe_ (const Condition & cond, SwipeDirection move) const;
};

/**
 * @brief Entry point
 *
 * Execution of the program
 * starts here.
 *
 * @param argc Number of arguments
 * @param argv List of arguments
 *
 * @return Program exit status
 */
int main ()
{
  Puzzle puzzle;
  std::cin >> puzzle;
  puzzle.ShowSolution();
  return 0;
}


std::istream & operator>> (std::istream & is, Puzzle & puzzle)
{
  uint64_t buffer;
  for (uint8_t i = 0; i < 16; i++)
  {
    is >> buffer;
    if (buffer == 0)
    {
      puzzle.condition.empty_pos = i;
    }
    puzzle.condition.layout |= buffer << (i * 4);
  }
  return is;
}

void Puzzle::ShowSolution () const
{
  if (condition.IsSolved())
  {
    std::cout << "0\n";
  }
  else if (!condition.IsSolvable())
  {
    std::cout << "-1\n";
  }
  else
  {
    std::list<char> solution;
    aStar_(solution);
    std::cout << solution.size() << std::endl;
    std::for_each(solution.rbegin(), solution.rend(), [] (char move) { std::cout << move; });
  }
}

void Puzzle::aStar_ (std::list<char> & answer) const
{
  std::priority_queue<std::pair<int32_t, Condition>, std::vector<std::pair<int, Condition> >, Comparator> conditions;
  conditions.push(std::make_pair(0, condition));
  std::unordered_map<uint64_t, std::pair<char, uint64_t> > path;
  path.emplace(condition.layout, std::make_pair('\0', 0));
  Condition current_condition, next_condition;
  while (!next_condition.IsSolved())
  {
    current_condition = conditions.top().second;
    conditions.pop();
    for (SwipeDirection move: {RIGHT, LEFT, UP, DOWN})
    {
      next_condition = swipe_(current_condition, move);
      next_condition.total_swipes++;
      if (next_condition.IsSolved())
      {
        path.emplace(next_condition.layout,
                     std::make_pair(next_condition.direction, current_condition.layout)
        );
        break;
      }
      if (next_condition.layout != 0 && path.find(next_condition.layout) == path.end())
      {
        path.emplace(next_condition.layout,
                     std::make_pair(next_condition.direction, current_condition.layout)
        );
        int32_t heuristic = next_condition.total_swipes + next_condition.ComputeHeuristic();
        conditions.emplace(heuristic, next_condition);
      }
    }
  }
  restoreSwipes_(answer, next_condition.layout, path);
}

void Puzzle::restoreSwipes_ (std::list<char> & answer, uint64_t current_layout,
                             const std::unordered_map<uint64_t, std::pair<char, uint64_t> > & path) const
{
  register char direction = '#';
  while (direction != '\0')
  {
    std::tie(direction, current_layout) = path.at(current_layout);
    answer.push_back(direction);
  }
  answer.pop_back();
}


SwipeEncoding Condition::EncodeSwipe (const Condition & cond, SwipeDirection move)
{
  switch (move)
  {
    case SwipeDirection::RIGHT:
      return cond.empty_pos % 4 == 3 || cond.direction == SwipeEncoding::LEFT_ENCODING
             ? ERROR_ENCODING
             : RIGHT_ENCODING;
    case SwipeDirection::LEFT:
      return cond.empty_pos % 4 == 0 || cond.direction == SwipeEncoding::RIGHT_ENCODING
             ? ERROR_ENCODING
             : LEFT_ENCODING;
    case SwipeDirection::DOWN:
      return cond.empty_pos / 4 == 3 || cond.direction == SwipeEncoding::UP_ENCODING
             ? ERROR_ENCODING
             : DOWN_ENCODING;
    case SwipeDirection::UP:
      return cond.empty_pos / 4 == 0 || cond.direction == SwipeEncoding::DOWN_ENCODING
             ? ERROR_ENCODING
             : UP_ENCODING;
    default:
      return SwipeEncoding::ERROR_ENCODING;
  }
}


Condition Puzzle::swipe_ (const Condition & cond, SwipeDirection move) const
{
  SwipeEncoding encoding = Condition::EncodeSwipe(cond, move);
  if (encoding == SwipeEncoding::ERROR_ENCODING)
  {
    return Condition();
  }
  Condition new_cond(cond);
  new_cond.direction = encoding;
  new_cond.Swap(new_cond.empty_pos, new_cond.empty_pos + move);
  new_cond.empty_pos = new_cond.empty_pos + move;
  return new_cond;
}

int32_t Condition::GetPlateNumber (int32_t position) const
{
  return static_cast<int32_t>((layout >> ((1LL * position) << 2)) & 0xF);
}


int32_t Condition::ComputeInversions () const
{
  register int32_t inversions = 0;
  for (uint8_t i = 0; i < 16; i++)
  {
    for (auto j = static_cast<uint8_t>(i + 1); j < 16; j++)
    {
      if (GetPlateNumber(i) != 0 && GetPlateNumber(j) != 0 &&
          GetPlateNumber(j) < GetPlateNumber(i))
      {
        inversions++;
      }
    }
  }
  return inversions;
}

inline int32_t ComputeRow (int32_t position)
{
  return position >> 2;
}

inline int32_t ComputeColumn (int32_t position)
{
  return position & 3;
}

int32_t Condition::ComputeHeuristic () const
{
  register uint8_t heuristic = 0;
  for (uint8_t i = 0; i < 16; i++)
  {
    if (GetPlateNumber(i) == 0)
    {
      continue;
    }
    heuristic += 4 * (abs(ComputeColumn(i) - (ComputeColumn(GetPlateNumber(i) - 1)))
                      + abs(ComputeRow(i) - ComputeRow(GetPlateNumber(i) - 1)));
  }
  for (uint8_t i = 0; i < 16; i++)
  {
    if (i == empty_pos)
    {
      continue;
    }
    for (auto j = static_cast<uint8_t>(i + 1); j < 16; j++)
    {
      if (j == empty_pos)
      {
        continue;
      }
      if (ComputeRow(i) == ComputeRow(GetPlateNumber(i) - 1) &&
          ComputeRow(i) == ComputeRow(j) &&
          ComputeRow(j) == ComputeRow(GetPlateNumber(j) - 1) &&
          GetPlateNumber(i) > GetPlateNumber(j))
      {
        heuristic += 2;
      }
      if (ComputeColumn(i) == ComputeColumn(GetPlateNumber(i) - 1) &&
          ComputeColumn(i) == ComputeColumn(j) &&
          ComputeColumn(j) == ComputeColumn(GetPlateNumber(j) - 1) &&
          GetPlateNumber(i) > GetPlateNumber(j))
      {
        heuristic += 2;
      }
    }
  }

  return heuristic / 2;
}

bool Condition::IsSolvable () const
{
  int32_t inversions = ComputeInversions();
  return !(inversions & 1) == ((empty_pos >> 2) & 1);
}

bool Condition::IsSolved () const
{
  return layout == 1147797409030816545ULL;
}

void Condition::Swap (int32_t first_pos, int32_t second_pos)
{
  int32_t first = GetPlateNumber(first_pos);
  int32_t second = GetPlateNumber(second_pos);
  uint64_t delta_first = layout & ((1LL << (first_pos << 2)) - 1);
  layout >>= 4 * first_pos + 4;
  layout <<= 4;
  layout |= second;
  layout <<= 4 * first_pos;
  layout |= delta_first;
  uint64_t delta_second = layout & ((1LL << (second_pos << 2)) - 1);
  layout >>= 4 * second_pos + 4;
  layout <<= 4;
  layout |= first;
  layout <<= 4 * second_pos;
  layout |= delta_second;
}
