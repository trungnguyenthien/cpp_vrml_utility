#ifndef COMMON_VRML_H_
#define COMMON_VRML_H_

#include <initializer_list>
#include <vector>

#include "Model.h"

using namespace std;

using FaceDef = vector<int>;

class VrmlObject {
public:
  VrmlObject();
  Color diffuseColor;
  Color emissiveColor;
  vector<Point> points;
  virtual ~VrmlObject() {}
};

class VrmlFaceSet : public VrmlObject {
public:
  VrmlFaceSet();
  // virtual ~VrmlFaceSet() {}
  vector<FaceDef> faces;
  // Trả string chứa các phần tử của points và faces
  string debug_description();
};

class VrmlLineSet : public VrmlObject {
public:
  VrmlLineSet();
  // virtual ~VrmlLineSet() {}
  vector<Point> polylines;
};

bool isNumberArray(string str);
Point parsePoint(string token);
FaceDef parseFace(string token);
void printVector2Int(string message, vector<vector<int>> contents);
void printVectorInt(const std::string &message, const std::vector<int> &contents);
vector<float> parseColor(string token);
vector<string> vrml2tokenStrings(string line);
vector<VrmlObject *> read_vrml_file(string file);
vector<FaceDef> makeFaces(vector<vector<int>> points);
#endif  // COMMON_VRML_H_