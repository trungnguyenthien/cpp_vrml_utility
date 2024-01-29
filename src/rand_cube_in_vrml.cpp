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
  void *shape = createShape3D(points, faces);
  vector<Point> poly = polygonAtZ(shape, z);
  printVectorPoints(poly);
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

  cout << "filename:" << filename << endl;
  cout << "number:" << numberCube << endl;
  cout << "size:" << sizeCube << endl;

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
    testGeo3D(faceSet->points, faceSet->faces, 10);
  }

  outputFile.close();
  return 1;
}