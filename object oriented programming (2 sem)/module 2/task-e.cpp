/*
 *
 * УСЛОВИЕ
 * Полный ориентированный взвешенный граф задан матрицей смежности.
 * Постройте матрицу кратчайших путей между его вершинами.
 * Гарантируется, что в графе нет циклов отрицательного веса.
 *
 * ФОРМАТ ВВОДА
 * В первой строке вводится единственное число N (1 < N < 100) —
 * количество вершин графа. В следующих N строках по N чисел задается
 * матрица смежности графа (j-ое число в i-ой строке — вес ребра из вершины i в вершину j).
 * Все числа по модулю не превышают 100. На главной диагонали матрицы — всегда нули.

 *
 * ФОРМАТ ВЫВОДА
 * Выведите N строк по N чисел — матрицу расстояний между парами вершин, где j-ое число
 * в i-ой строке равно весу кратчайшего пути из вершины i в j.
 *
 */



#include <cassert>
#include <iostream>
#include <vector>


using namespace std;


class CMatrixGraph
{

private:

  int total_vertices_;
  std::vector<std::vector<int> > container_;

public:

  CMatrixGraph (int n) : total_vertices_(n),
                         container_(n, std::vector<int>(n, 0)) {}

  int VerticesCount () const;

  void AddEdge (int from, int to, int dist);

  friend void FloydWarshall (CMatrixGraph &graph);

  friend istream &operator>> (const istream &is, CMatrixGraph &graph);

  friend ostream &operator<< (const ostream &os, const CMatrixGraph &graph);

};


int main ()
{
  int vertices = 0;
  cin >> vertices;
  CMatrixGraph graph(vertices);
  cin >> graph;
  FloydWarshall(graph);
  cout << graph;
  return 0;
}


int CMatrixGraph::VerticesCount () const
{
  return total_vertices_;
}

void CMatrixGraph::AddEdge (int from, int to, int dist)
{
  assert(from > -1 && from < container_.size());
  assert(to > -1 && to < container_.size());
  container_[from][to] = dist;
}

void FloydWarshall (CMatrixGraph &graph)
{
  for (int temp = 0; temp < graph.total_vertices_; temp++)
    for (int from = 0; from < graph.total_vertices_; from++)
      for (int to = 0; to < graph.total_vertices_; to++)
        if (from == to)
          continue;
        else
          graph.container_[from][to] = min(
              graph.container_[from][to],
              graph.container_[from][temp] + graph.container_[temp][to]
          );
}

istream &operator>> (const istream &is, CMatrixGraph &graph)
{
  int dist = 0;
  for (int from = 0; from < graph.total_vertices_; from++)
  {
    for (int to = 0; to < graph.total_vertices_; to++)
    {
      cin >> dist;
      graph.AddEdge(from, to, dist);
    }
  }
}

ostream &operator<< (const ostream &os, const CMatrixGraph &graph)
{
  for (int from = 0; from < graph.total_vertices_; from++)
  {
    for (int to = 0; to < graph.total_vertices_; to++)
    {
      cout << graph.container_[from][to] << " ";
    }
    cout << endl;
  }
}
