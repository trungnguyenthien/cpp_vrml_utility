#ifndef FUNCTION_GEOMETRY3D_H_
#define FUNCTION_GEOMETRY3D_H_
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Line_3.h>
#include <CGAL/Plane_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Triangle_3.h>
#include <CGAL/intersections.h>

#include <boost/optional.hpp>
#include <vector>

#include "../common/Model.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
// typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
// typedef Polyhedron::Halfedge_around_facet_circulator Halfedge_facet_circulator;
// typedef Kernel::Point_3 CGALPoint;
typedef Kernel::Segment_3 Segment_3;
typedef Kernel::Plane_3 Plane_3;
typedef Kernel::Point_3 Point_3;
typedef CGAL::Object Object;
// typedef CGAL::Line_3 Line_3;
typedef CGAL::Triangle_3<Kernel> Triangle_3;

using namespace std;
vector<Point> polygonAtZ(vector<Point> shapePoints, vector<vector<int>> faceSet, int z);

Point_3 convertToPoint_3(Point p);
Point convertFromPoint_3(Point_3 p3);
#endif  // FUNCTION_GEOMETRY3D_H_