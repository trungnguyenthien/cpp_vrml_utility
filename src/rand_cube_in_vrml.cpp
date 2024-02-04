#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <cairo/cairo.h>

#include <CLI/CLI.hpp>

#include "common/Model.h"
#include "common/StringUtils.h"
#include "common/VRML.h"
#include "function/DrawBoard.h"
#include "function/G4DCM.h"
#include "function/Geometry3D.h"

void testGeo3D(vector<Point> &points, vector<vector<int>> &faces, int z) {
  cout << "Testing Geo3D " << points.size() << " faces " << faces.size() << endl;
  // void *shape = createShape3D(points, faces);
  vector<Point> poly = polygonAtZ(points, faces, z);

  DBBoard db;
  db.size = BOARD_SIZE::_1_000;
  db.zoom = ZOOM_RATIO::X10;

  DBShape *shapePoly = new DBShape();
  shapePoly->appendPoints(poly);
  shapePoly->color = Color::parseColorFromHex("#3366FF");
  shapePoly->fillColor = Color::parseColorFromHex("#E5E968");
  db.addObject(shapePoly);

  db.render();
}

int main(int argc, char **argv) {
  CLI::App app{"---"};
  std::string filename = "";
  app.add_option("-f,--file", filename, "Path to input file");

  float numberCube = 100;
  app.add_option("-n,--number", numberCube, "Số lượng cube");

  float sizeCube = 1;
  app.add_option("-s,--size", sizeCube, "Size cube");

  CLI11_PARSE(app, argc, argv);

  if (filename.empty()) {
    return 0;
  }
  string outputFilePath = filename + "_" + to_string(int(numberCube)) + "_OUTPUT.wrl";
  std::ofstream outputFile(outputFilePath, std::ios::out);
  if (!outputFile.is_open()) {
    std::cerr << "Can not write out to file:" << outputFilePath << std::endl;
    return 1;
  }

  vector<VrmlObject *> vrmlObjects = read_vrml_file(filename);

  VrmlFaceSet *faceSet = dynamic_cast<VrmlFaceSet *>(vrmlObjects[0]);

  if (faceSet != NULL) {
    testGeo3D(faceSet->points, faceSet->faces, -20);
  }

  outputFile.close();
  return 1;
}


// typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
// typedef Kernel::Point_3 Point_3;
// typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
// typedef Polyhedron::HalfedgeDS HalfedgeDS;

// // Lớp builder để xây dựng polyhedron từ danh sách điểm và mặt
// class PolyhedronBuilder : public CGAL::Modifier_base<HalfedgeDS> {
// public:
//   std::vector<Point> &points;
//   std::vector<std::vector<int>> &faces;

//   PolyhedronBuilder(std::vector<Point> &pts, std::vector<std::vector<int>> &fcs)
//       : points(pts), faces(fcs) {}

//   void operator()(HalfedgeDS &hds) {
//     CGAL::Polyhedron_incremental_builder_3<HalfedgeDS> builder(hds, true);
//     builder.begin_surface(points.size(), faces.size());

//     for (const auto &p : points) {
//       builder.add_vertex(Point_3(p.x, p.y, p.z));
//     }

//     for (const auto &face : faces) {
//       builder.begin_facet();
//       for (int idx : face) {
//         builder.add_vertex_to_facet(idx);
//       }
//       builder.end_facet();
//     }

//     builder.end_surface();
//   }
// };

// void *createShape3D(std::vector<Point> &points, std::vector<std::vector<int>> &faces) {
//   Polyhedron *P = new Polyhedron();
//   PolyhedronBuilder builder(points, faces);
//   P->delegate(builder);
//   return (void *)P;
// }


// std::vector<Point> polygonAtZ(void *shape, int z) {
//   std::vector<Point> points;
//   vector<Segment> segments;
//   Polyhedron *P = static_cast<Polyhedron *>(shape);
//   // Tạo mặt phẳng cắt tại tọa độ z
//   Plane_3 plane(0, 0, 1, -z);
//   for (Polyhedron::Facet_iterator fi = P->facets_begin(); fi != P->facets_end(); ++fi) {
//     Halfedge_facet_circulator hec = fi->facet_begin();
//     do {
//       auto sourcePoint = hec->vertex()->point();
//       auto targetPoint = hec->next()->vertex()->point();
//       Segment mySegment(sourcePoint.x(), sourcePoint.y(), sourcePoint.z(), targetPoint.x(),
//                         targetPoint.y(), targetPoint.z());
//       Point pA(sourcePoint.x(), sourcePoint.y(), sourcePoint.z()),
//           pB(targetPoint.x(), targetPoint.y(), targetPoint.z());
//       // if (hasExistedSegment(segments, pA, pB)) {
//       //   continue;
//       // }
//       segments.push_back(mySegment);
//       Segment_3 segment(sourcePoint, targetPoint);
//       CGAL::Object result = CGAL::intersection(segment, plane);

//       if (const Point_3 *p = CGAL::object_cast<Point_3>(&result)) {
//         float px = static_cast<float>(p->x());
//         float py = static_cast<float>(p->y());
//         float pz = static_cast<float>(p->z());
//         // if (hasExistedPoint(points, px, py, pz)) {
//         //   cout << "hasExistedPoint << " << px << "," << py << "," << pz << endl;
//         //   continue;
//         // }

//         cout << "Cut Point << " << px << "," << py << "," << pz << endl;
//         points.push_back({px, py, pz});
//       }
//     } while (++hec != fi->facet_begin());
//   }
//   printVectorPoints(points);
//   return points;
// }
