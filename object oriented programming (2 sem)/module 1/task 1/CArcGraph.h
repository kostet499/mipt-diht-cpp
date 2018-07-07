//
//  CArcGraph.h
//  sem2task1
//
//  Created by Mykhail on 10.02.18.
//  Copyright (c) 2018 ___MVCION___. All rights reserved.
//

#ifndef __sem2task1__CArcGraph__
#define __sem2task1__CArcGraph__

#include <vector>

#include "IGraph.h"


class CArcGraph: public IGraph
{
  
 private:
  
  int total_vertices_;
  std::vector<std::pair<int, int> > graph_container_;
  
 public:
  
  CArcGraph (int n): total_vertices_(n) {}
  CArcGraph (const IGraph * other);
  ~CArcGraph() override;
  void AddEdge (int from, int to) override;
  int VerticesCount() const override;
  void GetNextVertices (int vertex, std::vector<int> & vertices) const override;
  void GetPrevVertices (int vertex, std::vector<int> & vertices) const override;
  
};

#endif /* defined(__sem2task1__CArcGraph__) */
