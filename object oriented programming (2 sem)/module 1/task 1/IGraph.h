//
//  IGraph.h
//  sem2task1
//
//  Created by Mykhail on 10.02.18.
//  Copyright (c) 2018 ___MVCION___. All rights reserved.
//

#ifndef __sem2task1__IGraph__
#define __sem2task1__IGraph__

#include <vector>


struct IGraph
{
  virtual ~IGraph() {}
  virtual void AddEdge(int from, int to) = 0;
	virtual int VerticesCount() const = 0;
  virtual void GetNextVertices(int vertex, std::vector<int> & vertices) const = 0;
  virtual void GetPrevVertices(int vertex, std::vector<int> & vertices) const = 0;
};

#endif /* defined(__sem2task1__IGraph__) */
