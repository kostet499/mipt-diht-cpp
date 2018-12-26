#include <bits/stdc++.h>

using namespace std;

class CMatrixGraph
{

protected:

  unsigned int vertices = 0, edges = 0;

  vector<vector<int> > container;

public:

  CMatrixGraph () = default;

  CMatrixGraph (unsigned int _vertices, unsigned int _edges);

  CMatrixGraph (const CMatrixGraph &graph);

  unsigned int VerticesCount () const;

  unsigned int EdgesCount () const;

  void AddEdge (int begin, int end, int weight);

  void GetNextEdges (int vertex, vector<tuple<int, int> > &edges) const;

};

istream &operator>> (istream &is, CMatrixGraph &graph);

class Network : public CMatrixGraph
{

  vector<vector<int> > flow_system;
  vector<int> depth, ptr;

public:

  explicit Network (const CMatrixGraph &graph);

  int ComputeMaxFlow ();

  int ComputeMaxFlow (int source, int sink);

private:

  int dinic (int source, int sink);

  bool bfs (int source, int sink);

  int dfs (int from, int sink, int flow);

};


int main ()
{
  unsigned int vertices = 0, edges = 0;
  cin >> vertices >> edges;
  CMatrixGraph graph(vertices, edges);
  cin >> graph;
  Network network(graph);
  cout << network.ComputeMaxFlow() << endl;
  return 0;
}


CMatrixGraph::CMatrixGraph (unsigned int _vertices, unsigned int _edges) :
    vertices(_vertices), edges(_edges),
    container(_vertices, vector<int>(_vertices, 0)) {}

CMatrixGraph::CMatrixGraph (const CMatrixGraph &graph) :
    CMatrixGraph(graph.VerticesCount(), graph.EdgesCount())
{
  vector<tuple<int, int> > edges;
  for (int vertex = 0; vertex < vertices; vertex++)
  {
    graph.GetNextEdges(vertex, edges);
    for (auto edge: edges)
    {
      int to = -1, weight = 0;
      tie(to, weight) = edge;
      container[vertex][to] = weight;
    }
  }
}

unsigned int CMatrixGraph::VerticesCount () const
{
  return vertices;
}

unsigned int CMatrixGraph::EdgesCount () const
{
  return edges;
}

void CMatrixGraph::AddEdge (int begin, int end, int weight)
{
  container[begin][end] += weight;
}

void CMatrixGraph::GetNextEdges (int vertex, vector<tuple<int, int> > &edges) const
{
  edges.clear();
  for (int i = 0; i < vertices; i++)
  {
    if (container[vertex][i] > 0)
    {
      edges.emplace_back(i, container[vertex][i]);
    }
  }
}

istream &operator>> (istream &is, CMatrixGraph &graph)
{
  int begin = 0, end = 0, weight = 0;
  for (int i = 0; i < graph.EdgesCount(); i++)
  {
    is >> begin >> end >> weight;
    graph.AddEdge(--begin, --end, weight);
  }
  return is;
}

Network::Network (const CMatrixGraph &graph) :
    CMatrixGraph(graph),
    flow_system(vertices, vector<int>(vertices, 0)) {}

int Network::ComputeMaxFlow ()
{
  return ComputeMaxFlow(0, vertices - 1);
}

int Network::ComputeMaxFlow (int source, int sink)
{
  return dinic(source, sink);
}

int Network::dinic (int source, int sink)
{
  int total_flow = 0;
  while (bfs(source, sink))
  {
    int current_flow = 0;
    do
    {
      ptr.assign(vertices, 0);
      current_flow = dfs(source, sink, INT32_MAX);
      total_flow += current_flow;
    } while (current_flow != 0);
  }
  return total_flow;
}

bool Network::bfs (int source, int sink)
{
  depth.assign(vertices, INT32_MIN);
  depth[source] = 0;
  queue<int> nodes;
  nodes.push(source);
  vector<bool> visited(vertices, false);
  visited[source] = true;
  while (!nodes.empty())
  {
    int from = nodes.front();
    nodes.pop();
    for (int to = 0; to < vertices; to++)
    {
      if (flow_system[from][to] < container[from][to] && !visited[to])
      {
        depth[to] = depth[from] + 1;
        nodes.push(to);
        visited[to] = true;
      }
    }
  }
  return visited[sink];
}

int Network::dfs (int from, int sink, int flow)
{
  if (flow == 0 || from == sink)
  {
    return flow;
  }
  else
  {
    for (int to = ptr[from]; to < vertices; to++)
    {
      if (depth[to] == depth[from] + 1)
      {
        int possible_flow = min(flow, container[from][to] - flow_system[from][to]);
        int pushed = dfs(to, sink, possible_flow);
        if (pushed != 0)
        {
          flow_system[from][to] += pushed;
          flow_system[to][from] -= pushed;
          return pushed;
        }
      }
    }
    return 0;
  }
}
