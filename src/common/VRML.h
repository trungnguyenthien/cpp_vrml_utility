#ifndef COMMON_VRML_H_
#define COMMON_VRML_H_

#include <initializer_list>
#include <vector>

#include "Model.h"

using namespace std;

using FaceDef = vector<int>;

class VrmlObject {
public:
  Color diffuseColor;
  Color emissiveColor;
  vector<Point> points;
};

class VrmlFaceSet : public VrmlObject {
public:
  VrmlFaceSet();
  vector<FaceDef> faces;
};
class VrmlLineSet : public VrmlObject {
public:
  vector<Point> polylines;
};

bool isNumberArray(string str);
Point parsePoint(string token);
FaceDef parseFace(string token);
vector<float> parseColor(string token);
vector<string> vrml2tokenStrings(string line);
vector<VrmlObject *> read_vrml_file(string file);
vector<FaceDef> makeFaces(vector<vector<int>> points);
#endif  // COMMON_VRML_H_