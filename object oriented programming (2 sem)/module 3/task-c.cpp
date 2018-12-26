#include <iostream>
#include <limits>
#include <tuple>
#include <queue>
#include <vector>
#include <unordered_map>
#include <utility>


using namespace std;

class CNetworkComputer
{

public:

  explicit CNetworkComputer ();

  explicit CNetworkComputer (unsigned int nodes);

  void IncreaseCapacity (int from, int to, int delta = 1);

  int ComputeMaxFlow ();

  int ComputeMaxFlow (int source, int sink);

  void GetReachableVertices (int source, vector<int> &vertices) const;

private:

  int dinic (int source, int sink);

  bool bfs (int source, int sink);

  int dfs (int from, int sink, int flow);

private:

  unsigned int total_nodes = 0;

  vector<vector<int> > capacity;

  vector<int> depth, pointers;

};

class CHamming
{

public:

  tuple<int, string, string> operator() (const string &str, const string &pattern);

private:

  void buildGraph ();

  void buildNetwork ();

  void fillStrings ();

  int restoreByHamming ();

  int getNode (int index, bool from_pattern = true) const;

  tuple<int, int> getIndex (int node) const;

private:

  unsigned int total_vertices = 0;

  int source = std::numeric_limits<int>::min(),
      sink = std::numeric_limits<int>::max();

  string str = "", pattern = "";

  unordered_map<int, int> table;

  CNetworkComputer network;
};


int main ()
{
  string str, pattern;
  cin >> str >> pattern;
  int hamming_dist = 0;
  CHamming hamming;
  tie(hamming_dist, str, pattern) = hamming(str, pattern);
  cout << hamming_dist << endl << str << endl << pattern;
  return 0;
}

CNetworkComputer::CNetworkComputer () : total_nodes(2)
{
  capacity.assign(total_nodes, vector<int>(total_nodes, 0));
}

CNetworkComputer::CNetworkComputer (unsigned int nodes) : total_nodes(nodes)
{
  capacity.resize(total_nodes, vector<int>(total_nodes, 0));
}

void CNetworkComputer::IncreaseCapacity (int from, int to, int delta)
{
  capacity[from][to] += delta;
}

int CNetworkComputer::ComputeMaxFlow ()
{
  return ComputeMaxFlow(0, total_nodes - 1);
}

int CNetworkComputer::ComputeMaxFlow (int source, int sink)
{
  return dinic(source, sink);
}

int CNetworkComputer::dinic (int source, int sink)
{
  int total_flow = 0;
  pointers.assign(total_nodes, 0);
  while (bfs(source, sink))
  {
    int current_flow = 0;
    std::fill(pointers.begin(), pointers.end(), 0);
    do
    {
      current_flow = dfs(source, sink, std::numeric_limits<int>::max());
      total_flow += current_flow;
    } while (current_flow != 0);
  }
  return total_flow;
}

bool CNetworkComputer::bfs (int source, int sink)
{
  depth.assign(total_nodes, -1);
  depth[source] = 0;
  queue<int> nodes;
  nodes.emplace(source);
  vector<int> visited(total_nodes, false);
  visited[source] = true;
  while (!nodes.empty())
  {
    int from = nodes.front();
    nodes.pop();
    for (int to = 0; to < total_nodes; to++)
    {
      if (capacity[from][to] > 0 && !visited[to])
      {
        depth[to] = depth[from] + 1;
        nodes.push(to);
        visited[to] = true;
        if (to == sink)
        {
          return true;
        }
      }
    }
  }
  return static_cast<bool>(visited[sink]);
}

int CNetworkComputer::dfs (int from, int sink, int flow)
{
  if (flow == 0 || from == sink)
  {
    return flow;
  }
  else
  {
    for (int to = pointers[from]; to < total_nodes; to++)
    {
      if (depth[to] == depth[from] + 1)
      {
        int possible_flow = min(flow, capacity[from][to]);
        int pushed = dfs(to, sink, possible_flow);
        if (pushed != 0)
        {
          capacity[from][to] -= pushed;
          capacity[to][from] += pushed;
          return pushed;
        }
      }
      pointers[from]++;
    }
    return 0;
  }
}

void CNetworkComputer::GetReachableVertices (int source, vector<int> &vertices) const
{
  queue<int> nodes;
  nodes.push(source);
  vector<int> visited(total_nodes, false);
  visited[source] = true;
  while (!nodes.empty())
  {
    int from = nodes.front();
    nodes.pop();
    for (int to = 0; to < total_nodes; to++)
    {
      if (capacity[from][to] > 0 && !visited[to])
      {
        vertices.emplace_back(to);
        nodes.push(to);
        visited[to] = true;
      }
    }
  }
}

