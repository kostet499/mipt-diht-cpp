/*
 *
 *
 * УСЛОВИЕ
 * Tребуется отыскать самый выгодный маршрут между городами. Из города может
 * выходить дорога, которая возвращается в этот же город. Требуемое время работы
 * O((N + M)log N), где N – количество городов, M – известных дорог между ними.
 * N ≤ 10000, M ≤ 250000. Длина каждой дороги ≤ 10000.
 *
 * ФОРМАТ ВВОДА
 * Первая строка содержит число N – количество городов.
 * Вторая строка содержит число M - количество дорог.
 * Каждая следующая строка содержит описание дороги (откуда, куда, время в пути).
 * Все указанные дороги двусторонние. Между любыми двумя городами может быть больше одной дороги.
 * Последняя строка содержит маршрут (откуда и куда нужно доехать).

 *
 * ФОРМАТ ВЫВОДА
 * Вывести длину самого выгодного маршрута.
 *
 *
 */



#include <cassert>
#include <iostream>
#include <list>
#include <queue>
#include <set>


using namespace std;


const int INFINITY = INT32_MAX;


class CListGraph
{

private:

  int total_vertices_;
  std::vector<std::vector<pair<int, int> > > graph_container_;

public:

  CListGraph (int n) : total_vertices_(n), graph_container_(n) {}

  void AddEdge (int from, int to, int dist);

  void ReadFromConsole (int total_edges);

  int VerticesCount () const;

  void GetNextVertices (int vertex, std::vector<pair<int, int> > &vertices) const;

};

int Dijkstra (const CListGraph &graph, int from, int to);


int main ()
{
  int vertices = -1, edges = -1;
  cin >> vertices >> edges;
  CListGraph graph(vertices);
  graph.ReadFromConsole(edges);
  int from = -1, to = -1;
  cin >> from >> to;
  cout << Dijkstra(graph, from, to) << endl;
  return 0;
}


void CListGraph::AddEdge (int from, int to, int dist)
{
  graph_container_[from].emplace_back(to, dist);
  graph_container_[to].emplace_back(from, dist);
}

void CListGraph::ReadFromConsole (int total_edges)
{
  int from = -1, to = -1, dist = 0;
  for (int i = 0; i < total_edges; i++)
  {
    cin >> from >> to >> dist;
    AddEdge(from, to, dist);
  }
}

int CListGraph::VerticesCount () const
{
  return total_vertices_;
}

void CListGraph::GetNextVertices (int vertex, std::vector<pair<int, int> > &vertices) const
{
  vertices.clear();
  vertices = graph_container_[vertex];
}

void Relaxation (int vertex, const vector<pair<int, int> > &vertices,
                 set<pair<int, int> > &relaxed_edges, vector<int> &distances)
{
  for (pair<int, int> current_edge : vertices)
  {
    int to = current_edge.first, dist = current_edge.second;
    if (distances[to] > distances[vertex] + dist)
    {
      relaxed_edges.erase(make_pair(distances[to], to));
      distances[to] = distances[vertex] + dist;
      relaxed_edges.insert(make_pair(distances[to], to));
    }
  }
}

int Dijkstra (const CListGraph &graph, int from, int to)
{
  set<pair<int, int> > relaxed_edges;
  vector<int> distances(graph.VerticesCount(), INFINITY);
  distances[from] = 0;
  relaxed_edges.insert(make_pair(0, from));
  vector<pair<int, int> > vertices;
  while (!relaxed_edges.empty())
  {
    int vertex = relaxed_edges.begin()->second;
    relaxed_edges.erase(relaxed_edges.begin());
    graph.GetNextVertices(vertex, vertices);
    Relaxation(vertex, vertices, relaxed_edges, distances);
  }
  return distances[to];
}

