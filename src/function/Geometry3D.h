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
#include <utility>
#include <vector>

#include "../common/Model.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Partition_traits_2<Kernel> Traits;
// typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
// typedef Polyhedron::Halfedge_around_facet_circulator Halfedge_facet_circulator;
// typedef Kernel::Point_3 CGALPoint;
typedef Kernel::Segment_3 Segment_3;
typedef Kernel::Segment_2 Segment_2;
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
bool checkPointInSidePolygon(const Point &point, const vector<Point> &polygonPoints);
bool checkPointInSidePolygons(const Point &point, const vector<vector<Point>> &polygonPoints);
vector<vector<Point>> polygonAtZ(const vector<Point> &shapePoints,
                                 const vector<vector<int>> &faceSet, float z);

Point_3 convertToPoint_3(Point p);
Point convertFromPoint_3(Point_3 p3);

// {connections} là bộ dữ liệu các đỉnh tạo thành 1 hay nhiều polygon (diểm đầu và cuối trùng nhau)
// Kết quả cần tìm của hàm này là bộ dữ liệu các đỉnh của các polygon.
// Lưu ý:
// - có thể có nhiều polygon.
// - connection mỗi polygon được chứa trong vector<Point>
vector<vector<Point>> findPolygonPath(vector<pair<Point, Point>> connections);

// Return (minPoint, maxPoint) của các point trong {points}
// Trong đó minPoint(xMin, yMin, zMin), maxPoint(xMax, yMax, zMax)
// Cấu trúc Point(float x, float y, float z)
pair<Point, Point> getMinMaxPoint2(const vector<vector<Point>> &points);

// Return (minPoint, maxPoint) của các point trong {points}
// Trong đó minPoint(xMin, yMin, zMin), maxPoint(xMax, yMax, zMax)
// Cấu trúc Point(float x, float y, float z)
pair<Point, Point> getMinMaxPoint1(const vector<Point> &points);

// Tạo ngẫu nhiên Point(float x, float y, float z) trong khoảng min-max cho trước.
Point randomPoint(float xMin, float yMin, float zMin, float xMax, float yMax, float zMax);

void insertPoint(const Point p, vector<Point> &points, int afterIndex);

Point *intersectionPoint(const Point &p1, const Point &q1, const Point &p2, const Point &q2);

bool doIntersect(const Point &p1, const Point &q1, const Point &p2, const Point &q2);

bool isSimplePolygon(const vector<Point> &points);

float randomFloat(float min, float max);
#endif  // FUNCTION_GEOMETRY3D_H_