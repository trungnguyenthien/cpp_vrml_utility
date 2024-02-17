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

void process(vector<Point> &points, vector<vector<int>> &faces, int numberCube, float sizeCube) {
  // printMinMaxPoint(points);
  auto minMaxPoints = getMinMaxPoint(points);
  auto minPoint = minMaxPoints.first;
  auto maxPoint = minMaxPoints.second;
  int count = 0;
  vector<Point> randInsidePoints;
  while (count < numberCube) {
    auto rz = randomFloat(minPoint.z, maxPoint.z);

    vector<vector<Point>> polys = polygonAtZ(points, faces, rz);
    auto minMaxPointAtZ = getMinMaxPoint(polys);

    auto rx = randomFloat(minMaxPointAtZ.first.x, minMaxPointAtZ.second.x);
    auto ry = randomFloat(minMaxPointAtZ.first.y, minMaxPointAtZ.second.y);
    Point randPoint(rx, ry, rz);

    // bool isInside = false;
    if (checkPointInSidePolygons(randPoint, polys)) {
      cout << "TRY POINT:" << randPoint.toString() << "\t\t\t\t\t";
      // isInside = true;
      count++;
      randInsidePoints.push_back(randPoint);
      cout << "✅" << count << endl;
    }
  }
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
    process(faceSet->points, faceSet->faces, numberCube, sizeCube);
  }

  outputFile.close();
  return 1;
}