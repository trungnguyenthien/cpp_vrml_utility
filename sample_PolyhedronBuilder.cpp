#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>

#include <iostream>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
typedef Polyhedron::HalfedgeDS HalfedgeDS;
typedef Kernel::Point_3 Point_3;

// Cấu trúc để định nghĩa các điểm và mặt của polyhedron
struct Point {
  float x, y, z;
};

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

    // Thêm các điểm vào polyhedron
    for (auto &pt : points) {
      builder.add_vertex(Point_3(pt.x, pt.y, pt.z));
    }

    // Thêm các mặt vào polyhedron
    for (auto &face : faces) {
      builder.begin_facet();
      for (int idx : face) {
        builder.add_vertex_to_facet(idx);
      }
      builder.end_facet();
    }

    builder.end_surface();
  }
};

int main() {
  std::vector<Point> points = {{0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0},
                               {0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1}};
  std::vector<std::vector<int>> faces = {{0, 1, 2, 3}, {7, 6, 5, 4}, {0, 4, 5, 1},
                                         {1, 5, 6, 2}, {2, 6, 7, 3}, {3, 7, 4, 0}};

  Polyhedron P;
  PolyhedronBuilder builder(points, faces);
  P.delegate(builder);

  // Kiểm tra và in thông tin polyhedron
  std::cout << "Polyhedron có " << P.size_of_vertices() << " đỉnh." << std::endl;
  std::cout << "Polyhedron có " << P.size_of_facets() << " mặt." << std::endl;

  return 0;
}
