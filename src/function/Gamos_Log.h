#ifndef COMMON_GAMOS_LOG_H_
#define COMMON_GAMOS_LOG_H_

#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../common/StringUtils.h"
using namespace std;

/*
Step#    X(mm)    Y(mm)    Z(mm) KinE(MeV)  dE(MeV) StepLeng TrackLeng  NextVolume ProcName
    0       69     2.41     5.35     0.232        0        0         0       water initStep
    1     69.1     2.14     5.26         0    0.232    0.575     0.575       water eIoni
*/
struct G4TrackRow {
  int Step;
  float Xmm;
  float Ymm;
  float Zmm;
  float KinE;
  float dE;
  float StepLeng;
  float TrackLeng;
  string NextVolume;
  string ProcName;
  //----------------------------------------------------------------
  string Particle;
  int TrackID;
  int ParentID;

  void printDebugInfo() {
    std::cout << "Step: " << Step << ", Xmm: " << Xmm << ", Ymm: " << Ymm << ", Zmm: " << Zmm
              << ", KinE: " << KinE << ", dE: " << dE << ", StepLeng: " << StepLeng
              << ", TrackLeng: " << TrackLeng << ", NextVolume: " << NextVolume
              << ", ProcName: " << ProcName << ", Table_Particle: " << Particle
              << ", Table_TrackID: " << TrackID << ", Table_ParentID: " << ParentID << std::endl;
  }

  string name() {
    ostringstream stream;
    stream << "* G4Track Information:   Particle = " << Particle << ",   Track ID = " << TrackID
           << ",   Parent ID = " << ParentID << " {Step: " << Step << "}";
    return stream.str();
  }
};

class G4TrackTable {
private:
  void parseG4TrackInformation(std::string text, std::string &Particle, int &TrackID,
                               int &ParentID) {
    // Biểu thức chính quy để trích xuất thông tin
    std::regex pattern(
        R"(\* G4Track Information:\s+Particle = ([^,]+),\s+Track ID = (\d+),\s+Parent ID = (\d+))");

    // Tìm sự khớp trong chuỗi văn bản
    std::smatch matches;
    if (std::regex_search(text, matches, pattern)) {
      // Lấy thông tin từ các nhóm trong biểu thức chính quy
      if (matches.size() == 4) {
        Particle = matches[1];
        TrackID = std::stoi(matches[2]);
        ParentID = std::stoi(matches[3]);
      }
    }
  }

public:
  vector<G4TrackRow> rows;

  static bool isG4TrackHeader(string line) {
    // Biểu thức chính quy để kiểm tra định dạng
    regex pattern(
        R"(\* G4Track Information:\s+Particle = ([^,]+),\s+Track ID = (\d+),\s+Parent ID = (\d+))");

    // Kiểm tra xem chuỗi line có khớp với biểu thức chính quy không
    return regex_match(line, pattern);
  }

  string Particle;
  int TrackID;
  int ParentID;

  void initHeader(string line) {
    // if (!isG4TrackHeader(line)) {
    //   return;
    // }

    //* G4Track Information:   Particle = e-,   Track ID = 1,   Parent ID = 0
    parseG4TrackInformation(line, Particle, TrackID, ParentID);
  }

  G4TrackRow *readRow(string line) {
    vector<string> columns = splitStringBySpace(line);
    if (columns.size() != 10) {
      return NULL;
    }

    G4TrackRow *row = new G4TrackRow();
    // Init Header
    row->ParentID = ParentID;
    row->Particle = Particle;
    row->TrackID = TrackID;
    // Set Column info
    row->Step = stoi(columns[0]);
    row->Xmm = stof(columns[1]);
    row->Ymm = stof(columns[2]);
    row->Zmm = stof(columns[3]);
    row->KinE = stof(columns[4]);
    row->dE = stof(columns[5]);
    row->StepLeng = stof(columns[6]);
    row->TrackLeng = stof(columns[7]);
    row->NextVolume = columns[8];
    row->ProcName = columns[9];

    rows.push_back(*row);
    return row;
  }
};

vector<G4TrackTable> readG4TrackTables(string file, function<void(G4TrackRow *)> func);

#endif  // COMMON_GAMOS_LOG_H_