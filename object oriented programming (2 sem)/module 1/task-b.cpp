/*
 *
 *  РЕАЛИЗАЦИЯ : CListGraph
 *
 */


/*
 *
 * УСЛОВИЕ
 * Дан невзвешенный неориентированный граф. Найдите цикл минимальной длины.
 * Требуемая сложность O(VE), + 1 балл за O(V + E).
 *
 * ФОРМАТ ВВОДА
 * В первой строке вводятся два натуральных числа N и M, не превосходящих 10000.
 * Далее идут M строк по 2 числа (от 0 до N-1) - индексы вершин между которыми есть ребро.
 *
 * ФОРМАТ ВЫВОДА
 * Требуется вывести одно целое число равное длине минимального цикла.
 * Если цикла нет, то вывести -1.
 *
 */

#include <assert.h>
#include <iostream>
#include <queue>
#include <vector>



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
void ParseGraph (IGraph *graph, int total_edges);


/*
 *
 * Функция обхода в ширину, используемая во время подсчитывания
 * циклов в нашем графе.
 *
 *     int vertex - номер вершины из которой запускаем обход в ширину
 *     const IGraph *const graph - константный указатель на констаный объект,
 *         наследуемый от IGraph
 *
 */
int Bfs (int vertex, const IGraph *const graph);

/*
 *
 * Непосредственно сама функция для нахождения самого цикла.
 *
 *     const IGraph *const graph - константный указатель на констаный объект,
 *         наследуемый от IGraph
 *
 */
int ComputeMinCycle (const IGraph *const graph);


int main (int argc, const char *argv[])
{

  int total_vertices = 0, total_edges = 0;
  std::cin >> total_vertices >> total_edges;

  IGraph *graph = new CListGraph(total_vertices);
  ParseGraph(graph, total_edges);

  std::cout << ComputeMinCycle(graph);

  delete graph;
  return 0;
}


void ParseGraph (IGraph *const graph, int total_edges)
{
  int from = -1, to = -1;
  for (int i = 0; i < total_edges; i++)
  {
    std::cin >> from >> to;
    graph->AddEdge(from, to);
    graph->AddEdge(to, from); // граф неореиентированный по условию задания
  }
}

int Bfs (int vertex, const IGraph *const graph)
{
  int total_vertices = graph->VerticesCount();
  std::vector<int> paths(total_vertices, INT32_MAX);  // paths - длина пути до каждой вершины от вершины vertex,
  // по-умолчанию, это расстояниие равно INT32_MAX (ещё непосещенна)
  std::vector<int> parents(total_vertices, -1);  // parents - предки каждой вершины для текущего обхода,
  // т.е.: parents[vertex] - вершины из которой мы добрались до vertex
  // по-умолчанию, предок -1 (вершина ещё непосещенна)
  std::queue<int> vertices_queue;
  vertices_queue.push(vertex);
  paths[vertex] = 0;

  while (!vertices_queue.empty())
  {
    vertex = vertices_queue.front();
    vertices_queue.pop();

    std::vector<int> next_vertices;
    graph->GetNextVertices(vertex, next_vertices);

    for (int next_vertex : next_vertices) // рассматриваем каждое ребро исходящее из vertex
    {
      if (paths[next_vertex] == INT32_MAX)      // вершина ещё не посещена
      {
        vertices_queue.push(next_vertex);       // добавим её для обхода
        paths[next_vertex] = paths[vertex] + 1; // укажим длину пути
        parents[next_vertex] = vertex;          // оговорим её предка
      }
      else if (next_vertex != parents[vertex])          // если же вершина был посещена - мы нашли цикл
        return paths[vertex] + paths[next_vertex] + 1;  // длина найденого цикла - это сумма длин длины
      // пути к вершине из которой мы дошли до уже посещённой
      // и самой посещённой + 1 (дополнить ребро до полного цикла)
    }
  }
  return total_vertices + 1;  // мы посетили все вершины и не нашли цикла
  // тогда вернём все вершины + 1 (цикла такой длины быть не может)
}

int ComputeMinCycle (const IGraph *const graph)
{
  int answer = graph->VerticesCount() + 1;
  int total_vertices = graph->VerticesCount();

  for (int i = 0; i < total_vertices; i++)    // запустим обход в ширину с поиском цикла
    answer = std::min(answer, Bfs(i, graph)); // минимизируем ответ

  return answer == total_vertices + 1 ? -1 : answer; // вернём -1 если граф ацикличен
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