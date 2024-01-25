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

class IndexedFaceSet : VrmlObject {
public:
  vector<FaceDef> faces;
};
class IndexedLineSet : VrmlObject {
public:
  vector<Point> polylines;
};

bool isNumberArray(string str);
Point parsePoint(string token);
vector<float> parseColor(string token);
vector<string> vrml2tokenStrings(string line);
vector<VrmlObject> read_vrml_file(string file);

#endif  // COMMON_VRML_H_