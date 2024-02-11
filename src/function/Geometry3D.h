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
vector<vector<Point>> polygonAtZ(vector<Point> shapePoints, vector<vector<int>> faceSet, int z);

Point_3 convertToPoint_3(Point p);
Point convertFromPoint_3(Point_3 p3);

// Return TRUE nếu: Điểm X nằm trên đoạn thẳng AB
// Các trường hợp return FALSE: X không nằm trên đoạn AB, X trùng với A hoặc B
bool isPointsInSegment(Point x, Point a, Point b);

// Trả về danh sách Points sau khi {sourcePoints} đã excude các point trong {removePoints}
vector<Point> excludePoints(vector<Point> sourcePoints, vector<Point> removePoints);

vector<Point> findPointsInSegment(vector<Point> segmentPoints);

// {connections} là bộ dữ liệu các đỉnh tạo thành 1 hay nhiều polygon (diểm đầu và cuối trùng nhau)
// Kết quả cần tìm của hàm này là bộ dữ liệu các đỉnh của các polygon.
// Lưu ý:
// - có thể có nhiều polygon.
// - connection mỗi polygon được chứa trong vector<Point>
vector<vector<Point>> findPolygonPath(vector<pair<Point, Point>> connections);

#endif  // FUNCTION_GEOMETRY3D_H_