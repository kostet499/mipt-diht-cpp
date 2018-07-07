//
//  CArcGraph.cpp
//  sem2task1
//
//  Created by Mykhail on 10.02.18.
//  Copyright (c) 2018 ___MVCION___. All rights reserved.
//

#include "CArcGraph.h"


CArcGraph::CArcGraph (const IGraph * other)
{
  total_vertices_ = other->VerticesCount();
  
  std::vector<int> vertices;
  for (int from = 0; from < total_vertices_; from++)
  {
    vertices.clear();
    other->GetNextVertices(from, vertices);
    for (int to: vertices)
      AddEdge(from, to);
  }
}

CArcGraph::~CArcGraph()
{
  graph_container_.clear();
}

void CArcGraph::AddEdge (int from, int to)
{
  graph_container_.push_back( std::pair<int, int>(from, to) );
}

int CArcGraph::VerticesCount() const
{
  return total_vertices_;
}

void CArcGraph::GetNextVertices (int vertex, std::vector<int> & vertices) const
{
  vertices.clear();
  for (auto edge: graph_container_)
    if (edge.first == vertex)
      vertices.push_back(edge.second);
}

void CArcGraph::GetPrevVertices (int vertex, std::vector<int> & vertices) const
{
  vertices.clear();
  for (auto edge: graph_container_)
    if (edge.second == vertex)
      vertices.push_back(edge.first);
}
