#include <cairo/cairo.h>

#include <CLI/CLI.hpp>

#include "common/Model.h"
#include "common/StringUtils.h"
#include "common/VRML.h"
#include "function/DrawBoard.h"
#include "function/G4DCM.h"

void test_cairo() {
  DBBoard db;
  db.size = BOARD_SIZE::_5_000;
  db.zoom = ZOOM_RATIO::X10;
  DBPoint point;
  point.x = 10;
  point.y = 30;
  point.color = Color::parseColorFromHex("#FF4433");
  db.addObject(point);

  DBShape shape;
  shape.xyList = {20, 30, 40, 20, 50, 50};
  shape.color = Color::parseColorFromHex("#3366FF");
  shape.fillColor = Color::parseColorFromHex("#E5E968");
  db.addObject(shape);

  db.render();
}

int main(int argc, char **argv) {
  test_cairo();

  // CLI::App app{"---"};
  // std::string filename = "";
  // app.add_option("-f,--file", filename, "Path to input file");

  // float numberCube = 100;
  // app.add_option("-n,--number", numberCube, "Số lượng cube");

  // float sizeCube = 1;
  // app.add_option("-s,--size", sizeCube, "Size cube");

  // CLI11_PARSE(app, argc, argv);

  // cout << "filename:" << filename << endl;
  // cout << "number:" << numberCube << endl;
  // cout << "size:" << sizeCube << endl;

  // if (filename.empty()) {
  //   return 0;
  // }
  // string outputFilePath = filename + "_" + to_string(int(numberCube)) + "_OUTPUT.wrl";
  // std::ofstream outputFile(outputFilePath, std::ios::out);
  // if (!outputFile.is_open()) {
  //   std::cerr << "Can not write out to file:" << outputFilePath << std::endl;
  //   return 1;
  // }

  // vector<VrmlObject *> vrmlObjects = read_vrml_file(filename);

  // outputFile.close();
  return 1;
}