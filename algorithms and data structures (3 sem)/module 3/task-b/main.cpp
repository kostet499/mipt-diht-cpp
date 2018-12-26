#include <algorithm>        // std::for_each, std::sort
#include <cmath>            // abs
#include <initializer_list> // std::initializer_list
#include <iostream>         // std::istream, std::ostream, std::cin, std::cout
#include <iterator>         // std::iterator, std::reverse_iterator , std::const_iterator
#include <list>             // std::list
#include <stdexcept>        // std::invalid_argument, std::logic_error
#include <vector>           // std::vector
#include <unordered_set>    // std::unordered_set


namespace basic_geometry_3d
{
  class CPoint3D
  {
  public:
    explicit CPoint3D () = default;

    explicit CPoint3D (double_t x, double_t y, double_t z) :
        x_axis(x), y_axis(y), z_axis(z)
    {}

    CPoint3D (const std::initializer_list<double_t> &point)
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

    CPoint3D (CPoint3D &&point) noexcept :
        x_axis(point.X()), y_axis(point.Y()), z_axis(point.Z())
    {}

    bool operator== (const CPoint3D &point) const
    {
      return x_axis == point.x_axis &&
             y_axis == point.y_axis &&
             z_axis == point.z_axis;
    }

    double_t X () const
    {
      return x_axis;
    }

    double_t Y () const
    {
      return y_axis;
    }

    double_t Z () const
    {
      return z_axis;
    }

    ~CPoint3D () = default;

    friend std::istream &operator>> (std::istream &is, CPoint3D &point3d);

    friend std::ostream &operator<< (std::ostream &os, const CPoint3D &point3d);

  protected:
    double_t x_axis = 0, y_axis = 0, z_axis = 0;
  };

  std::istream &operator>> (std::istream &is, CPoint3D &point3d)
  {
    is >> point3d.x_axis >> point3d.y_axis >> point3d.z_axis;
    return is;
  }

  std::ostream &operator<< (std::ostream &os, const CPoint3D &point3d)
  {
    os << point3d.x_axis << " " << point3d.y_axis << " " << point3d.z_axis;
    return os;
  }


  struct CVector3D : public CPoint3D
  {
    explicit CVector3D () = default;

    CVector3D (double_t x, double_t y, double_t z) :
        CPoint3D(x, y, z)
    {}

    CVector3D (const std::initializer_list<double_t> &vector3d) :
        CPoint3D(vector3d)
    {}

    explicit CVector3D (const CPoint3D &end) :
        CVector3D::CPoint3D(end)
    {}

    explicit CVector3D (CPoint3D &&end) noexcept:
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

    CVector3D &operator*= (double_t scale)
    {
      x_axis *= scale;
      y_axis *= scale;
      z_axis *= scale;
      return *this;
    }

    CVector3D operator- () const
    {
      CVector3D antivector3d(*this);
      antivector3d *= -1;
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

    CVector3D ComputeCross (const CVector3D &vector3d) const
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

  CVector3D operator* (double_t scale, const CVector3D &rhs)
  {
    CVector3D new_vector(rhs);
    new_vector *= scale;
    return new_vector;
  }

  CVector3D operator* (const CVector3D &lhs, double_t scale)
  {
    CVector3D new_vector(lhs);
    new_vector *= scale;
    return new_vector;
  }
};

enum Order
{
  LEXICOGRAPHICALLY,
  LEXICOGRAPHICALLY_REVERSED,
  NON_ORDER
};

using basic_geometry_3d::CVector3D;

namespace convex_hull_supporting_structures
{
  struct CVertex3D : public CVector3D
  {
    CVertex3D () = default;

    explicit CVertex3D (const CVector3D &vector3d) :
        CVector3D(vector3d), order_id(CVertex3D::vertex_cnt)
    {}

    std::size_t order_id = 0;
    static std::size_t vertex_cnt;
  };

  std::size_t CVertex3D::vertex_cnt = 0;


  struct CEdge3D
  {
    CVertex3D vertex_a{};
    CVertex3D vertex_b{};

    CEdge3D () = default;

    CEdge3D (const CVertex3D &vertex3d_a, const CVertex3D &vertex3d_b);

    static const std::size_t HASH_MULTIPLIER = 199999;
    static const std::size_t HASH_MODULO = 1000000007;
  };


