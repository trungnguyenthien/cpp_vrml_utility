#ifndef COMMON_VRML_H_
#define COMMON_VRML_H_

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <map>
#include <set>
#include <unordered_set>
#include <vector>

#include "Debug.h"
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

bool doIntersect(Point p1, Point q1, Point p2, Point q2);

bool isSelfCross(vector<Point> polyPoints);

bool isSelfCross(set<vector<Point>> setPolyPoints);

bool is2PolyCross(vector<Point> poly1, vector<Point> poly2);

bool is2PolyCross(set<vector<Point>> setPoly);

bool isNumberArray(string str);
Point parsePoint(string token);
FaceDef parseFace(string token);

vector<vector<int>> splitFace3P(vector<int> points);

vector<float> parseColor(string token);
vector<string> vrml2tokenStrings(string &line);
vector<VrmlObject *> read_vrml_file(string file);
vector<FaceDef> makeFaces(vector<vector<int>> points);

#endif  // COMMON_VRML_H_