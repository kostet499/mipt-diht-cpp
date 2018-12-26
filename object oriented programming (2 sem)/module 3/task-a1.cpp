#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <set>
#include <vector>


using namespace std;


bool operator< (tuple<int, int> tuple1, tuple<int, int> tuple2)
{
  return get<1>(tuple1) < get<1>(tuple2);
}


int PredictGraph (unsigned int vertices, unsigned int edges)
{
  if (edges * log2(vertices) < vertices * vertices)
  {
    return 1;
  }
  else
  {
    return -1;
  }
}


class IGraph
{
protected:

  unsigned int vertices, edges;

public:

  IGraph (unsigned int _vertices, unsigned int _edges) :
      vertices(_vertices), edges(_edges) {}

  virtual void AddEdge (int begin, int end, int weight) = 0;

  virtual void GetNextEdges (int vertex, vector<tuple<int, int> > &edges) const = 0;

  virtual unsigned int VerticesCount () const;

  virtual unsigned int EdgesCount () const;

};

istream &operator>> (istream &is, IGraph &graph);


class CListGraph : public IGraph
{

  vector<vector<tuple<int, int> > > container;

public:

  CListGraph (unsigned int _vertices, unsigned int _edges) :
      IGraph(_vertices, _edges), container(_vertices) {}

  void AddEdge (int begin, int end, int weight) override;

  void GetNextEdges (int vertex, vector<tuple<int, int> > &edges) const override;
};


class CMatrixGraph : public IGraph
{

  vector<vector<int> > container;

public:

  CMatrixGraph (unsigned int _vertices, unsigned int _edges) :
      IGraph(_vertices, _edges),
      container(_vertices, vector<int>(_vertices, INT32_MAX)) {}

  void AddEdge (int begin, int end, int weight) override;

  void GetNextEdges (int vertex, vector<tuple<int, int> > &edges) const override;
};


class Prima
{
public:

  int operator() (const IGraph &graph) const;

private:

  int computeMstForDenseGraph_ (const IGraph &graph) const;

  void addToMst_ (const IGraph &graph, int begin,
                  vector<tuple<int, int> > &minimal_edges) const;

  int computeMstForSparseGraph_ (const IGraph &graph) const;

  void addToMst_ (const IGraph &graph, int begin,
                  vector<tuple<int, int> > &minimal_edges,
                  const vector<bool> &added,
                  set<tuple<int, int> > &edges_queue) const;
};


IGraph *ChooseGraph (unsigned int vertices, unsigned int edges);


int main ()
{
  freopen("kruskal.in", "r", stdin);
  freopen("kruskal.out", "w", stdout);
  unsigned int vertices = 0, edges = 0;
  cin >> vertices >> edges;
  IGraph *graph = ChooseGraph(vertices, edges);
  cin >> *graph;
  Prima prima;
  cout << prima(*graph) << endl;
  fclose(stdin);
  fclose(stdout);
  delete graph;
  return 0;
}


unsigned int IGraph::VerticesCount () const
{
  return this->vertices;
}

unsigned int IGraph::EdgesCount () const
{
  return this->edges;
}


istream &operator>> (istream &is, IGraph &graph)
{
  int begin = 0, end = 0, weight = 0;
  for (int i = 0; i < graph.EdgesCount(); i++)
  {
    is >> begin >> end >> weight;
    graph.AddEdge(--begin, --end, weight);
  }
  return is;
}


void CListGraph::AddEdge (int begin, int end, int weight)
{
  container[begin].emplace_back(end, weight);
  container[end].emplace_back(begin, weight);
}

void CListGraph::GetNextEdges (int vertex, vector<tuple<int, int> > &edges) const
{
  edges.clear();
  edges = container[vertex];
}


void CMatrixGraph::AddEdge (int begin, int end, int weight)
{
  container[begin][end] = container[end][begin] =
      min(weight, container[begin][end]);
}