  inline bool operator== (const CEdge3D &edge1, const CEdge3D &edge2);


  struct CEdge3DHash
  {
    std::size_t operator() (const CEdge3D &edge) const;
  };


  struct CFace3D
  {
    CVertex3D vertex_a{};
    CVertex3D vertex_b{};
    CVertex3D vertex_c{};
    CVector3D outter_normal{};

    CFace3D (const CVertex3D &vertex_a,
             const CVertex3D &vertex_b,
             const CVertex3D &vertex_c);

    std::tuple<CEdge3D, CEdge3D, CEdge3D> Split () const;

    friend std::ostream &operator<< (std::ostream &os, const CFace3D &face)
    {
      os << face.vertex_a.order_id << " "
         << face.vertex_b.order_id << " "
         << face.vertex_c.order_id;
      return os;
    }
  };

  bool checkForVisibility (const CFace3D &face, const CVector3D &vector);
};

using convex_hull_supporting_structures::CVertex3D;
using convex_hull_supporting_structures::CEdge3D;
using convex_hull_supporting_structures::CEdge3DHash;
using convex_hull_supporting_structures::CFace3D;

using convex_hull_supporting_structures::checkForVisibility;


class CConvexHull3D
{
public:

  explicit CConvexHull3D () = default;

  explicit CConvexHull3D (Order init_order) :
      new_order(init_order)
  {}

  CConvexHull3D (const std::vector<CVector3D> &points, Order init_order);

  ~CConvexHull3D ();

  const CFace3D &operator[] (std::size_t index);

  std::vector<CFace3D>::iterator begin ();

  std::vector<CFace3D>::iterator end ();

  std::vector<CFace3D>::reverse_iterator rbegin ();

  std::vector<CFace3D>::reverse_iterator rend ();

  std::vector<CFace3D>::const_iterator begin () const;

  std::vector<CFace3D>::const_iterator end () const;

  std::size_t size () const;

  void SetOrder (Order order);

  friend std::istream &operator>> (std::istream &is, CConvexHull3D &hull);

  friend std::ostream &operator<< (std::ostream &os, const CConvexHull3D &hull);

private:

  void addPointToConvexHull (std::size_t point_id);

  void orientFacesOutterNormal (CFace3D &face);

  void build (const std::vector<CVector3D> &points);

  void reorder (Order order);

private:

