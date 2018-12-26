/*
 *
 *  РЕАЛИЗАЦИЯ : CListGraph
 *
 */


/*
 *
 * УСЛОВИЕ
 * Дан ориентированный граф. Определите, какое минимальное число ребер
 * необходимо добавить, чтобы граф стал сильносвязным.
 *
 * ФОРМАТ ВВОДА
 * В первой строке указывается число вершин графа V, во второй – число
 * ребер E, в последующих – E пар вершин, задающих ребра.
 *
 * ФОРМАТ ВЫВОДА
 * Минимальное число ребер k.
 *
 */



#include <algorithm>
#include <cassert>
#include <iostream>
#include <stack>
#include <vector>


using namespace std;



///////////////////// CLASS DECLARATION BEGIN /////////////////////

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

  void AddEdge (int from, int to) override;

  int VerticesCount () const override;

  void GetNextVertices (int vertex, std::vector<int> &vertices) const override;

  void GetPrevVertices (int vertex, std::vector<int> &vertices) const override;

};


/************************* GRAPHCONDENSER *************************/


class GraphCondenser
{

private:

  const IGraph *graph;

  int component_counter;
  vector<int> vertices_component;

public:

  GraphCondenser (const IGraph *graph_to_condense);

  // функция для нахождения минимального число ребер
  // необходимо добавить, чтобы граф стал сильносвязным.
  int ComputeAdditionalEdges ();

private:

  // функция выполняющая препроцессинг, в том числе и конденсацию,
  // а также нахождение компонент стльной связности
  void preprocess_ ();

  // функция для запуска серий dfsForOrder_ графа, которая записывает вершины в
  // порядке увеличения времени выхода - список вершин - order
  void makeOrder_ (vector<bool> &visited, vector<int> &order);

  void dfsForOrder_ (int vertex, vector<bool> &visited, vector<int> &order);

  // функция для запуска серий обходов dfsForComponent_ этого графа в порядке,
  // определяемом списком order (а именно, в обратном порядке, т.е. в порядке
  // уменьшения времени выхода): каждое множество вершин, достигнутое в результате
  // очередного запуска обхода, и будет очередной компонентой сильной связности
  void condense_ (vector<int> &order, vector<bool> &visited, vector<int> &component);

  void dfsForComponent_ (int vertex, vector<bool> &visited, vector<int> &component);

  // отметить очередную компоненту в vertices_component
  // после каждой серии запусков dfsForComponent_
  void addComponent_ (const vector<int> &component);
};

////////////////////// CLASS DECLARATION END //////////////////////


/*
 *
 * Функция для считывания графа из консоли.
 *
 *      IGraph *graph   - указатель на базовый класс графа
 *      int total_edges - количество рёбер графа
 *
 */
void ParseGraph (IGraph *const graph, int total_edges);


int main ()
{
  int total_vertices = 0, total_edges = 0;
  cin >> total_vertices >> total_edges;

  CListGraph graph(total_vertices);
  ParseGraph(&graph, total_edges);

  GraphCondenser condenser(&graph);
  cout << condenser.ComputeAdditionalEdges() << endl;

  return 0;
}


void ParseGraph (IGraph *const graph, int total_edges)
{
  int from = -1, to = -1;
  for (int i = 0; i < total_edges; i++)
  {
    cin >> from >> to;
    // нумерация вершин с единицы
    graph->AddEdge(--from, --to);
  }
}



///////////////////// CLASS DECLARATION BEGIN /////////////////////

CListGraph::CListGraph (const IGraph *other) : CListGraph(other->VerticesCount())
{
  std::vector<int> vertices;
  for (int from = 0; from < total_vertices_; from++)
  {
    vertices.clear();
    other->GetNextVertices(from, vertices);
    for (int to: vertices)
      AddEdge(from, to);
  }
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
  vertices = graph_container_in_[vertex];
}

void CListGraph::GetPrevVertices (int vertex, std::vector<int> &vertices) const
{
  vertices = graph_container_out_[vertex];
}


