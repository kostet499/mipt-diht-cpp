//
//  CSetGraph.cpp
//  sem2task1
//
//  Created by Mykhail on 10.02.18.
//  Copyright (c) 2018 ___MVCION___. All rights reserved.
//

#include "CSetGraph.h"


CSetGraph::CSetGraph (const IGraph * other)
{
  total_vertices_ = other->VerticesCount();
  graph_container_in_.resize(total_vertices_);
  graph_container_out_.resize(total_vertices_);
  
  std::vector<int> vertices;
  for (int from = 0; from < total_vertices_; from++)
  {
    vertices.clear();
    other->GetNextVertices (from, vertices);
    for (int to: vertices)
      AddEdge(from, to);
  }
}

CSetGraph::~CSetGraph()
{
  graph_container_in_.clear();
  graph_container_out_.clear();
}

void CSetGraph::AddEdge (int from, int to)
{
  graph_container_in_[from].insert(to);
  graph_container_out_[to].insert(from);
}

int CSetGraph::VerticesCount() const
{
  return total_vertices_;
}

void CSetGraph::GetNextVertices (int vertex, std::vector<int> & vertices) const
{
  vertices.clear();
  for (int next_vertex: graph_container_in_[vertex])
    vertices.push_back(next_vertex);
}

void CSetGraph::GetPrevVertices (int vertex, std::vector<int> & vertices) const
{
  vertices.clear();
  for (int prev_vertex: graph_container_out_[vertex])
    vertices.push_back(prev_vertex);
}