  Order order = Order::NON_ORDER;
  Order new_order = Order::NON_ORDER;
  std::vector<CFace3D> faces;
  std::vector<CVertex3D> vertices;
  CVector3D inner_point{};
};


void HandleTest (std::istream &is, std::ostream &os);

int main ()
{
  uint16_t tests_amt;
  std::cin >> tests_amt;
  for (uint16_t test_num = 0; test_num < tests_amt; ++test_num)
  {
    HandleTest(std::cin, std::cout);
  }
  return 0;
}

void HandleTest (std::istream &is, std::ostream &os)
{
  CConvexHull3D convexhull(Order::LEXICOGRAPHICALLY);
  std::cin >> convexhull;
  std::cout << convexhull;
}


std::istream &operator>> (std::istream &is, CConvexHull3D &hull)
{
  std::size_t polygon_size;
  is >> polygon_size;
  std::vector<CVector3D> polygon(polygon_size);
  for (auto &vector3d: polygon)
  {
    is >> vector3d;
  }
  hull.build(polygon);
  return is;
}

std::ostream &operator<< (std::ostream &os, const CConvexHull3D &hull)
{
  os << hull.size() << std::endl;
  for (const auto &face: hull)
  {
    os << 3 << " " << face << std::endl;
  }
  return os;
}

// ------------------ CConvexHull3D ------------------ //
// ---------------------- BEGIN ---------------------- //
CConvexHull3D::CConvexHull3D (const std::vector<CVector3D> &points, Order init_order) :
    new_order(init_order)
{
  build(points);
}

CConvexHull3D::~CConvexHull3D ()
{
  CVertex3D::vertex_cnt = 0;
}

const CFace3D &CConvexHull3D::operator[] (std::size_t index)
{
  if (index >= size())
  {
    throw std::out_of_range("Index out of range");
  }
  return faces[index];
}

std::vector<CFace3D>::iterator CConvexHull3D::begin ()
{
  return faces.begin();
}

std::vector<CFace3D>::iterator CConvexHull3D::end ()
{
  return faces.end();
}

std::vector<CFace3D>::reverse_iterator CConvexHull3D::rbegin ()
{
  return faces.rbegin();
}

std::vector<CFace3D>::reverse_iterator CConvexHull3D::rend ()
{
  return faces.rend();
}

std::vector<CFace3D>::const_iterator CConvexHull3D::begin () const
{
  return faces.begin();
}

std::vector<CFace3D>::const_iterator CConvexHull3D::end () const
{
  return faces.end();
}

std::size_t CConvexHull3D::size () const
{
  return faces.size();
}

void CConvexHull3D::SetOrder (Order order)
{
  if (order == this->order || order == Order::NON_ORDER)
  {
    return;
  }
  switch (order)
  {
    case Order::LEXICOGRAPHICALLY:
      if (this->order == Order::LEXICOGRAPHICALLY_REVERSED)
      {
        std::reverse(begin(), end());
      }
      else
      {
        reorder(order);
      }
      break;
    case Order::LEXICOGRAPHICALLY_REVERSED:
      if (this->order == Order::LEXICOGRAPHICALLY)
      {
        std::reverse(begin(), end());
      }
      else
      {
        reorder(order);
      }
      break;
    default:
      break;
  }
  this->order = order;
}

void CConvexHull3D::addPointToConvexHull (std::size_t point_id)
{
  std::unordered_set<CEdge3D, CEdge3DHash> edges;
  std::list<CFace3D> new_faces(faces.begin(), faces.end());
  faces.clear();
  for (auto face_iter = new_faces.begin(); face_iter != new_faces.end(); ++face_iter)
  {
    CFace3D face = *face_iter;
    if (checkForVisibility(face, vertices[point_id]))
    {
      face_iter = --new_faces.erase(face_iter);
      CEdge3D edge1, edge2, edge3;
      std::tie(edge1, edge2, edge3) = face.Split();
      std::pair<CEdge3D, std::unordered_set<CEdge3D, CEdge3DHash>::iterator>
          edges_iters[]{
          std::make_pair(edge1, edges.find(edge1)),
          std::make_pair(edge2, edges.find(edge2)),
          std::make_pair(edge3, edges.find(edge3)),
      };
      for (auto &edge_iter: edges_iters)
      {
        if (edge_iter.second == edges.end())
        {
          edges.insert(edge_iter.first);
        }
        else
        {
          edges.erase(edge_iter.second);
        }
      }
    }
  }
  for (const auto &edge: edges)
  {
    CFace3D face(vertices[point_id], edge.vertex_a, edge.vertex_b);
    orientFacesOutterNormal(face);
    new_faces.emplace_back(face);
  }
  for (const auto &face: new_faces)
  {
    faces.push_back(face);
  }
}

void CConvexHull3D::orientFacesOutterNormal (CFace3D &face)
{
  CVector3D vector_diff_ba(face.vertex_b - face.vertex_a);
  CVector3D vector_diff_ca(face.vertex_c - face.vertex_a);
  face.outter_normal = vector_diff_ba.ComputeCross(vector_diff_ca);
  if (face.outter_normal.ComputeDot(inner_point - face.vertex_a) > 0)
  {
    face.outter_normal *= -1;
  }
}

void CConvexHull3D::build (const std::vector<CVector3D> &points)
{
  if (points.size() < 4)
  {
    throw std::logic_error("At least 4 points are necessary");
  }
  for (std::size_t tetrahedron_vertex_num = 0;
       tetrahedron_vertex_num < 4;
       ++tetrahedron_vertex_num)
  {
    vertices.emplace_back(points[tetrahedron_vertex_num]);
    ++CVertex3D::vertex_cnt;
    inner_point += points[tetrahedron_vertex_num];
  }
  inner_point *= 0.25;
  faces.emplace_back(vertices[0], vertices[1], vertices[2]);
  faces.emplace_back(vertices[0], vertices[1], vertices[3]);
  faces.emplace_back(vertices[0], vertices[2], vertices[3]);
  faces.emplace_back(vertices[1], vertices[2], vertices[3]);
  for (std::size_t face_num = 0; face_num < 4; ++face_num)
  {
    orientFacesOutterNormal(faces[face_num]);
  }
  for (std::size_t point_id = 4; point_id < points.size(); ++point_id)
  {
    vertices.emplace_back(points[point_id]);
    ++CVertex3D::vertex_cnt;
    addPointToConvexHull(point_id);
  }
  SetOrder(new_order);
}

void CConvexHull3D::reorder (Order order)
{
  std::for_each(begin(), end(), [=] (CFace3D &face)
  {
    if (face.vertex_a.order_id > face.vertex_b.order_id)
    {
      std::swap(face.vertex_a, face.vertex_b);
    }
    if (face.vertex_a.order_id > face.vertex_c.order_id)
    {
      std::swap(face.vertex_a, face.vertex_c);
    }
    CVector3D vector_diff_ba(face.vertex_b - face.vertex_a);
    CVector3D vector_diff_ca(face.vertex_c - face.vertex_a);
    CVector3D face_normal(vector_diff_ba.ComputeCross(vector_diff_ca));
    double scalar_mult_val = face.outter_normal.ComputeDot(face_normal);
    if (scalar_mult_val < 0)
    {
      std::swap(face.vertex_b, face.vertex_c);
    }
  });
  auto face_comparator = [] (const CFace3D &face1, const CFace3D &face2)
  {
    if (face1.vertex_a.order_id == face2.vertex_a.order_id)
    {
      if (face1.vertex_b.order_id == face2.vertex_b.order_id)
      {
        return face1.vertex_c.order_id < face2.vertex_c.order_id;
      }
      return face1.vertex_b.order_id < face2.vertex_b.order_id;
    }
    return face1.vertex_a.order_id < face2.vertex_a.order_id;
  };
  switch (order)
  {
    case Order::LEXICOGRAPHICALLY:
      std::sort(begin(), end(), face_comparator);
      break;
    case Order::LEXICOGRAPHICALLY_REVERSED:
      std::sort(rbegin(), rend(), face_comparator);
      break;
    default:
      break;
  }
}
// ----------------------- END ----------------------- //
// ------------------ CConvexHull3D ------------------ //


// --------------------- CFace3D --------------------- //
// ---------------------- BEGIN ---------------------- //
CFace3D::CFace3D (const CVertex3D &vertex_a,
                  const CVertex3D &vertex_b,
                  const CVertex3D &vertex_c)
{
  this->vertex_a = vertex_a;
  this->vertex_b = vertex_b;
  this->vertex_c = vertex_c;
}

std::tuple<CEdge3D, CEdge3D, CEdge3D> CFace3D::Split () const
{
  return std::make_tuple(
      CEdge3D(vertex_a, vertex_b),
      CEdge3D(vertex_a, vertex_c),
      CEdge3D(vertex_b, vertex_c)
  );
}

bool convex_hull_supporting_structures::
checkForVisibility (const CFace3D &face, const CVector3D &vector)
{
  return face.outter_normal.ComputeDot(vector - face.vertex_a) > 0;
}
// ----------------------- END ----------------------- //
// --------------------- CFace3D --------------------- //


// --------------------- CEdge3D --------------------- //
// ---------------------- BEGIN ---------------------- //
CEdge3D::CEdge3D (const CVertex3D &vertex3d_a, const CVertex3D &vertex3d_b) :
    vertex_a(vertex3d_a), vertex_b(vertex3d_b)
{
  if (vertex_a.order_id > vertex3d_b.order_id)
  {
    std::swap(vertex_a, vertex_b);
  }
}
// ----------------------- END ----------------------- //
// --------------------- CEdge3D --------------------- //


// ------------------- CEdgeHash3D ------------------- //
// ---------------------- BEGIN ---------------------- //
std::size_t CEdge3DHash::operator() (const CEdge3D &edge) const
{
  return (edge.vertex_a.order_id * CEdge3D::HASH_MULTIPLIER
          + edge.vertex_b.order_id) % CEdge3D::HASH_MODULO;
}
// ----------------------- END ----------------------- //
// ------------------- CEdgeHash3D ------------------- //

inline bool convex_hull_supporting_structures::
operator== (const CEdge3D &edge1, const CEdge3D &edge2)
{
  return (edge1.vertex_a.order_id == edge2.vertex_a.order_id
          && edge1.vertex_b.order_id == edge2.vertex_b.order_id) ||
         (edge1.vertex_a.order_id == edge2.vertex_b.order_id
          && edge1.vertex_b.order_id == edge2.vertex_a.order_id);
}

