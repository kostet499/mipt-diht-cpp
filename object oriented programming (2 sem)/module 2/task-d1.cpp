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
#include <iostream>
#include <list>
#include <queue>
#include <unordered_map>
#include <vector>



using namespace std;


class Condition
{

public:

  int empty_pos;
  int total_swipes;
  char direction;
  unsigned long long layout;

  Condition ():
      direction('\0'),
      empty_pos(-1),
      layout(0),
      total_swipes(0) {}

  Condition (const Condition &condition) :
      empty_pos(condition.empty_pos),
      direction(condition.direction),
      layout(condition.layout),
      total_swipes(condition.total_swipes) {}

  int GetNumber (int position) const;

  int ComputeManhathan () const;

  int ComputeInversions () const;

  int GetHeuristic () const;

  bool IsSolvable () const;

  bool IsSolved () const;

  void Swap (int first_pos, int second_pos);
};

class Comparator
{

public:

  bool operator() (const pair<int, Condition> &arg0, const pair<int, Condition> &arg1)
  {
    return arg0.first >= arg1.first;
  }
};

class Puzzle
{

  Condition condition;

public:

  void ShowSolution () const;

  friend istream &operator>> (istream &is, Puzzle &puzzle);

private:

  void aStar_ (list<char> &answer) const;

  void restoreSwipes_ (list<char> &answer, unsigned long long current_layout,
                       const unordered_map<unsigned long long, pair<char, unsigned long long> > &path) const;

  Condition swipe_ (const Condition &cond, int move) const;

  Condition swipeUp_ (const Condition &cond) const;

  Condition swipeDown_ (const Condition &cond) const;

  Condition swipeRight_ (const Condition &cond) const;

  Condition swipeLeft_ (const Condition &cond) const;
};



int main ()
{
  Puzzle puzzle;
  cin >> puzzle;
  puzzle.ShowSolution();
  return 0;
}



istream &operator>> (istream &is, Puzzle &puzzle)
{
  unsigned long long buffer;
  for (int i = 0; i < 16; i++)
  {
    cin >> buffer;
    if (buffer == 0)
    {
      puzzle.condition.empty_pos = i;
    }
    puzzle.condition.layout |= buffer << (i * 4);
  }
}

void Puzzle::ShowSolution () const
{
  if (condition.IsSolved())
  {
    cout << "0\n";
  }
  else if (!condition.IsSolvable())
  {
    cout << "-1\n";
  }
  else
  {
    list<char> solution;
    aStar_(solution);
    cout << solution.size() << endl;
    std::for_each(solution.rbegin(), solution.rend(), [](char move){ cout << move; });
  }
}

void Puzzle::aStar_ (list<char> &answer) const
{
  priority_queue<pair<int, Condition>, vector<pair<int, Condition> >, Comparator> conditions;
  conditions.push(make_pair(0, condition));
  unordered_map<unsigned long long, pair<char, unsigned long long> > path;
  path.emplace(condition.layout, make_pair('\0', 0));
  Condition current_condition, next_condition;
  while (!next_condition.IsSolved())
  {
    current_condition = conditions.top().second;
    conditions.pop();
    for (int move: {+1, -1, +4, -4})
    {
      next_condition = swipe_(current_condition, move);
      next_condition.total_swipes++;
      if (next_condition.IsSolved())
      {
        path.emplace(next_condition.layout,
                        make_pair(next_condition.direction, current_condition.layout)
        );
        break;
      }
      if (next_condition.layout != 0 && path.find(next_condition.layout) == path.end())
      {
        path.emplace(next_condition.layout,
                        make_pair(next_condition.direction, current_condition.layout)
        );
        conditions.emplace(next_condition.total_swipes + next_condition.GetHeuristic(), next_condition);
      }
    }
  }
  restoreSwipes_(answer, next_condition.layout, path);
}

void Puzzle::restoreSwipes_ (list<char> &answer, unsigned long long current_layout,
                             const unordered_map<unsigned long long, pair<char, unsigned long long> > &path) const
{
  char direction = '#';
  while (direction != '\0')
  {
    std::tie(direction, current_layout) = path.at(current_layout);
    answer.push_back(direction);
  }
  answer.pop_back();
}