tuple<int, string, string> CHamming::operator() (const string &str, const string &pattern)
{
  this->str = str;
  this->pattern = pattern;
  buildGraph();
  buildNetwork();
  return make_tuple(restoreByHamming(), this->str, this->pattern);
}

void CHamming::buildGraph ()
{
  // decoded nodes -> <code, node_no>
  // encoded nodes -> <node_no, code>
  table.clear();
  total_vertices = 0;
  table.emplace(source, total_vertices);
  table.emplace(total_vertices, source);
  for (int i = 0; i < pattern.size(); i++)
  {
    if (pattern[i] == '?')
    {
      table.emplace(-i - 1, ++total_vertices);
      table.emplace(total_vertices, -i - 1);
    }
  }
  for (int i = 0; i < str.size(); i++)
  {
    if (str[i] == '?')
    {
      table.emplace(-i - 1 - pattern.size(), ++total_vertices);
      table.emplace(total_vertices, -i - 1 - pattern.size());
    }
  }
  table.emplace(sink, ++total_vertices);
  table.emplace(total_vertices, sink);
  total_vertices++;
}

void CHamming::buildNetwork ()
{
  network = CNetworkComputer(total_vertices);
  size_t str_len = str.length(), pattern_len = pattern.length();
  for (size_t str_pos = 0; str_pos < str_len - pattern_len + 1; str_pos++)
  {
    for (size_t pattern_pos = 0; pattern_pos < pattern_len; pattern_pos++)
    {
      if (pattern[pattern_pos] == '?' && str[str_pos + pattern_pos] == '?')
      {
        network.IncreaseCapacity(getNode(pattern_pos), getNode(str_pos + pattern_pos, false));
        network.IncreaseCapacity(getNode(str_pos + pattern_pos, false), getNode(pattern_pos));
      }
      else if (pattern[pattern_pos] == '1' && str[str_pos + pattern_pos] == '?')
      {
        network.IncreaseCapacity(getNode(source), getNode(str_pos + pattern_pos, false));
      }
      else if (pattern[pattern_pos] == '?' && str[str_pos + pattern_pos] == '1')
      {
        network.IncreaseCapacity(getNode(source), getNode(pattern_pos));
      }
      else if (pattern[pattern_pos] == '0' && str[str_pos + pattern_pos] == '?')
      {
        network.IncreaseCapacity(getNode(str_pos + pattern_pos, false), getNode(sink));
      }
      else if (pattern[pattern_pos] == '?' && str[str_pos + pattern_pos] == '0')
      {
        network.IncreaseCapacity(getNode(pattern_pos), getNode(sink));
      }
    }
  }
}

void CHamming::fillStrings ()
{
  vector<int> encoded;
  network.GetReachableVertices(getNode(source), encoded);
  for (int node: encoded)
  {
    int index, from_pattern;
    tie(index, from_pattern) = getIndex(node);
    if (from_pattern)
    {
      pattern[index] = '1';
    }
    else
    {
      str[index] = '1';
    }
  }
  for (char &sign : pattern)
  {
    if (sign == '?')
    {
      sign = '0';
    }
  }
  for (char &sign : str)
  {
    if (sign == '?')
    {
      sign = '0';
    }
  }
}

int CHamming::restoreByHamming ()
{
  network.ComputeMaxFlow();
  fillStrings();
  int distance = 0;
  size_t str_len = str.length(),
      pattern_len = pattern.length();
  for (size_t str_pos = 0; str_pos < str_len - pattern_len + 1; str_pos++)
  {
    for (size_t pattern_pos = 0; pattern_pos < pattern_len; pattern_pos++)
    {
      if (str[str_pos + pattern_pos] != pattern[pattern_pos])
      {
        distance++;
      }
    }
  }
  return distance;
}

int CHamming::getNode (int index, bool from_pattern) const
{
  if (index == std::numeric_limits<int>::min())
  {
    return 0;
  }
  else if (index == std::numeric_limits<int>::max())
  {
    return total_vertices - 1;
  }
  else if (from_pattern)
  {
    return table.at(-index - 1);
  }
  else
  {
    return table.at(-index - 1 - pattern.size());
  }
}

tuple<int, int> CHamming::getIndex (int node) const
{
  int code = table.at(node);
  if (code >= -(int) pattern.size())
  {
    return make_tuple(-(code + 1), true);
  }
  else
  {
    return make_tuple(-(code + 1 + pattern.size()), false);
  }
}