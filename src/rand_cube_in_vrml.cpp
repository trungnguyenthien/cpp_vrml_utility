#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <cairo/cairo.h>

#include <CLI/CLI.hpp>

#include "common/Debug.h"
#include "common/Model.h"
#include "common/StringUtils.h"
#include "common/VRML.h"
#include "function/DrawBoard.h"
#include "function/G4DCM.h"
#include "function/Geometry3D.h"

void testGeo3D(vector<Point> &points, vector<vector<int>> &faces, int z) {
  // cout << "Testing Geo3D " << points.size() << " faces " << faces.size() << endl;
  // void *shape = createShape3D(points, faces);
  printMinMaxPoint(points);
  vector<Point> poly = *(polygonAtZ(points, faces, z).begin());
  printVectorPoints(poly);
  DBBoard db;
  db.size = BOARD_SIZE::_5_000;
  db.zoom = ZOOM_RATIO::X1;

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
    testGeo3D(faceSet->points, faceSet->faces, -6050);
  }

  outputFile.close();
  return 1;
}