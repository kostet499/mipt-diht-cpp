/*
 *   ContestID: 15975932  
 */

#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <vector>


namespace geometry2d
{
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

    inline static constexpr double GetEpsilon ()
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

  CVector2D operator* (double scale, const CVector2D &rhs)
  {
    CVector2D new_vector(rhs);
    new_vector *= scale;
    return new_vector;
  }

  CVector2D operator* (const CVector2D &lhs, double scale)
  {
    return scale * lhs;
  }
};

using geometry2d::CPoint2D;
using geometry2d::CVector2D;
using CPolygon2D = std::vector<CVector2D>;

std::istream &operator>> (std::istream &is, CPolygon2D &polygon)
{
  std::size_t total_vectors;
  std::cin >> total_vectors;
  polygon.resize(total_vectors);
  for (CVector2D &vector: polygon)
  {
    std::cin >> vector;
  }
  return is;
}


namespace minkovsky_space
{
  double ComputeArea (const CPoint2D &a, const CPoint2D &b, const CPoint2D &c)
  {
    return (b.X() - a.X()) * (c.Y() - a.Y()) - (b.Y() - a.Y()) * (c.X() - a.X());
  }

  double ComputeArea (const CPolygon2D &polygon, const CPoint2D &point)
  {
    double area = 0;
    std::size_t polygon_size = polygon.size();
    for (std::size_t i = 0; i < polygon_size; i++)
    {
      area += fabs(ComputeArea(point, polygon[i], polygon[(i + 1) % polygon_size]) / 2);
    }
    return area;
  }

  int CheckForClockwise(const CPoint2D &a, const CPoint2D &b, const CPoint2D &c)
  {
    double area = ComputeArea(a, b, c);
    if (fabs(area) < CPoint2D::GetEpsilon())
    {
      return 0;
    }
    else if (area > 0)
    {
      return -1;
    }
    else
    {
      return 1;
    }
  }

  int CheckForClockwise(const CVector2D &a, const CVector2D &b)
  {
    return CheckForClockwise(CPoint2D(0, 0), a, b);
  }

  void __reflectPolygon (CPolygon2D &polygon)
  {
    for (CVector2D &vector: polygon)
    {
      vector *= -1.0;
    }
  }

  void __shiftPolygon (CPolygon2D &polygon)
  {
    std::size_t start_index = 0;
    std::size_t polygon_size = polygon.size();
    for (std::size_t i = 1; i < polygon.size(); i++)
    {
      bool is_abscissa_less = polygon[i].X() < polygon[start_index].X();
      bool is_ordinate_less = polygon[i].Y() < polygon[start_index].Y()
          && fabs(polygon[i].X() - polygon[start_index].X()) < CPoint2D::GetEpsilon();
      if (is_abscissa_less || is_ordinate_less)
      {
        start_index = i;
      }
    }
    CPolygon2D shifted(polygon_size);
    for (std::size_t i = 0; i < polygon.size(); i++)
    {
      shifted[i] = polygon[(i + start_index) % polygon_size];
    }
    polygon = std::move(shifted);
  }

  void ComputeMinkovskySum (const CPolygon2D &polygon_a,
                            const CPolygon2D &polygon_b,
                            CPolygon2D &minkovsky_sum)
  {
    std::size_t a_size = polygon_a.size();
    std::size_t b_size = polygon_b.size();
    std::size_t index_a = 0, index_b = 0;
    while (index_a < a_size || index_b < b_size)
    {
      minkovsky_sum.emplace_back(polygon_a[index_a % a_size] +
                                 polygon_b[index_b % b_size]);
      int angle_comparation = CheckForClockwise(
          polygon_a[(index_a + 1) % a_size] - polygon_a[index_a % a_size],
          polygon_b[(index_b + 1) % b_size] - polygon_b[index_b % b_size]
       );
      if (angle_comparation == 0)
      {
        ++index_a;
        ++index_b;
      }
      else if (angle_comparation == 1)
      {
        ++index_b;
      }
      else
      {
        ++index_a;
      }
    }
  }

  bool CheckForIntersect (CPolygon2D &polygon_a, CPolygon2D &polygon_b)
  {
    __reflectPolygon(polygon_b);
    std::reverse(polygon_a.begin(), polygon_a.end());
    std::reverse(polygon_b.begin(), polygon_b.end());
    __shiftPolygon(polygon_a);
    __shiftPolygon(polygon_b);
    CPolygon2D minkovsky_sum;
    ComputeMinkovskySum(polygon_a, polygon_b, minkovsky_sum);
    return fabs(
        ComputeArea(minkovsky_sum, minkovsky_sum[0]) -
        ComputeArea(minkovsky_sum, CPoint2D(0, 0))
    ) < geometry2d::CPoint2D::GetEpsilon(); // zero belongs?
  }
};

using minkovsky_space::CheckForIntersect;


int main ()
{
  CPolygon2D polygon_a, polygon_b;
  std::cin >> polygon_a >> polygon_b;
  std::cout << (CheckForIntersect(polygon_a, polygon_b) ? "YES" : "NO") << "\n";
  return 0;
}