Condition Puzzle::swipe_ (const Condition &cond, int move) const
{
  switch (move)
  {
    case +1:
      return swipeRight_(cond);
    case -1:
      return swipeLeft_(cond);
    case +4:
      return swipeDown_(cond);
    case -4:
      return swipeUp_(cond);
    default:
      return Condition();
  }
}

Condition Puzzle::swipeUp_ (const Condition &cond) const
{
  if (cond.empty_pos / 4 == 0 || cond.direction == 'U')
  {
    return Condition();
  }
  else
  {
    Condition new_cond(cond);
    new_cond.direction = 'D';
    new_cond.Swap(new_cond.empty_pos, new_cond.empty_pos - 4);
    new_cond.empty_pos = new_cond.empty_pos - 4;
    return new_cond;
  }
}

Condition Puzzle::swipeDown_ (const Condition &cond) const
{
  if (cond.empty_pos / 4 == 3 || cond.direction == 'D')
  {
    return Condition();
  }
  else
  {
    Condition new_cond(cond);
    new_cond.direction = 'U';
    new_cond.Swap(new_cond.empty_pos, new_cond.empty_pos + 4);
    new_cond.empty_pos = new_cond.empty_pos + 4;
    return new_cond;
  }
}

Condition Puzzle::swipeRight_ (const Condition &cond) const
{
  if (cond.empty_pos % 4 == 3 || cond.direction == 'R')
  {
    return Condition();
  }
  else
  {
    Condition new_cond(cond);
    new_cond.direction = 'L';
    new_cond.Swap(new_cond.empty_pos, new_cond.empty_pos + 1);
    new_cond.empty_pos = new_cond.empty_pos + 1;
    return new_cond;
  }
}

Condition Puzzle::swipeLeft_ (const Condition &cond) const
{
  if (cond.empty_pos % 4 == 0 || cond.direction == 'L')
  {
    return Condition();
  }
  else
  {
    Condition new_cond(cond);
    new_cond.direction = 'R';
    new_cond.Swap(new_cond.empty_pos, new_cond.empty_pos - 1);
    new_cond.empty_pos = new_cond.empty_pos - 1;
    return new_cond;
  }
}



int Condition::GetNumber (int position) const
{
  return static_cast<int>((layout >> ((1LL * position) << 2)) & 0xF);
}

int Condition::ComputeManhathan () const
{
  int manhathan = 0;
  for (int i = 0; i < 16; i++)
  {
    if (GetNumber(i) == 0)
      continue;
    manhathan += (abs(i % 4 - (GetNumber(i) - 1) % 4) +
                  abs(i / 4 - (GetNumber(i) - 1) / 4));
  }
  return manhathan;
}

int Condition::ComputeInversions () const
{
  int inversions = 0;
  for (int i = 0; i < 16; i++)
  {
    for (int j = i + 1; j < 16; j++)
    {
      if (GetNumber(i) != 0 && GetNumber(j) != 0 &&
          GetNumber(j) < GetNumber(i))
      {
        inversions++;
      }
    }
  }
  return inversions;
}

int Condition::GetHeuristic () const
{
  int manhathan = ComputeManhathan();
  int inversions = ComputeInversions();
  return (manhathan * 5 + inversions / 4) / 4;
}

bool Condition::IsSolvable () const
{
  int inversions = ComputeInversions();
  return !(inversions & 1) == ((empty_pos >> 2) & 1);
}

bool Condition::IsSolved () const
{
  return layout == 1147797409030816545LL;
}

void Condition::Swap (int first_pos, int second_pos)
{
  int first = GetNumber(first_pos);
  int second = GetNumber(second_pos);
  unsigned long long delta_first = layout & ((1LL << (first_pos << 2)) - 1);
  layout >>= 4 * first_pos + 4;
  layout <<= 4;
  layout |= second;
  layout <<= 4 * first_pos;
  layout |= delta_first;
  unsigned long long delta_second = layout & ((1LL << (second_pos << 2)) - 1);
  layout >>= 4 * second_pos + 4;
  layout <<= 4;
  layout |= first;
  layout <<= 4 * second_pos;
  layout |= delta_second;
}