/************************* GRAPHCONDENSER *************************/


GraphCondenser::GraphCondenser (const IGraph *graph_to_condense) :
    graph(graph_to_condense)
{
  preprocess_();
}

int GraphCondenser::ComputeAdditionalEdges ()
{
  if (component_counter == 1)
    return 0;

  vector<pair<int, int> > inner_outter(component_counter, pair<int, int>(0, 0));
  vector<int> next_vertices;

  for (int from = 0; from < graph->VerticesCount(); from++)
  {
    graph->GetNextVertices(from, next_vertices);
    for (int to: next_vertices)
    {
      if (vertices_component[from] != vertices_component[to])
      {
        inner_outter[vertices_component[from]].second++;
        inner_outter[vertices_component[to]].first++;
      }
    }
  }

  int isolated = 0, only_inner = 0, only_outter = 0;
  for (pair<int, int> &cnt: inner_outter)
  {
    if (cnt.first != 0 && cnt.second == 0)
      only_outter++;
    if (cnt.second != 0 && cnt.first == 0)
      only_inner++;
    if (cnt.first == 0 && cnt.second == 0)
      isolated++;
  }

  return isolated + max(only_inner, only_outter);
}

void GraphCondenser::preprocess_ ()
{
  vector<int> order;
  vector<bool> visited(graph->VerticesCount(), false);
  makeOrder_(visited, order);
  visited.assign(graph->VerticesCount(), false);
  component_counter = 0;
  vertices_component.resize(graph->VerticesCount());
  vector<int> component;
  condense_(order, visited, component);
}

void GraphCondenser::makeOrder_ (vector<bool> &visited, vector<int> &order)
{
  for (int vertex = 0; vertex < graph->VerticesCount(); vertex++)
    if (!visited[vertex])
      dfsForOrder_(vertex, visited, order);
}

void GraphCondenser::dfsForOrder_ (int vertex, vector<bool> &visited, vector<int> &order)
{
  stack<int> recursion;
  recursion.push(vertex);
  vector<int> next_vertices;
  while (!recursion.empty())
  {
    vertex = recursion.top();
    recursion.pop();
    if (visited[vertex])
    {
      order.push_back(vertex);
      continue;
    }
    visited[vertex] = true;
    recursion.push(vertex);
    graph->GetNextVertices(vertex, next_vertices);
    for (int next_vertex: next_vertices)
      if (!visited[next_vertex])
        recursion.push(next_vertex);
  }
}

void GraphCondenser::condense_ (vector<int> &order, vector<bool> &visited, vector<int> &component)
{
  // Выполняем серию обходов по транспонированному графу в обратном порядке.
  for (int vertex = graph->VerticesCount() - 1; vertex >= 0; vertex--)
  {
    int next_vertex = order[vertex];
    if (!visited[next_vertex])
    {
      component.clear();
      dfsForComponent_(next_vertex, visited, component);
      // отметиим новую компоненту сильной связности
      addComponent_(component);
    }
  }
}

void GraphCondenser::dfsForComponent_ (int vertex, vector<bool> &visited, vector<int> &component)
{
  stack<int> recursion;
  recursion.push(vertex);
  vector<int> prev_vertices;
  while (!recursion.empty())
  {
    vertex = recursion.top();
    recursion.pop();
    visited[vertex] = true;
    component.push_back(vertex);
    // Выполняем обход по транспонированному графу
    graph->GetPrevVertices(vertex, prev_vertices);
    for (int prev_vertex: prev_vertices)
      if (!visited[prev_vertex])
        recursion.push(prev_vertex);
  }
}

/*
 *
 * Функция для записи очередной компоненты.
 *
 *      const vector<int> &component - список из вершин входящих в
 *      очередную компоненту сильной связности, т.е. component_counter-ая компонента
 *
 */
void GraphCondenser::addComponent_ (const vector<int> &component)
{
  for (int vertex: component)
    vertices_component[vertex] = component_counter;
  component_counter++;
}

////////////////////// CLASS DECLARATION END //////////////////////
