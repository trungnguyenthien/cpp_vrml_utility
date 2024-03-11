#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>

#include <CLI/CLI.hpp>
#include <algorithm>
#include <iomanip>
#include <iostream>

#include "common/Debug.h"
#include "common/Model.h"
#include "common/StringUtils.h"
#include "common/VRML.h"
// #include "function/G4DCM.h"
#include "function/Geometry3D.h"
#include "function/WRLHelper.h"

void process(vector<Point> &points, vector<vector<int>> &faces, int numberCube, float sizeCube,
             std::ofstream &outputFile) {
  // printMinMaxPoint(points);
  auto minMaxPoints = getMinMaxPoint1(points);
  auto minPoint = minMaxPoints.first;
  auto maxPoint = minMaxPoints.second;
  int count = 0;
  int failed = 0;
  vector<Point> randInsidePoints;
  ostringstream outputStream;
  // float rz = -6103.69;

  // vector<vector<Point>> polys = polygonAtZ(points, faces, rz);
  // printVectorVectorPoint(polys);
  // cout << "minpoint = " << minPoint.toString <<
  while (count < numberCube) {
    // -6103.69

    float rz = randomFloat(minPoint.z, maxPoint.z);

    // float rz = -6102.187;
    // cout << "RZ = " << rz << endl;
    vector<vector<Point>> polys = polygonAtZ(points, faces, rz);
    auto minMaxPointAtZ = getMinMaxPoint2(polys);

    while (true) {  // START WHILE
      auto rx = randomFloat(minMaxPointAtZ.first.x, minMaxPointAtZ.second.x);
      auto ry = randomFloat(minMaxPointAtZ.first.y, minMaxPointAtZ.second.y);
      Point randPoint(rx, ry, rz);
      // cout << "rx = " << rx << ", ry = " << ry << ", rz = " << rz << endl;
      cout << "RAND_Z " << rz << "\tRAND_X_Y ";
      cout << randPoint.toStringXY() << "\t";
      // bool isInside = false;
      if (checkPointInSidePolygons(randPoint, polys)) {
        // isInside = true;
        count++;
        randInsidePoints.push_back(randPoint);
        cout << "✅\t" << count << "/" << numberCube << endl;
        WrlShapeCubic cubic("-", Point(randPoint.x, randPoint.y, randPoint.z),
                            string("InnerPoint_") + intToString(count));
        cubic.size = Size3D(sizeCube);
        outputFile << cubic.print() << endl;
        break;
      } else {
        cout << "❌\tFail: " << failed++ << endl;
        // break;
      }
    }  // END WHILE
  }
  // appendAfterLine(wrlFile, logFile + ".wrl", "#End of file.", outputFile.str());
}

int main(int argc, char **argv) {
  std::cout << std::fixed
            << std::setprecision(3);  // Thiết lập độ chính xác là 3 chữ số phần thập phân

  std::srand(unsigned(std::time(0)));
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
  cout << "faceSet->points = " << faceSet->points.size() << endl;
  if (faceSet != NULL) {
    process(faceSet->points, faceSet->faces, numberCube, sizeCube, outputFile);
  }

  outputFile.close();
  return 1;
}