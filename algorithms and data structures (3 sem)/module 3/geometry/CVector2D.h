//
// Created by mvcion on 11/19/18.
//

#ifndef GEOMETRY_CVECTOR2D_H
#define GEOMETRY_CVECTOR2D_H

#include "CPoint2D.h"


struct CVector2D : public CPoint2D
{

  explicit CVector2D () = default;

  explicit CVector2D (double x, double y) :
      CPoint2D(x, y)
  {}

  CVector2D (const std::initializer_list<double> &vector3d) :
      CPoint2D(vector3d)
  {}

  CVector2D &operator= (const CVector2D &point) = default;

  CVector2D (const CVector2D &point) = default;

  ~CVector2D () = default;

  CVector2D &operator+= (const CVector2D &vector2d)
  {
    x_axis += vector2d.x_axis;
    y_axis += vector2d.y_axis;
    return *this;
  }

  CVector2D &operator*= (double scale)
  {
    x_axis *= scale;
    y_axis *= scale;
    return *this;
  }

  const CVector2D operator- () const
  {
    CVector2D antivector2d(*this);
    antivector2d *= -1.0;
    return antivector2d;
  }

  CVector2D &operator-= (const CVector2D &vector2d)
  {
    (*this) += -vector2d;
    return *this;
  }

  double ComputeDot (const CVector2D &vector2d) const
  {
    return x_axis * vector2d.x_axis +
           y_axis * vector2d.y_axis;
  }

  const CVector2D ComputeCross (const CVector2D &vector2d) const
  {
    return CVector2D(
        x_axis * vector2d.y_axis,
        -y_axis * vector2d.x_axis
    );
  }

  double ComputeNorm () const
  {
    return this->ComputeDot(*this);
  }

  double Abs () const
  {
    return sqrt(this->ComputeNorm());
  }
};

CVector2D operator+ (const CVector2D &lhs, const CVector2D &rhs)
{
  CVector2D new_vector(lhs);
  new_vector += rhs;
  return new_vector;
}

CVector2D operator- (const CVector2D &lhs, const CVector2D &rhs)
{
  return lhs + (-rhs);
}

#endif //GEOMETRY_CVECTOR2D_H
