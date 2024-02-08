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
vector<string> vrml2tokenStrings(string line);
vector<VrmlObject *> read_vrml_file(string file);
vector<FaceDef> makeFaces(vector<vector<int>> points);

// Kiểm tra xem hai FaceDef có cạnh chung hay không
bool haveCommonEdge(FaceDef &f1, FaceDef &f2);

// void sortVertex(const FaceDef &f1, const FaceDef &f2);

// Cấu trúc Point(float x, float y, float z)
// Cấu trúc Segment(Point p1, Point p2)
// Input của FindPolygonFunction là {inputSegments} là tập các Segment của 1 hoặc nhiều polygon
// Method find() FindPolygonFunction có chức năng tìm tập các polygon được thứ tự bằng
// vector<Point>. Mỗi polygon cần đáp ứng:
// - Mỗi set<vector<Point>> là 1 tập các polygon đáp ứng yêu cầu bài toán
// - Mỗi polygon (vector<Point>) có điểm đầu và điểm cuối trùng nhau, 2 điểm liên tiếp trong polygon
// phải được quy định trong {inputSegments}
// - Mỗi cạnh trọng polygon có 2 đỉnh nối với 2 cạnh liền kề.
// - Các polygon trong một bộ kết quả không được TỰ CÁT hoặc CẮT LẪN NHAU.
// - Kết quả của `find()` là một `set<set<vector<Point>>>` tức nhiều bộ kết quả, mỗi bộ kết quả gồm
// nhiều Polygon.
class FindPolygonFunction {
public:
  FindPolygonFunction() {}
  set<Segment> inputSegments;
  FindPolygonFunction(const set<Segment> &segments) : inputSegments(segments) {}

  set<set<vector<Point>>> find() {
    // printSetSegment(this->inputSegments);
    set<set<vector<Point>>> resultPolygons;
    // cout << "find 1" << endl;
    // Tạo một map để lưu trữ các kết nối giữa các điểm
    map<Point, set<Point>> pointConnections;

    for (const auto &segment : inputSegments) {
      // cout << "find 2" << endl;
      pointConnections[segment.p1].insert(segment.p2);
      pointConnections[segment.p2].insert(segment.p1);
    }

    // Duyệt qua mỗi điểm để bắt đầu tìm kiếm các polygon
    for (const auto &pointPair : pointConnections) {
      // cout << "find 3" << endl;
      vector<Point> path;
      set<vector<Point>> foundPolygons;
      findPolygons(pointPair.first, pointPair.first, path, pointConnections, foundPolygons, true);
      cout << "find 4 " << foundPolygons.size() << endl;
      printSetVectorPoint(foundPolygons);
      if (!foundPolygons.empty() && !isSelfCross(foundPolygons) && !is2PolyCross(foundPolygons)) {
        printSetVectorPoint(foundPolygons);
        resultPolygons.insert(foundPolygons);
      }
    }

    return resultPolygons;
  }

private:
  void findPolygons(const Point &start, const Point &current, vector<Point> &path,
                    const map<Point, set<Point>> &connections, set<vector<Point>> &foundPolygons,
                    bool isFirstPoint) {
    if (!isFirstPoint && start == current) {
      if (path.size() > 2 && path.front() == path.back()) {
        foundPolygons.insert(path);
      }
      return;
    }

    for (const auto &next : connections.at(current)) {
      if (std::find(path.begin(), path.end(), next) == path.end() ||
          (isFirstPoint && next == start)) {
        path.push_back(current);
        findPolygons(start, next, path, connections, foundPolygons, false);
        path.pop_back();
      }
    }
  }
};

#endif  // COMMON_VRML_H_