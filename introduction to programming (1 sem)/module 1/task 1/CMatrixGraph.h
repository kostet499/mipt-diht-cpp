//
//  CMatrixGraph.h
//  sem2task1
//
//  Created by Mykhail on 10.02.18.
//  Copyright (c) 2018 ___MVCION___. All rights reserved.
//

#ifndef __sem2task1__CMatrixGraph__
#define __sem2task1__CMatrixGraph__

#include <assert.h>
#include <vector>

#include "IGraph.h"


class CMatrixGraph: public IGraph
{
  
 private:
  
  int total_vertices_;
  std::vector<std::vector <int> > graph_container_;
  
 public:
  
  CMatrixGraph (int n) : total_vertices_(n),
    graph_container_( n, std::vector<int>(n, 0) ) {}
  CMatrixGraph (const IGraph * other);
  ~CMatrixGraph() override;
  void AddEdge (int from, int to) override;
  int VerticesCount() const override;
  void GetNextVertices (int vertex, std::vector<int> & vertices) const override;
  void GetPrevVertices (int vertex, std::vector<int> & vertices) const override;
  
};

#endif /* defined(__sem2task1__CMatrixGraph__) */
