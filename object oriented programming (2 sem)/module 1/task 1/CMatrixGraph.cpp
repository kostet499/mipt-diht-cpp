//
//  CMatrixGraph.cpp
//  sem2task1
//
//  Created by Mykhail on 10.02.18.
//  Copyright (c) 2018 ___MVCION___. All rights reserved.
//

#include "CMatrixGraph.h"


CMatrixGraph::CMatrixGraph (const IGraph * other)
{
  total_vertices_ = other->VerticesCount();
  graph_container_.resize( total_vertices_,
                          std::vector<int>(total_vertices_, 0) );
  std::vector<int> vertices;
  
  for (int from = 0; from < total_vertices_; from++)
  {
    vertices.clear();
    other->GetNextVertices(from, vertices);
    for (int to: vertices)
    {
      graph_container_[from][to] = 1;
      graph_container_[to][from] = -1;
    }
  }
}

CMatrixGraph::~CMatrixGraph()
{
  for (auto & vector_tmp: graph_container_)
    vector_tmp.clear();
  graph_container_.clear();
}

void CMatrixGraph::AddEdge (int from, int to)
{
  assert(from > -1 && from < graph_container_.size());
  assert(to > -1 && to < graph_container_.size());
  graph_container_[from][to] = 1;
  graph_container_[to][from] = -1;
}

int CMatrixGraph::VerticesCount() const
{
  return total_vertices_;
}

void CMatrixGraph::GetNextVertices (int vertex, std::vector<int> & vertices) const
{
  vertices.clear();
  int vertices_count = VerticesCount();
  for (int tmp_vertex = 0; tmp_vertex < vertices_count; tmp_vertex++)
    if (graph_container_[vertex][tmp_vertex] == 1)
      vertices.push_back(tmp_vertex);
  
}

void CMatrixGraph::GetPrevVertices (int vertex, std::vector<int> & vertices) const
{
  vertices.clear();
  int vertices_count = VerticesCount();
  for (int tmp_vertex = 0; tmp_vertex < vertices_count; tmp_vertex++)
    if (graph_container_[vertex][tmp_vertex] == -1)
      vertices.push_back(tmp_vertex);
}




