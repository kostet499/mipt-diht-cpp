/*
 *   ContestID: 15801452  
 */

#include <cmath>
#include <initializer_list>
#include <iomanip>
#include <iostream>
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

  CPoint3D (const CPoint3D &point) :
      x_axis(point.X()), y_axis(point.Y()), z_axis(point.Z())
  {}

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

  inline constexpr static double GetEpsilon ()
  {
    return 1e-10;
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


struct CVector3D : public CPoint3D
{

  explicit CVector3D () = default;

  explicit CVector3D (double x, double y, double z) :
      CPoint3D(x, y, z)
  {}

  CVector3D (const std::initializer_list<double> &vector3d) :
      CPoint3D(vector3d)
  {}

  explicit CVector3D (const CPoint3D &end) :
      CVector3D::CPoint3D(end)
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

  CVector3D &operator*= (double scale)
  {
    x_axis *= scale;
    y_axis *= scale;
    z_axis *= scale;
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

  double ComputeDot (const CVector3D &vector3d) const
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

  bool IsCollinearTo (const CVector3D &vector3d) const
  {
    double self_scalar_sqr = ComputeDot(*this);
    double scalar_lhs_on_rhs = ComputeDot(vector3d);
    double vector3d_scalar_sqr = vector3d.ComputeDot(vector3d);
    double D = self_scalar_sqr * vector3d_scalar_sqr -
               scalar_lhs_on_rhs * scalar_lhs_on_rhs;
    return D < CPoint3D::GetEpsilon();
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

CVector3D operator* (double scale, const CVector3D &rhs)
{
  CVector3D new_vector(rhs);
  new_vector *= scale;
  return new_vector;
}

CVector3D operator* (const CVector3D &lhs, double scale)
{
  CVector3D new_vector(lhs);
  new_vector *= scale;
  return new_vector;
}


struct CSegment3D
{
  CVector3D begin, end;

  explicit CSegment3D () = default;

  explicit CSegment3D (const CVector3D &begin, const CVector3D &end) :
      begin(begin), end(end)
  {}

  const CVector3D GetDirectiveVector () const
  {
    return this->end - this->begin;
  }

  double GetLength () const
  {
    return (end - begin).Abs();
  }

  double ComputeDistance (const CVector3D &vector3d) const
  {
    CVector3D directive_vec = GetDirectiveVector();
    CVector3D vec_begin = vector3d - begin;
    if (vec_begin.ComputeDot(directive_vec) <= 0.0)
    {
      return vec_begin.Abs();
    }
    CVector3D vec_end = vector3d - end;
    if (vec_end.ComputeDot(directive_vec) >= 0.0)
    {
      return vec_end.Abs();
    }
    return (directive_vec.ComputeCross(vec_begin)).Abs() / directive_vec.Abs();
  }

  double ComputeDistance (const CSegment3D &segment) const
  {
    CVector3D directive_vector = GetDirectiveVector();
    double closer_to_begin = 0;
    double closer_to_end = 1.0;
    while (closer_to_end - closer_to_begin >= CPoint3D::GetEpsilon())
    {
      double to_begin = closer_to_begin + (closer_to_end - closer_to_begin) / 3.0;
      double to_end = closer_to_end - (closer_to_end - closer_to_begin) / 3.0;
      if (segment.ComputeDistance(to_begin * directive_vector + begin) <=
          segment.ComputeDistance(to_end * directive_vector + begin))
      {
        closer_to_end = to_end;
      }
      else
      {
        closer_to_begin = to_begin;
      }
    }
    return segment.ComputeDistance(closer_to_begin * directive_vector + begin);
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


int main ()
{
  CSegment3D segment1, segment2;
  std::cin >> segment1 >> segment2;
  std::cout << std::fixed;
  std::cout << std::setprecision(10);
  std::cout << segment1.ComputeDistance(segment2);
  return 0;
}
