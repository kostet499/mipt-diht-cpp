//
// Created by mvcion on 11/19/18.
//

#ifndef GEOMETRY_CPOINT2D_H
#define GEOMETRY_CPOINT2D_H

#include <cmath>
#include <iostream>
#include <initializer_list>
#include <iomanip>
#include <stdexcept>


class CPoint2D
{
public:

  explicit CPoint2D () = default;

  explicit CPoint2D (double x, double y) :
      x_axis(x), y_axis(y)
  {}

  CPoint2D (const std::initializer_list<double> &point)
  {
    if (point.size() != 2)
    {
      throw std::invalid_argument("Invalid dimension size.");
    }
    x_axis = *point.begin();
    y_axis = *(point.begin() + 1);
  }

  CPoint2D &operator= (const CPoint2D &point) = default;

  CPoint2D (const CPoint2D &point) = default;

  bool operator== (const CPoint2D &point) const
  {
    return fabs(x_axis - point.x_axis) < GetEpsilon() &&
           fabs(y_axis - point.y_axis) < GetEpsilon();
  }

  double X () const
  {
    return x_axis;
  }

  double Y () const
  {
    return y_axis;
  }

  static constexpr double GetEpsilon ()
  {
    return 1e-8;
  }

  ~CPoint2D () = default;

  friend std::istream &operator>> (std::istream &is, CPoint2D &point2d);

  friend std::ostream &operator<< (std::ostream &os, const CPoint2D &point2d);

protected:

  double x_axis, y_axis;
};


std::istream &operator>> (std::istream &is, CPoint2D &point2d)
{
  is >> point2d.x_axis >> point2d.y_axis;
  return is;
}

std::ostream &operator<< (std::ostream &os, const CPoint2D &point2d)
{
  os << std::fixed;
  os << std::setprecision(9);
  os << point2d.x_axis << " " << point2d.y_axis;
  return os;
}

#endif //GEOMETRY_CPOINT2D_H
