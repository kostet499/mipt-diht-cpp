/*
 *
 *  РЕАЛИЗАЦИЯ : CListGraph (CVectorGraph)
 *
 */


/*
 *
 * УСЛОВИЕ
 * Дан невзвешенный неориентированный граф. В графе может быть несколько кратчайших
 * путей между какими-то вершинами. Найдите количество различных кратчайших путей
 * между заданными вершинами. Требуемая сложность O(V+E).
 *
 * ФОРМАТ ВВОДА
 * v: кол-во вершин (макс. 50000),
 * n: кол-во ребер (макс. 200000),
 * n пар реберных вершин,
 * пара вершин v, w для запроса.
 *
 * ФОРМАТ ВЫВОДА
 * Количество кратчайших путей от v к w.
 *
 */

#include <assert.h>
#include <iostream>
#include <queue>
#include <vector>
#include <list>



///////////////////// CLASS DEFINITION BEGIN /////////////////////

struct IGraph
{
  virtual ~IGraph () {}

  virtual void AddEdge (int from, int to) = 0;

  virtual int VerticesCount () const = 0;

  virtual void GetNextVertices (int vertex, std::vector<int> &vertices) const = 0;

  virtual void GetPrevVertices (int vertex, std::vector<int> &vertices) const = 0;
};



class CListGraph : public IGraph
{

private:

  int total_vertices_;
  std::vector<std::vector<int> > graph_container_in_;
  std::vector<std::vector<int> > graph_container_out_;

public:

  CListGraph (int n) : total_vertices_(n),
                       graph_container_in_(n), graph_container_out_(n) {}

  CListGraph (const IGraph *other);

  ~CListGraph () override;

  void AddEdge (int from, int to) override;

  int VerticesCount () const override;

  void GetNextVertices (int vertex, std::vector<int> &vertices) const override;

  void GetPrevVertices (int vertex, std::vector<int> &vertices) const override;

};

////////////////////// CLASS DEFINITION END //////////////////////


/*
 *
 * Функция для считывания графа из консоли.
 *
 *      IGraph *graph   - указатель на базовый класс графа
 *      int total_edges - количество рёбер графа
 *
 */
void ParseGraph (IGraph *const graph, int total_edges);

/*
 *
 * Непосредственно сама функция для нахождения количества различных путей.
 *
 *     const IGraph *const graph - константный указатель на констаный объект,
 *         наследуемый от IGraph
 *     int vertex_from - вершина-начало пути
 *     int vertex_to   - вершина-конец пути
 *
 */
int CountShortestPaths (const IGraph *const graph, int vertex_from, int vertex_to);


int main (int argc, const char *argv[])
{
  int v = 0, n = 0;
  std::cin >> v >> n;
  IGraph *graph = new CListGraph(v);
  ParseGraph(graph, n);
  int vertex_from = -1, vertex_to = -1;
  std::cin >> vertex_from >> vertex_to;
  std::cout << CountShortestPaths(graph, vertex_from, vertex_to) << std::endl;
  return 0;
}


void ParseGraph (IGraph *const graph, int total_edges)
{
  int from = -1, to = -1;
  for (int i = 0; i < total_edges; i++)
  {
    std::cin >> from >> to;
    graph->AddEdge(from, to);
    graph->AddEdge(to, from); // граф неориентированный по условию задания
  }
}

int CountShortestPaths (const IGraph *const graph, int vertex_from, int vertex_to)
{
  // храним данные для каждой вершины
  // (сколькими способами мы до неё добрались
  // из vertex_from за min_length шагов)
  std::vector<std::pair<int, int> > paths; // first -> count, second -> min_length
  // по-умолчанию это 0 способов за INT32_MAX шагов
  // (вершины не посещенна либо недосегаемая)
  paths.assign(graph->VerticesCount(), std::pair<int, int>(0, INT32_MAX));
  paths[vertex_from].first = 1;   // для стартовой вершины обозначим
  paths[vertex_from].second = 0;  // 1 способ за 0 шагов

  std::queue<int> vertices_queue;
  vertices_queue.push(vertex_from);

  // буффер для следующих вершин
  std::vector<int> next_vertices;

  while (!vertices_queue.empty())
  {
    int vertex = vertices_queue.front();
    vertices_queue.pop();
    graph->GetNextVertices(vertex, next_vertices);
    for (int next_vertex : next_vertices)
    {
      // если вершина впервые посещенна - установить данные в paths
      if (paths[next_vertex].second == INT32_MAX)
      {
        paths[next_vertex].first = paths[vertex].first;
        paths[next_vertex].second = paths[vertex].second + 1;
        // если данная вершина не финишная, а длина пути к ней не меньше,
        // чем к самой финишной, то далее пусть бфс из этой вершины не
        // целесообразно - найден полный путь, который гарантировано короче
        if (paths[next_vertex].second < paths[vertex_to].second)
          vertices_queue.push(next_vertex);
      }
      else
      {
        // мы дошли до next_vertex иным путём за одинаковое
        // количество шагов - увеличим paths[next_vertex].first
        if (paths[next_vertex].second == paths[vertex].second + 1)
          paths[next_vertex].first += paths[vertex].first;
        // если нашли путь короче к next_vertex

      }
    }
  }
  return paths[vertex_to].first;
}


///////////////////// CLASS DEFINITION BEGIN /////////////////////

CListGraph::CListGraph (const IGraph *other)
{
  total_vertices_ = other->VerticesCount();
  graph_container_in_.resize(total_vertices_);
  graph_container_out_.resize(total_vertices_);

  std::vector<int> vertices;
  for (int from = 0; from < total_vertices_; from++)
  {
    vertices.clear();
    other->GetNextVertices(from, vertices);
    for (int to: vertices)
      AddEdge(from, to);
  }
}

CListGraph::~CListGraph ()
{
  graph_container_in_.clear();
  graph_container_out_.clear();
}

void CListGraph::AddEdge (int from, int to)
{
  graph_container_in_[from].push_back(to);
  graph_container_out_[to].push_back(from);
}

int CListGraph::VerticesCount () const
{
  return total_vertices_;
}

void CListGraph::GetNextVertices (int vertex, std::vector<int> &vertices) const
{
  vertices.clear();
  for (int next_vertex: graph_container_in_[vertex])
    vertices.push_back(next_vertex);
}

void CListGraph::GetPrevVertices (int vertex, std::vector<int> &vertices) const
{
  vertices.clear();
  for (int next_vertex: graph_container_out_[vertex])
    vertices.push_back(next_vertex);
}

////////////////////// CLASS DEFINITION END //////////////////////
