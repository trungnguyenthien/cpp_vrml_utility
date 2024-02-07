#ifndef COMMON_VRML_H_
#define COMMON_VRML_H_

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <map>
#include <set>
#include <unordered_set>
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

vector<vector<int>> splitFace3P(vector<int> points);

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
  set<Segment> inputSegments;

  FindPolygonFunction(const set<Segment> &segments) : inputSegments(segments) {}

  set<set<vector<Point>>> find() {
    set<set<vector<Point>>> resultPolygons;

    // Tạo một map để lưu trữ các kết nối giữa các điểm
    map<Point, set<Point>> pointConnections;
    for (const auto &segment : inputSegments) {
      pointConnections[segment.p1].insert(segment.p2);
      pointConnections[segment.p2].insert(segment.p1);
    }

    // Duyệt qua mỗi điểm để bắt đầu tìm kiếm các polygon
    for (const auto &pointPair : pointConnections) {
      vector<Point> path;
      set<vector<Point>> foundPolygons;
      findPolygons(pointPair.first, pointPair.first, path, pointConnections, foundPolygons, true);

      if (!foundPolygons.empty()) {
        resultPolygons.insert(foundPolygons);
      }
    }

    return resultPolygons;
  }

private:
    // Hàm kiểm tra sự giao nhau của hai đoạn thẳng
  bool doIntersect(Point p1, Point q1, Point p2, Point q2) {
    // Hàm phụ để kiểm tra sự sắp xếp theo chiều của ba điểm
    // và kiểm tra xem điểm q có nằm trên đoạn thẳng pr không
    auto onSegment = [](Point p, Point q, Point r) {
      if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) && q.y <= max(p.y, r.y) &&
          q.y >= min(p.y, r.y))
        return true;
      return false;
    };

    // Hàm phụ để tìm hướng xoay: trả về các giá trị khác nhau
    // cho các trường hợp Colinear (0), Clockwise (1) và Counterclockwise (2)
    auto orientation = [](Point p, Point q, Point r) {
      float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
      if (val == 0)
        return 0;                // colinear
      return (val > 0) ? 1 : 2;  // clock or counterclock wise
    };

    // Kiểm tra bốn điều kiện của sự giao nhau
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // Trường hợp chính: các điểm cắt nhau không phải là đầu mút
    if (o1 != o2 && o3 != o4)
      return true;

    // Trường hợp đặc biệt: các điểm colinear và giao nhau tại đầu mút
    // Được loại bỏ vì yêu cầu của câu hỏi

    return false;  // Không có trường hợp giao nhau
  }

  // {polyPoints} là tập các Point(float x, float y, float z) theo thứ tự tạo thành 1 polygon
  // Return TRUE nếu polygon có 2 cạnh bất kỳ cắt nhau tại điểm giữa mỗi cạnh.
  // Chú ý: điểm cắt nhau phải khác điểm đầu và cuối mỗi cạnh
  bool isSelfCross(vector<Point> polyPoints) {
    int n = polyPoints.size();
    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        // Bỏ qua kiểm tra giao nhau cho các cạnh kề nhau và cặp cạnh cuối cùng với cạnh đầu tiên
        if (abs(i - j) > 1 && i != 0 && j != n - 1) {
          if (doIntersect(polyPoints[i], polyPoints[(i + 1) % n], polyPoints[j],
                          polyPoints[(j + 1) % n])) {
            return true;
          }
        }
      }
    }
    return false;
  }

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