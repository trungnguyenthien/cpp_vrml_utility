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

// FaceDef là tập các vertex thuộc 1 mặt phẳng (plane) có giới hạn bởi các cạnh
// Một tập vertex thường có 3-4 vertex
// using FaceDef = vector<int>;

// Sắp xếp lại các FaceDef trong faces sao cho: 2 mặt phẳng có chung cạnh nằm kế bên nhau.
// Mỗi FaceDef trong faces cũng cần sắp xếp thứ tự các vertex sao cho 2 FaceDef cạnh nhau có khai
// báo cạnh kề nhau. Ví dụ: [1,2,3], [2,3,6], [3, 6, 5, 7]
vector<FaceDef> sortFaceByEdge(vector<FaceDef> faces);

// Kiểm tra xem hai FaceDef có cạnh chung hay không
bool haveCommonEdge(FaceDef &f1, FaceDef &f2);

// void sortVertex(const FaceDef &f1, const FaceDef &f2);
#endif  // COMMON_VRML_H_