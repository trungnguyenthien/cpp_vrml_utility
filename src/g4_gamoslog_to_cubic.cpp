#include <CLI/CLI.hpp>
#include <sstream>

#include "common/FileUtils.h"
#include "common/Model.h"
#include "common/StringUtils.h"
#include "function/Gamos_Log.h"
#include "function/WRLHelper.h"

int main(int argc, char **argv) {
  CLI::App app{"---"};
  std::string logFile = "";
  app.add_option("-l,--log", logFile, "Path to input log file");

  std::string wrlFile = "";
  app.add_option("-w,--wrl", wrlFile, "Path to output file");

  CLI11_PARSE(app, argc, argv);
  if (logFile.empty()) {
    return 0;
  }

  // std::ofstream outputFile(wrlFile, std::ios::out);
  // if (!outputFile.is_open()) {
  //   std::cerr << "Can not write out to file:" << wrlFile << std::endl;
  //   return 1;
  // }
  ostringstream outputStream;
  readG4TrackTables(logFile, [&outputStream](G4TrackRow *row) {
    // Print row
    // row->printDebugInfo();
    WrlShapeCubic cubic(row->ProcName, Point(row->Xmm, row->Ymm, row->Zmm), row->name());
    outputStream << cubic.print() << endl;
    // outputFile << cubic.print() << endl;
  });
  appendAfterLine(wrlFile, logFile + ".wrl", "#End of file.", outputStream.str());
  // outputFile.close();
  return 1;
}