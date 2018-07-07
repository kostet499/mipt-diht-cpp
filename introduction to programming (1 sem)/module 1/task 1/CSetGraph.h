//
//  CSetGraph.h
//  sem2task1
//
//  Created by Mykhail on 10.02.18.
//  Copyright (c) 2018 ___MVCION___. All rights reserved.
//

#ifndef __sem2task1__CSetGraph__
#define __sem2task1__CSetGraph__

#include <set>
#include <vector>

#include "IGraph.h"


class CSetGraph: public IGraph
{
  
 private:
  
  int total_vertices_;
  std::vector<std::set<int> > graph_container_in_;
  std::vector<std::set<int> > graph_container_out_;
  
 public:
  
  CSetGraph (int n): total_vertices_(n),
    graph_container_in_(n), graph_container_out_(n) {}
  CSetGraph (const IGraph * other);
  ~CSetGraph() override;
  void AddEdge (int from, int to) override;
  int VerticesCount() const override;
  void GetNextVertices (int vertex, std::vector<int> & vertices) const override;
  void GetPrevVertices (int vertex, std::vector<int> & vertices) const override;
  
};

#endif /* defined(__sem2task1__CSetGraph__) */
