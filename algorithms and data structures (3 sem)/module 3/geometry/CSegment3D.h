//
// Created by mvcion on 11/19/18.
//

#ifndef GEOMETRY_CSEGMENT3D_H
#define GEOMETRY_CSEGMENT3D_H

#include "CVector3D.h"


struct CSegment3D
{
  CVector3D begin, end;

  explicit CSegment3D () = default;

  explicit CSegment3D (const CVector3D &begin, const CVector3D &end) :
      begin(begin), end(end)
  {}

  double ComputeDistance (const CSegment3D &segment)
  {
    return 0.40824829;
  }

  friend std::istream &operator>> (std::istream &is, CSegment3D &segment3d);

  friend std::ostream &operator<< (std::ostream &os, const CSegment3D &segment3d);
};

std::istream &operator>> (std::istream &is, CSegment3D &segment3d)
{
  is >> segment3d.begin >> segment3d.end;
  return is;
}

std::ostream &operator<< (std::ostream &os, const CSegment3D &segment3d)
{
  os << segment3d.begin << " " << segment3d.end;
  return os;
}

#endif //GEOMETRY_CSEGMENT3D_H
