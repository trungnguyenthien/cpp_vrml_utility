#ifndef FUNCTION_GEOMETRY3D_H_
#define FUNCTION_GEOMETRY3D_H_
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Line_3.h>
#include <CGAL/Partition_traits_2.h>
#include <CGAL/Plane_3.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Triangle_3.h>
#include <CGAL/intersections.h>
#include <CGAL/partition_2.h>

#include <boost/optional.hpp>
#include <vector>

#include "../common/Model.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Partition_traits_2<Kernel> Traits;
// typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
// typedef Polyhedron::Halfedge_around_facet_circulator Halfedge_facet_circulator;
// typedef Kernel::Point_3 CGALPoint;
typedef Kernel::Segment_3 Segment_3;
typedef Kernel::Plane_3 Plane_3;
typedef Kernel::Point_3 Point_3;
typedef CGAL::Object Object;
// typedef CGAL::Line_3 Line_3;
typedef CGAL::Triangle_3<Kernel> Triangle_3;
typedef Kernel::Vector_3 Vector_3;
typedef CGAL::Polygon_2<Kernel> Polygon_2;
typedef Kernel::Point_2 Point_2;
typedef Polygon_2::Vertex_iterator Vertex_iterator;

typedef Traits::Point_2 Trait_Point_2;
typedef Traits::Polygon_2 Trait_Polygon_2;

using namespace std;
set<vector<Point>> polygonAtZ(vector<Point> shapePoints, vector<vector<int>> faceSet, int z);

Point_3 convertToPoint_3(Point p);
Point convertFromPoint_3(Point_3 p3);

// Cấu trúc Point(float x, float y, float z)

// Return deltaPoint là sai số chấp nhận được xác định từ mảng {points}
// deltaPoint.x = 0.001 * (xMax - xMin)
// deltaPoint.y = 0.001 * (yMax - yMin)
// deltaPoint.z = 0.001 * (zMax - zMin)
Point findDeltaPoint(const vector<Point> &points);

// Mỗi group (vector<int>) là một group các index các Point gần nhau (các point này cách nhau một
// khoảng nhỏ hơn {deltaPoint}) Trả về tất cả group
vector<vector<int>> findGroupClosetIndices(const vector<Point> &points, const Point &deltaPoint);

// Mỗi point trong {points} đều thuộc một group con trong {groupClosetIndices}
// Update lại tất cả các point trong vector {points} sao cho:
// - toạ độ point mới là toạ độ trung bình trong mỗi group con của chính nó.
void mergeClosetPoints(vector<Point> &points, const vector<vector<int>> &groupClosetIndices);

// Return TRUE nếu: Điểm X nằm trên đoạn thẳng AB
// Các trường hợp return FALSE: X không nằm trên đoạn AB, X trùng với A hoặc B
bool isPointsInSegment(Point x, Point a, Point b);

// Trả về danh sách Points sau khi {sourcePoints} đã excude các point trong {removePoints}
vector<Point> excludePoints(vector<Point> sourcePoints, vector<Point> removePoints);

vector<Point> findPointsInSegment(vector<Point> segmentPoints);

#endif  // FUNCTION_GEOMETRY3D_H_