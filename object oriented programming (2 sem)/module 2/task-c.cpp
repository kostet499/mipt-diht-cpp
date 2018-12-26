/*
    ВОСЬМИНАШКИ

 «Восьминашки» – упрощенный вариант известной головоломки «Пятнашки».
 Восемь костяшек, пронумерованных от 1 до 8, расставлены по ячейкам
 игровой доски 3 на 3, одна ячейка при этом остается пустой. За один
 ход разрешается передвинуть одну из костяшек, расположенных рядом с
 пустой ячейкой, на свободное место. Цель игры – для заданной начальной
 конфигурации игровой доски за минимальное число ходов получить
 выигрышную конфигурацию (пустая ячейка обозначена нулем):
    1 2 3
    4 5 6
    7 8 0

    ФОРМАТ ВХОДНОГО ФАЙЛА

 Во входном файле содержится начальная конфигурация головоломки – 3 строки
 по 3 числа в каждой.

    ФОРМАТ ВЫХОДНОГО ФАЙЛА

 Если решение существует, то в первой строке выходного файла выведите
 минимальное число перемещений костяшек, которое нужно сделать, чтобы
 достичь выигрышной конфигурации, а во второй строке выведите
 соответствующую последовательность ходов: L означает, что в результате
 перемещения костяшки пустая ячейка сдвинулась влево, R – вправо, U – вверх,
 D – вниз. Если таких последовательностей несколько, то выведите любую из
 них. Если же выигрышная конфигурация недостижима, то выведите в выходной
 файл одно число −1.

 */

#include <iostream>
#include <cmath>
#include <queue>
#include <set>


using namespace std;


class Condition
{

public:

  int empty_pos;
  long long layout;
  string path;

  Condition () : path(""), empty_pos(-1), layout(0) {}

  Condition (const Condition &condition) :
      empty_pos(condition.empty_pos),
      path(condition.path),
      layout(condition.layout) {}

  bool IsSolvable () const;

  void MakeSwipe (int digit0_pos, int digit1_pos);

  bool IsSolved () const;

private:

  int getPosition_ (int digit) const;

  int tenPower_ (int power) const;
};


class Puzzle
{

  Condition condition;

public:

  string GetSolution () const;

  friend istream &operator>> (istream &is, Puzzle &puzzle);

private:

  Condition swipe_ (const Condition &cond, int move) const;

  Condition swipeUp_ (const Condition &cond) const;

  Condition swipeDown_ (const Condition &cond) const;

  Condition swipeRight_ (const Condition &cond) const;

  Condition swipeLeft_ (const Condition &cond) const;

};


istream &operator>> (istream &is, Puzzle &puzzle)
{
  int buffer;
  puzzle.condition.layout = 0LL;
  for (int i = 8; i >= 0; i--)
  {
    cin >> buffer;
    puzzle.condition.layout += buffer * pow(10, i);
    if (buffer == 0)
    {
      puzzle.condition.empty_pos = i;
    }
  }
}


int main ()
{
  Puzzle puzzle;
  cin >> puzzle;
  string answer = puzzle.GetSolution();
  if (answer != "-1")
    cout << answer.size() << endl << answer << endl;
  else
    cout << answer << endl;
  return 0;
}


bool Condition::IsSolvable () const
{
  int inversions = 0;
  for (int i = 0; i < 9; i++)
    for (int j = i + 1; j < 9; j++)
      inversions += (
          getPosition_(8 - i) &&
          getPosition_(8 - j) &&
          getPosition_(8 - j) < getPosition_(8 - i)
      );
  return inversions % 2 == 0;
}

void Condition::MakeSwipe (int digit0_pos, int digit1_pos)
{
  int digit0 = getPosition_(digit0_pos);
  int digit1 = getPosition_(digit1_pos);
  layout -= digit0 * tenPower_(digit0_pos);
  layout -= digit1 * tenPower_(digit1_pos);
  layout += digit0 * tenPower_(digit1_pos);
  layout += digit1 * tenPower_(digit0_pos);
}

bool Condition::IsSolved () const
{
  return this->layout == 123456780;
}

int Condition::getPosition_ (int digit) const
{
  return static_cast<int>((layout / tenPower_(digit)) % 10);
}

int Condition::tenPower_ (int power) const
{
  return static_cast<int>(pow(10, power));
}


string Puzzle::GetSolution () const
{
  if (!condition.IsSolvable())
    return "-1";
  set<long long> visited;
  Condition current_condition;
  Condition next_condition;
  queue<Condition> conditions;
  conditions.push(condition);
  while (true)
  {
    current_condition = conditions.front();
    conditions.pop();
    if (current_condition.IsSolved())
    {
      return current_condition.path;
    }
    for (int move: {1, -1, 3, -3})
    {
      next_condition = swipe_(current_condition, move);
      if (next_condition.layout != 0 && visited.find(next_condition.layout) == visited.end())
      {
        conditions.push(next_condition);
        visited.insert(next_condition.layout);
      }
    }
  }
}

Condition Puzzle::swipe_ (const Condition &cond, int move) const
{
  switch (move)
  {
    case +1:
      return swipeRight_(cond);
    case -1:
      return swipeLeft_(cond);
    case +3:
      return swipeDown_(cond);
    case -3:
      return swipeUp_(cond);
    default:
      return Condition();
  }
}

Condition Puzzle::swipeUp_ (const Condition &cond) const
{
  if (cond.empty_pos / 3 == 0 || *cond.path.rbegin() == 'U')
  {
    return Condition();
  }
  else
  {
    Condition new_cond(cond);
    new_cond.path += 'D';
    new_cond.MakeSwipe(new_cond.empty_pos, new_cond.empty_pos - 3);
    new_cond.empty_pos = new_cond.empty_pos - 3;
    return new_cond;
  }
}

Condition Puzzle::swipeDown_ (const Condition &cond) const
{
  if (cond.empty_pos / 3 == 2 || *cond.path.rbegin() == 'D')
  {
    return Condition();
  }
  else
  {
    Condition new_cond(cond);
    new_cond.path += 'U';
    new_cond.MakeSwipe(new_cond.empty_pos, new_cond.empty_pos + 3);
    new_cond.empty_pos = new_cond.empty_pos + 3;
    return new_cond;
  }
}

Condition Puzzle::swipeRight_ (const Condition &cond) const
{
  if (cond.empty_pos % 3 == 2 || *cond.path.rbegin() == 'R')
  {
    return Condition();
  }
  else
  {
    Condition new_cond(cond);
    new_cond.path += 'L';
    new_cond.MakeSwipe(new_cond.empty_pos, new_cond.empty_pos + 1);
    new_cond.empty_pos = new_cond.empty_pos + 1;
    return new_cond;
  }
}

Condition Puzzle::swipeLeft_ (const Condition &cond) const
{
  if (cond.empty_pos % 3 == 0 || *cond.path.rbegin() == 'L')
  {
    return Condition();
  }
  else
  {
    Condition new_cond(cond);
    new_cond.path += 'R';
    new_cond.MakeSwipe(new_cond.empty_pos, new_cond.empty_pos - 1);
    new_cond.empty_pos = new_cond.empty_pos - 1;
    return new_cond;
  }
}
