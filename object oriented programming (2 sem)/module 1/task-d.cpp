/*
 *
 *  РЕАЛИЗАЦИЯ : CMatrixGraph
 *
 */


/*
 *
 * УСЛОВИЕ
 * Дана матрица смежности ориентированного графа. Проверьте, содержит ли граф
 * вершину-сток. То есть вершину, в которую ведут ребра из всех вершин, и из
 * которой не выходит ни одного ребра. Требуемая сложность O(V).
 *
 * ФОРМАТ ВВОДА
 * В первой строке вводятся натуральное число N.
 * Далее идет матрица смежности ориентированного графа.
 *
 * ФОРМАТ ВЫВОДА
 * Требуется вывести YES - если такая вершина есть, NO - иначе.
 *
 */

#include <assert.h>
#include <iostream>
#include <vector>



///////////////////// CLASS DEFINITION BEGIN /////////////////////

struct IGraph
{

  virtual ~IGraph () {}

  virtual void AddEdge (int from, int to) = 0;

  virtual int VerticesCount () const = 0;

  virtual void GetNextVertices (int vertex, std::vector<int> &vertices) const = 0;

  virtual void GetPrevVertices (int vertex, std::vector<int> &vertices) const = 0;

  virtual bool EdgeExists (int from, int to) const = 0;

};

class CMatrixGraph : public IGraph
{

private:

  int total_vertices_;
  std::vector<std::vector<int> > graph_container_;

public:

  CMatrixGraph (int n) : total_vertices_(n),
                         graph_container_(n, std::vector<int>(n, 0)) {}

  CMatrixGraph (const IGraph *other);

  ~CMatrixGraph () override;

  void AddEdge (int from, int to) override;

  int VerticesCount () const override;

  void GetNextVertices (int vertex, std::vector<int> &vertices) const override;

  void GetPrevVertices (int vertex, std::vector<int> &vertices) const override;

  bool EdgeExists (int from, int to) const;

};

////////////////////// CLASS DEFINITION END //////////////////////


/*
 *
 * Функция для считывания графа из консоли.
 *
 *      IGraph *const graph   - константный указатель на базовый класс графа
 *      int total_edges - количество рёбер графа
 *
 */
void ParseGraph (IGraph *const graph);

/*
 *
 * Функция для считывания графа из консоли.
 *
 *     const IGraph *const graph - константный указатель на констаный объект,
 *         наследуемый от IGraph
 *
 */
bool CheckGraph (const IGraph *const graph);


int main ()
{

  int n = 0;
  std::cin >> n;
  assert(n > 0);

  IGraph *graph = new CMatrixGraph(n);
  ParseGraph(graph);

  std::cout << (CheckGraph(graph) ? "YES" : "NO") << std::endl;

  return 0;
}


void ParseGraph (IGraph *const graph)
{
  int total_vertices = graph->VerticesCount();
  int edge_exists = 0;
  for (int from = 0; from < total_vertices; from++)
    for (int to = 0; to < total_vertices; to++)
    {
      std::cin >> edge_exists;
      if (edge_exists)
        graph->AddEdge(from, to);
    }
}

bool CheckGraph (const IGraph *const graph)
{
  int total_vertices = graph->VerticesCount();

  // possible[vertex] -> vertex является стоком
  std::vector<bool> possible(total_vertices, true);

  // проверяем все вершины на возможность быть стоком
  for (int vertex = 0; vertex < total_vertices; vertex++)
  {
    if (!possible[vertex]) // ранее удостоверились что вершина не подходит
      continue;            // итерируемся далее

    // если эта вершина без петли (если да - по опредеоение не является стоком)
    if (!graph->EdgeExists(vertex, vertex))
    {
      int next_vertex = (vertex + 1) % total_vertices;
      // рассмотрим с какими соединена vertex
      while (next_vertex != vertex)
      {
        if (!graph->EdgeExists(vertex, next_vertex))
          // в next_vertex не входят рёбра из всех остальных - не сток
          possible[next_vertex] = false;
        else
        {
          // из vertex выходит минимум одно ребро - не сток
          possible[vertex] = false;
          break;
        }
        next_vertex = (next_vertex + 1) % total_vertices;
      }

      // если мы прошли все вершины (их ровно total_vertices - 1), которые могут быть
      // соединены с verteх и убедились, что ребра выходят из verteх и входят в них
      if (next_vertex == vertex)
      {
        // убедимся, что из vertex не исходят рёбра
        for (int other_vertex = 0; other_vertex < total_vertices; other_vertex++)
          if (!graph->EdgeExists(other_vertex, vertex) && other_vertex != vertex)
            return false;
        return true;
      }
    }
    else // эта вершина с петлей (по опредеоение не является стоком)
      possible[vertex] = false;

  }
  return false;
}


///////////////////// CLASS DEFINITION BEGIN /////////////////////

CMatrixGraph::CMatrixGraph (const IGraph *other)
{
  total_vertices_ = other->VerticesCount();
  graph_container_.resize(total_vertices_,
                          std::vector<int>(total_vertices_, 0));
  std::vector<int> vertices;
  for (int from = 0; from < total_vertices_; from++)
  {
    vertices.clear();
    other->GetNextVertices(from, vertices);
    for (int to: vertices)
      graph_container_[from][to] = 1;
  }
}

CMatrixGraph::~CMatrixGraph ()
{
  for (auto &vector_tmp: graph_container_)
    vector_tmp.clear();
  graph_container_.clear();
}

void CMatrixGraph::AddEdge (int from, int to)
{
  assert(from > -1 && from < graph_container_.size());
  assert(to > -1 && to < graph_container_.size());
  graph_container_[from][to] = 1;
}

int CMatrixGraph::VerticesCount () const
{
  return total_vertices_;
}

void CMatrixGraph::GetNextVertices (int vertex, std::vector<int> &vertices) const
{
  vertices.clear();
  int vertices_count = VerticesCount();
  for (int tmp_vertex = 0; tmp_vertex < vertices_count; tmp_vertex++)
    if (graph_container_[vertex][tmp_vertex] == 1)
      vertices.push_back(tmp_vertex);
}

void CMatrixGraph::GetPrevVertices (int vertex, std::vector<int> &vertices) const
{
  vertices.clear();
  int vertices_count = VerticesCount();
  for (int tmp_vertex = 0; tmp_vertex < vertices_count; tmp_vertex++)
    if (graph_container_[tmp_vertex][vertex] == 1)
      vertices.push_back(tmp_vertex);
}

bool CMatrixGraph::EdgeExists (int from, int to) const
{
  return graph_container_[from][to] == 1;
}

////////////////////// CLASS DEFINITION END //////////////////////
