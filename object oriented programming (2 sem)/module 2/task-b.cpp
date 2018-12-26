/*
 *
 * УСЛОВИЕ
 * Необходимо написать торгового советника для поиска арбитража.
 * Арбитраж - это торговля по цепочке различных валют в надежде
 * заработать на небольших различиях в коэффициентах.
 * Например, есть следующие курсы валют:
 *      GBP/USD: 0.67
 *      RUB/GBP: 78.66
 *      USD/RUB: 0.02
 * Имея 1$ и совершив цикл USD->GBP->RUB->USD, получим 1.054$. Таким образом заработав 5.4
 *
 * ФОРМАТ ВВОДА
 * Первая строка содержит число N – количество возможных валют (определяет размер таблицы
 * котировок). Далее следует построчное представление таблицы. Диагональные элементы (i, i)
 * пропущены (подразумевается, что курс валюты к себе же 1.0). В элементе таблицы (i, j)
 * содержится обменный курс i->j. Если обмен в данном направлении не производится, то -1.
 *
 * ФОРМАТ ВЫВОДА
 * Выведите YES, если арбитраж есть, и NO, иначе.
 *
 */


#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <tuple>
#include <vector>


using namespace std;


class CArcGraph
{

  unsigned int vertices;
  vector<tuple<int, int, double> > container;

public:

  explicit CArcGraph (unsigned int n) : vertices(n) {}

  unsigned int VerticesCount () const;

  void AddEdge (int from, int to, double weight);

  vector<tuple<int, int, double> >::const_iterator begin () const;

  vector<tuple<int, int, double> >::const_iterator end () const;
};

istream &operator>> (istream &is, CArcGraph &graph);


bool CheckForArbitrage (const CArcGraph &graph);

void BellmanFord (const CArcGraph &graph, int start, vector<double> &distance);

bool FindNegativeCycle (const CArcGraph &graph, const vector<double> &distance);


int main ()
{
  unsigned int vertices = 0;
  cin >> vertices;
  CArcGraph graph(vertices);
  cin >> graph;
  cout << (CheckForArbitrage(graph) ? "YES" : "NO") << endl;
  return 0;
}


unsigned int CArcGraph::VerticesCount () const
{
  return vertices;
}

void CArcGraph::AddEdge (int from, int to, double weight)
{
  container.emplace_back(from, to, weight);
}

vector<tuple<int, int, double> >::const_iterator CArcGraph::begin () const
{
  return container.begin();
}

vector<tuple<int, int, double> >::const_iterator CArcGraph::end () const
{
  return container.end();
}


istream &operator>> (istream &is, CArcGraph &graph)
{
  double dist = 0.0;
  int vertices_count = graph.VerticesCount();
  for (int from = 0; from < vertices_count; from++)
  {
    for (int to = 0; to < vertices_count; to++)
    {
      if (from == to)
      {
        graph.AddEdge(from, to, 1.0);
        continue;
      }
      is >> dist;
      if (dist != -1.0)
      {
        graph.AddEdge(from, to, dist);
      }
    }
  }
}


void BellmanFord (const CArcGraph &graph, int start, vector<double> &distance)
{
  distance[start] = 1.0;
  int from = -1, to = -1;
  double weight = 0.0;
  for_each(graph.begin(), graph.end(), [&] (auto edge)
  {
    tie(from, to, weight) = edge;
    if (distance[from] != -1.0 && distance[to] < distance[from] * weight)
      distance[to] = distance[from] * weight;
  });
}

bool FindNegativeCycle (const CArcGraph &graph, const vector<double> &distance)
{
  bool found = false;
  int from = -1, to = -1;
  double weight = 0.0;
  for_each(graph.begin(), graph.end(), [&] (auto edge)
  {
    tie(from, to, weight) = edge;
    if (distance[from] != -1.0 && distance[to] < distance[from] * weight)
      found = true;
  });
  return found;
}

bool CheckForArbitrage (const CArcGraph &graph)
{
  for (int iteration = 0; iteration < graph.VerticesCount() - 1; iteration++)
  {
    vector<double> distance(graph.VerticesCount(), -1.0);
    BellmanFord(graph, iteration, distance);
    if (FindNegativeCycle(graph, distance))
      return true;
  }
  return false;
}
