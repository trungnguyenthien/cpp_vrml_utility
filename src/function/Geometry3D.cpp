#include "Geometry3D.h"

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>

#include <iostream>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 CGALPoint;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
typedef Polyhedron::HalfedgeDS HalfedgeDS;

// Lớp builder để xây dựng polyhedron từ danh sách điểm và mặt
class PolyhedronBuilder : public CGAL::Modifier_base<HalfedgeDS> {
public:
  std::vector<Point> &points;
  std::vector<std::vector<int>> &faces;

  PolyhedronBuilder(std::vector<Point> &pts, std::vector<std::vector<int>> &fcs)
      : points(pts), faces(fcs) {}

  void operator()(HalfedgeDS &hds) {
    CGAL::Polyhedron_incremental_builder_3<HalfedgeDS> builder(hds, true);
    builder.begin_surface(points.size(), faces.size());

    for (const auto &p : points) {
      builder.add_vertex(CGALPoint(p.x, p.y, p.z));
    }

    for (const auto &face : faces) {
      builder.begin_facet();
      for (int idx : face) {
        builder.add_vertex_to_facet(idx);
      }
      builder.end_facet();
    }

    builder.end_surface();
  }
};

void *createShape3D(std::vector<Point> &points, std::vector<std::vector<int>> &faces) {
  Polyhedron *P = new Polyhedron();
  PolyhedronBuilder builder(points, faces);
  P->delegate(builder);
  return (void *)P;
}

// Hàm để giải phóng bộ nhớ của polyhedron
void deleteShape3D(void *shape) { delete static_cast<Polyhedron *>(shape); }

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
typedef Polyhedron::Halfedge_around_facet_circulator Halfedge_facet_circulator;
typedef Kernel::Point_3 CGALPoint;
typedef Kernel::Segment_3 Segment_3;
typedef Kernel::Plane_3 Plane_3;

std::vector<Point> polygonAtZ(void *shape, int z) {
  std::vector<Point> points;
  cout << "polygonAtZ 1" << endl;
  Polyhedron *P = static_cast<Polyhedron *>(shape);
  cout << "polygonAtZ 2" << endl;
  // Tạo mặt phẳng cắt tại tọa độ z
  Plane_3 plane(0, 0, 1, -z);
  cout << "polygonAtZ 3 " << endl;
  for (Polyhedron::Facet_iterator fi = P->facets_begin(); fi != P->facets_end(); ++fi) {
    cout << "polygonAtZ 3.1 " << endl;
    Halfedge_facet_circulator hec = fi->facet_begin();
    do {
      cout << "polygonAtZ 3.2 " << endl;
      Segment_3 segment(hec->vertex()->point(), hec->next()->vertex()->point());
      CGAL::Object result = CGAL::intersection(segment, plane);

      if (const CGALPoint *p = CGAL::object_cast<CGALPoint>(&result)) {
        cout << "polygonAtZ 3.3 " << endl;
        points.push_back(
            {static_cast<float>(p->x()), static_cast<float>(p->y()), static_cast<float>(p->z())});
      }
    } while (++hec != fi->facet_begin());
  }
  cout << "polygonAtZ 4 " << points.size() << endl;
  printVectorPoints(points);
  return points;
}
