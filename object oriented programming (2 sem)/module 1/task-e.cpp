
/*
 *
 * УСЛОВИЕ
 * Единственный способ попасть в Зал Круглых Столов – пройти через Колонный Коридор.
 * Стены Коридора изображаются на карте прямыми линиями, которые параллельны оси OY
 * системы координат. Вход в Коридор находится снизу, а выход из Коридора в Зал – сверху.
 * В Коридоре есть цилиндрические (на карте круглые) Колонны одинакового радиуса R.
 * Разработайте алгоритм, который по информации о размерах Коридора, и размещения Колонн
 * определяет диаметр наибольшего из Круглых Столов, который можно пронести через такой
 * Коридор, сохраняя поверхность Стола горизонтальной.
 *
 * ФОРМАТ ВВОДА
 * В первой строке задано два целых числа XL и XR - x-координаты левой и правой стен Коридора.
 * Во второй строке находится целое число R (1≤R≤1000000) - радиус всех Колон.
 * В третьей - целое число N (1≤N≤200), задающее количество Колон.
 * Далее идут N строк, в каждой из которых по два целых числа – x- и y-координаты центра
 * соответствующей Колоны. Все входные координаты – целые числа, не превышающие по модулю 1000000.
 *
 * ФОРМАТ ВЫВОДА
 * Вывести одно число - искомый диаметр наибольшего Стола.
 * Диаметр нужно выводить с точностью 3 знака после десятичной точки (даже в случае, если он окажется целым).
 * Если нельзя пронести ни одного Стола, то ответ должен быть: 0.000
 * Точность 3 знака после точки, по обычнам правилам округления, означает, что ответ, который выводится в
 * выходной файл, должен отличатся от точного не более чем на 5×10−4 (то есть на 0.0005). Например, если
 * точный ответ 1.234567, то в файле должно находится число 1.235. Если точный ответ 5.0005, то необходимо
 * округлять в большую сторону, то есть в файл необходимо вывести 5.001.
 *
 */



#include <cassert>
#include <cmath>
#include <iostream>
#include <queue>



using namespace std;


// из входных данных построим полный граф из коллон и стен
void BuildGraph (int columns, vector<pair<double, double> > &coords,
                 double left_wall, double right_wall, double column_radius, vector<vector<double> > &graph);

/*
 *
 * "перевернём" граф и в качестве стартовой вершины рассмотрим левую стенку, фиинишем - правую
 * в качестве существования ребра между вершинами U и V будем рассматривать неравенство вида
 * Dist(U, V) <= r, где Dist - расстояние между вершинами графа, r - предполагаемый максимально
 * допустимый радиус стола;
 * ответом на задачу будет inf R, где R = {r | для r можно достичь правой стенки начиная с левой}
 *
 */
double ComputeRadius (int columns, double left_wall, double right_wall, const vector<vector<double> > &graph);



int main ()
{
  int columns = 0;
  double left_wall = 0, right_wall = 0, column_radius = 0;
  cin >> left_wall >> right_wall >> column_radius >> columns;

  vector<pair<double, double> > coords;
  coords.resize(columns + 1);
  for (int i = 1; i <= columns; i++)
    cin >> coords[i].first >> coords[i].second;

  // размер columns + 2 так как стоит учесть за вершины правую и левую стены
  vector<vector<double> > graph(columns + 2, vector<double>(columns + 2, 0));

  // создадим полный граф (graph) используя coords
  BuildGraph(columns, coords, left_wall, right_wall, column_radius, graph);

  cout.precision(3);
  cout << fixed;
  cout << ComputeRadius(columns, left_wall, right_wall, graph);

  return 0;
}



double ComputeDistance (pair<double, double> &p1, pair<double, double> &p2)
{
  return sqrt((p1.first - p2.first) * (p1.first - p2.first) +
                  (p1.second - p2.second) * (p1.second - p2.second));
}

void BuildGraph (int columns, vector<pair<double, double> > &coords,
                 double left_wall, double right_wall, double column_radius, vector<vector<double> > &graph)
{
  // расчитаем расстоярие между каждыми колонами
  for (int i = 1; i <= columns; i++)
    for (int j = i + 1; j <= columns; j++)
      graph[i][j] = graph[j][i] = ComputeDistance(coords[i], coords[j]) - column_radius * 2;

  // а также между колонами и стенами
  for (int i = 1; i <= columns; i++)
  {
    graph[0][i] = graph[i][0] = fabs(coords[i].first - left_wall) - column_radius;
    graph[columns + 1][i] = graph[i][columns + 1] = fabs(coords[i].first - right_wall) - column_radius;
  }

  // и наконец между самими стенами
  graph[0][columns + 1] = graph[columns + 1][0] = right_wall - left_wall;
}

void Bfs (const vector<vector<double> > &graph, vector<bool> &visited, double supposed_radius)
{
  queue<int> vertices;
  // стартовая вершина - левая стена
  vertices.push(0);
  while (!vertices.empty())
  {
    int vertex = vertices.front();
    vertices.pop();
    // рассмотрим вершину vertex и все достижимые из неё: supposed_radius не меньше расстояния
    for (int i = 0; i < static_cast<int>(graph.size()); i++)
      if (!visited[i] && graph[vertex][i] <= supposed_radius)
        // добавим в очередь для поиска в ширину из всех таких ещё непосещённых
        vertices.push(i);
    visited[vertex] = true;
  }
}

double ComputeRadius (int columns, double left_wall, double right_wall, const vector<vector<double> > &graph)
{
  double min_radius = 0, max_radius = right_wall - left_wall;
  vector<bool> visited(columns + 2, false);

  // выполним итеративный бинарный поиск  для поиска радиуса
  // с ограничением на количество аппроксимаций  (40)
  for (int approx_iter = 0; approx_iter < 40; approx_iter++)
  {
    double supposed_radius = (min_radius + max_radius) / 2;
    visited.assign(columns + 2, false);
    Bfs(graph, visited, supposed_radius);

    // если мы смогли достичь правой стенки, то радиус стола больше нужного
    if (visited[columns + 1])
      max_radius = supposed_radius;
    // иначе - текущий радиус стола не больше искомого
    else
      min_radius = supposed_radius;
  }

  return min_radius;
}
