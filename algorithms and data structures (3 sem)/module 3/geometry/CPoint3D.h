//
// Created by mvcion on 11/19/18.
//

#ifndef GEOMETRY_CPOINT3D_H
#define GEOMETRY_CPOINT3D_H

#include <cmath>
#include <iostream>
#include <initializer_list>
#include <iomanip>
#include <stdexcept>


class CPoint3D
{

public:

  explicit CPoint3D () = default;

  explicit CPoint3D (double x, double y, double z) :
      x_axis(x), y_axis(y), z_axis(z)
  {}

  CPoint3D (const std::initializer_list<double> &point)
  {
    if (point.size() != 3)
    {
      throw std::invalid_argument("Invalid dimension size.");
    }
    x_axis = *point.begin();
    y_axis = *(point.begin() + 1);
    z_axis = *(point.begin() + 2);
  }

  CPoint3D &operator= (const CPoint3D &point) = default;

  CPoint3D (const CPoint3D &point) = default;

  bool operator== (const CPoint3D &point) const
  {
    return fabs(x_axis - point.x_axis) < GetEpsilon() &&
           fabs(y_axis - point.y_axis) < GetEpsilon() &&
           fabs(z_axis - point.z_axis) < GetEpsilon();
  }

  double X () const
  {
    return x_axis;
  }

  double Y () const
  {
    return y_axis;
  }

  double Z () const
  {
    return z_axis;
  }

  constexpr double GetEpsilon () const
  {
    return 1e-8;
  }

  ~CPoint3D () = default;

  friend std::istream &operator>> (std::istream &is, CPoint3D &point3d);

  friend std::ostream &operator<< (std::ostream &os, const CPoint3D &point3d);

protected:

  double x_axis, y_axis, z_axis;
};

std::istream &operator>> (std::istream &is, CPoint3D &point3d)
{
  is >> point3d.x_axis >> point3d.y_axis >> point3d.z_axis;
  return is;
}

std::ostream &operator<< (std::ostream &os, const CPoint3D &point3d)
{
  os << std::fixed;
  os << std::setprecision(9);
  os << point3d.x_axis << " " << point3d.y_axis << " " << point3d.z_axis;
  return os;
}

#endif //GEOMETRY_CPOINT3D_H
