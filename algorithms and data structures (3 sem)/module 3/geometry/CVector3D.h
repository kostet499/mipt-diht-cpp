//
// Created by mvcion on 11/19/18.
//

#ifndef GEOMETRY_CVECTOR3D_H
#define GEOMETRY_CVECTOR3D_H


#include "CPoint3D.h"


struct CVector3D : public CPoint3D
{

  explicit CVector3D () = default;

  explicit CVector3D (double x, double y, double z) :
      CPoint3D(x, y, z)
  {}

  CVector3D (const std::initializer_list<double> &vector3d) :
      CPoint3D(vector3d)
  {}

  CVector3D &operator= (const CVector3D &point) = default;

  CVector3D (const CVector3D &point) = default;

  ~CVector3D () = default;

  CVector3D &operator+= (const CVector3D &vector3d)
  {
    x_axis += vector3d.x_axis;
    y_axis += vector3d.y_axis;
    z_axis += vector3d.z_axis;
    return *this;
  }

  CVector3D &operator*= (double multiplier)
  {
    x_axis *= multiplier;
    y_axis *= multiplier;
    z_axis *= multiplier;
    return *this;
  }

  const CVector3D operator- () const
  {
    CVector3D antivector3d(*this);
    antivector3d *= -1.0;
    return antivector3d;
  }

  CVector3D &operator-= (const CVector3D &vector3d)
  {
    (*this) += -vector3d;
    return *this;
  }

  const double ComputeDot (const CVector3D &vector3d) const
  {
    return x_axis * vector3d.x_axis +
           y_axis * vector3d.y_axis + z_axis * vector3d.z_axis;
  }

  const CVector3D ComputeCross (const CVector3D &vector3d) const
  {
    return CVector3D(
        y_axis * vector3d.z_axis - vector3d.y_axis * z_axis,
        -(x_axis * vector3d.z_axis - vector3d.x_axis * z_axis),
        x_axis * vector3d.y_axis - vector3d.x_axis * y_axis
    );
  }

  double Abs () const
  {
    return sqrt(this->ComputeDot(*this));
  }
};

CVector3D operator+ (const CVector3D &lhs, const CVector3D &rhs)
{
  CVector3D new_vector(lhs);
  new_vector += rhs;
  return new_vector;
}

CVector3D operator- (const CVector3D &lhs, const CVector3D &rhs)
{
  return lhs + (-rhs);
}

#endif //GEOMETRY_CVECTOR3D_H
