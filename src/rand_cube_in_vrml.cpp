#include <CLI/CLI.hpp>

#include "common/StringUtils.h"
#include "common/VRML.h"
#include "function/G4DCM.h"

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

  outputFile.close();
  return 1;
}