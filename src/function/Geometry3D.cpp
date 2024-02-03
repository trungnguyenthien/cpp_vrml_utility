#include "Geometry3D.h"

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>

#include <iostream>
#include <vector>

#include "../common/Debug.h"

// Trả về true nếu trong segment có đoạn AB hoặc BA
bool hasExistedSegment(vector<Segment> &segments, Point &pA, Point &pB) {
  for (const Segment &segment : segments) {
    if ((segment.p1 == pA && segment.p2 == pB) || (segment.p1 == pB && segment.p2 == pA)) {
      return true;
    }
  }
  return false;
}

// Trả về true nếu {points} có chứa point tạo bỏi px, py, pz
bool hasExistedPoint(const std::vector<Point> &points, float px, float py, float pz) {
  for (const Point &point : points) {
    if (std::fabs(point.x - px) < 1e-6 && std::fabs(point.y - py) < 1e-6 &&
        std::fabs(point.z - pz) < 1e-6) {
      return true;
    }
  }
  return false;
}

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
  vector<Segment> segments;
  Polyhedron *P = static_cast<Polyhedron *>(shape);
  // Tạo mặt phẳng cắt tại tọa độ z
  Plane_3 plane(0, 0, 1, -z);
  for (Polyhedron::Facet_iterator fi = P->facets_begin(); fi != P->facets_end(); ++fi) {
    Halfedge_facet_circulator hec = fi->facet_begin();
    do {
      auto sourcePoint = hec->vertex()->point();
      auto targetPoint = hec->next()->vertex()->point();
      Segment mySegment(sourcePoint.x(), sourcePoint.y(), sourcePoint.z(), targetPoint.x(),
                        targetPoint.y(), targetPoint.z());
      Point pA(sourcePoint.x(), sourcePoint.y(), sourcePoint.z()),
          pB(targetPoint.x(), targetPoint.y(), targetPoint.z());
      // if (hasExistedSegment(segments, pA, pB)) {
      //   continue;
      // }
      segments.push_back(mySegment);
      Segment_3 segment(sourcePoint, targetPoint);
      CGAL::Object result = CGAL::intersection(segment, plane);

      if (const CGALPoint *p = CGAL::object_cast<CGALPoint>(&result)) {
        float px = static_cast<float>(p->x());
        float py = static_cast<float>(p->y());
        float pz = static_cast<float>(p->z());
        if (hasExistedPoint(points, px, py, pz)) {
          cout << "hasExistedPoint << " << px << "," << py << "," << pz << endl;
          continue;
        }

        cout << "Cut Point << " << px << "," << py << "," << pz << endl;
        points.push_back({px, py, pz});
      }
    } while (++hec != fi->facet_begin());
  }
  printVectorPoints(points);
  return points;
}
