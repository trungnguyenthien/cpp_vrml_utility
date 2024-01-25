#include "Gamos_Log.h"

#include "../common/FileUtils.h"
#include "../common/StringUtils.h"

vector<G4TrackTable> readG4TrackTables(string file, function<void(G4TrackRow *)> func) {
  string tableHeader = "Step#    X(mm)    Y(mm)    Z(mm) KinE(MeV)  dE(MeV) StepLeng TrackLeng  NextVolume ProcName";
  string starLine = "*********************************************************************************************************";
  vector<G4TrackTable> output;
  bool inTable = false;
  forEachLine(file, [starLine, tableHeader, &inTable, func, &output](long lineIndex, string line) {
    if (G4TrackTable::isG4TrackHeader(line)) {
      inTable = true;
      G4TrackTable new_table;
      new_table.initHeader(line);
      output.push_back(new_table);
      return;
    }

    if (!inTable) {
      return;
    }

    if (line == tableHeader) {
      return;
    }

    if (line == starLine) {
      return;
    }

    if (line.empty()) {
      return;
    }

    G4TrackTable *table = &(output.back());

    vector<string> columns = splitStringBySpace(line);
    if (columns.size() != 10) {
      inTable = false;
      return;
    }

    G4TrackRow *row = table->readRow(line);
    if (row != NULL) {
      func(row);
    }
  });
  return output;
}