void CMatrixGraph::GetNextEdges (int vertex, vector<tuple<int, int> > &edges) const
{
  edges.clear();
  for (int i = 0; i < VerticesCount(); i++)
  {
    if (container[vertex][i] != INT32_MAX)
    {
      edges.emplace_back(i, container[vertex][i]);
    }
  }
}


IGraph *ChooseGraph (unsigned int vertices, unsigned int edges)
{
  int prediction = PredictGraph(vertices, edges);
  if (prediction == 1)
  {
    return new CListGraph(vertices, edges);
  }
  else if (prediction == -1)
  {
    return new CMatrixGraph(vertices, edges);
  }
  else
  {
    return nullptr;
  }
}


int Prima::operator() (const IGraph &graph) const
{
  int prediction = PredictGraph(graph.VerticesCount(), graph.EdgesCount());
  if (prediction == 1)
  {
    return computeMstForSparseGraph_(graph);
  }
  else if (prediction == -1)
  {
    return computeMstForDenseGraph_(graph);
  }
}

int Prima::computeMstForDenseGraph_ (const IGraph &graph) const
{
  int total = 0;
  vector<bool> added(graph.VerticesCount(), false);
  vector<tuple<int, int> > minimal_edges(graph.VerticesCount(), make_tuple(-1, INT32_MAX));
  get<1>(minimal_edges[0]) = 0;
  for (int iteration = 0; iteration < graph.VerticesCount(); iteration++)
  {
    int begin = -1;
    for (int vertex = 0; vertex < graph.VerticesCount(); vertex++)
    {
      if (!added[vertex])
      {
        if (begin == -1 || minimal_edges[vertex] < minimal_edges[begin])
        {
          begin = vertex;
        }
      }
    }
    added[begin] = true;
    total += get<1>(minimal_edges[begin]);
    addToMst_(graph, begin, minimal_edges);
  }
  return total;
}

void Prima::addToMst_ (const IGraph &graph, int begin,
                       vector<tuple<int, int> > &minimal_edges) const
{
  vector<tuple<int, int> > edges;
  graph.GetNextEdges(begin, edges);
  for (auto edge: edges)
  {
    int to = -1, weight = 0;
    tie(to, weight) = edge;
    if (weight < get<1>(minimal_edges[to]))
    {
      minimal_edges[to] = make_tuple(begin, weight);
    }
  }
}

int Prima::computeMstForSparseGraph_ (const IGraph &graph) const
{
  int total = 0;
  vector<tuple<int, int> > minimal_edges(graph.VerticesCount(), make_tuple(-1, INT32_MAX));
  get<1>(minimal_edges[0]) = 0;
  vector<bool> added(graph.VerticesCount(), false);
  set<tuple<int, int> > edges_queue;
  edges_queue.emplace(get<1>(minimal_edges[0]), 0);
  for (int iteration = 0; iteration < graph.VerticesCount(); iteration++)
  {
    int begin = -1, edge_weight = 0;
    tie(edge_weight, begin) = *edges_queue.begin();
    total += edge_weight;
    added[begin] = true;
    edges_queue.erase(edges_queue.begin());
    addToMst_(graph, begin, minimal_edges, added, edges_queue);
  }
  return total;
}

void Prima::addToMst_ (const IGraph &graph, int begin,
                       vector<tuple<int, int> > &minimal_edges,
                       const vector<bool> &added,
                       set<tuple<int, int> > &edges_queue) const
{
  vector<tuple<int, int> > edges;
  graph.GetNextEdges(begin, edges);
  for (auto edge: edges)
  {
    int to = -1, weight = 0;
    tie(to, weight) = edge;
    if (!added[to] && edge < minimal_edges[to])
    {
      edges_queue.erase(make_tuple(get<1>(minimal_edges[to]), to));
      minimal_edges[to] = make_tuple(begin, weight);
      edges_queue.emplace(get<1>(minimal_edges[to]), to);
    }
  }
}
