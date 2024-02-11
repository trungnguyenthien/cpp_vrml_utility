#include "Geometry3D.h"

#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../common/Debug.h"
#include "../common/VRML.h"

// Cấu trúc Point(float x, float y, float z)
void printDebug(const Point &currentPoint, vector<Point> &polygonPath, vector<bool> &visited) {
  return;
  cout << "===========================================================" << endl;
  cout << "All Vertex: " << visited.size() << endl;
  std::cout << "Current Point: ";
  std::cout << currentPoint.toStringXY();
  std::cout << std::endl;

  std::cout << "Polygon Path: [";
  for (size_t i = 0; i < polygonPath.size(); ++i) {
    std::cout << polygonPath[i].toStringXY();
    if (i < polygonPath.size() - 1) {
      std::cout << ", ";
    }
  }

  std::cout << "]" << std::endl;

  cout << "polygonPath Size: " << polygonPath.size() << endl;

  std::cout << "Visited: [";
  for (size_t i = 0; i < visited.size(); ++i) {
    std::cout << (visited[i] ? "x" : "o");
    if (i < visited.size() - 1) {
      std::cout << ", ";
    }
  }
  std::cout << "]" << std::endl;
}

bool findNextPoint(const Point &currentPoint,
                   const unordered_map<Point, set<Point>, hash<Point>> &graphEdge,
                   vector<Point> &polygonPath, vector<bool> &visited,
                   const vector<Point> &allPoints) {
  polygonPath.push_back(currentPoint);

  printDebug(currentPoint, polygonPath, visited);

  // Không tồn tại cạnh có điểm đầu là startPoint
  if (graphEdge.find(currentPoint) == graphEdge.end())
    return false;

  for (const auto &nextPoint : graphEdge.at(currentPoint)) {
    // Kiểm tra xem đã trở về điểm bắt đầu để tạo thành đa giác đóng
    if (nextPoint == polygonPath.front() && polygonPath.size() > 2) {
      polygonPath.push_back(
          nextPoint);  // Đóng đa giác khi nextPoint trùng với startPoint (path.front())
      return true;
    }
    // Tìm điểm tiếp theo chưa được thăm
    auto nextPointIndex = find(allPoints.begin(), allPoints.end(), nextPoint) - allPoints.begin();
    if (!visited[nextPointIndex]) {
      visited[nextPointIndex] = true;
      if (findNextPoint(nextPoint, graphEdge, polygonPath, visited, allPoints))
        return true;
    }
  }
  return false;
}

vector<vector<Point>> findPolygonPath(vector<pair<Point, Point>> connections) {
  // Tập edge ứng mỗi từng đỉnh, mỗi đỉnh là key của map. Dùng để kiểm tra khả năng đi từ mỗi đỉnh.
  unordered_map<Point, set<Point>, hash<Point>> graphEdge;
  // Danh sách các đỉnh của polygon
  vector<Point> allPoints;
  for (const auto &conn : connections) {
    graphEdge[conn.first].insert(conn.second);
    graphEdge[conn.second].insert(conn.first);
    // conn.first chưa tồn tại trong allPoints
    if (find(allPoints.begin(), allPoints.end(), conn.first) == allPoints.end())
      allPoints.push_back(conn.first);
    // conn.second chưa tồn tại trong allPoints
    if (find(allPoints.begin(), allPoints.end(), conn.second) == allPoints.end())
      allPoints.push_back(conn.second);
  }

  vector<vector<Point>> polygons;
  vector<bool> visited(allPoints.size(), false);
  for (int i = 0; i < allPoints.size(); ++i) {
    // Tìm đỉnh chưa xuất hiện trọng polygon nào
    if (!visited[i]) {
      // Chuẩn bị path cho polygon đó
      vector<Point> path;
      // Đánh dấu đỉnh này đã duyệt, đây là đỉnh đầu tiên của polygon
      visited[i] = true;
      // Xác định path
      if (findNextPoint(allPoints[i], graphEdge, path, visited, allPoints)) {
        cout << "Found path" << endl;
        polygons.push_back(path);
      }
    }
  }

  return polygons;
}

// Convert vector<Segment> thành vector<pair<Point, Point>>
// Cấu trúc Segment(Point p1, Point p2)
// Cấu trúc Point(float x, float y, float z)
vector<pair<Point, Point>> createConnections(const vector<Segment> &segments) {
  vector<pair<Point, Point>> connections;
  for (const auto &segment : segments) {
    connections.push_back(make_pair(segment.p1, segment.p2));
  }
  return connections;
}

// Trả về true nếu trong segment có đoạn AB hoặc BA
bool hasExistedSegment(set<Segment> &segments, Point &pA, Point &pB) {
  for (const Segment &segment : segments) {
    if ((segment.p1 == pA && segment.p2 == pB) || (segment.p1 == pB && segment.p2 == pA)) {
      return true;
    }
  }
  return false;
}

// Trả về true nếu {points} có chứa point tạo bỏi px, py, pz
bool hasExistedPoint(const std::vector<Point> &points, float px, float py, float pz) {
  for (const Point &point : points) {
    if (std::fabs(point.x - px) < 1e-6 && std::fabs(point.y - py) < 1e-6 &&
        std::fabs(point.z - pz) < 1e-6) {
      return true;
    }
  }
  return false;
}

bool hasExistedPoint(const std::vector<Point> &points, Point p) {
  for (const Point &point : points) {
    if (std::fabs(point.x - p.x) < 1e-6 && std::fabs(point.y - p.y) < 1e-6 &&
        std::fabs(point.z - p.z) < 1e-6) {
      return true;
    }
  }
  return false;
}

Plane_3 createPlane3Z(int z) {
  Point_3 point_on_plane(0, 0, z);

  // Tạo vector pháp tuyến cho mặt phẳng. Đối với mặt phẳng song song với Oxy và có tọa độ z là 222,
  // vector pháp tuyến có thể là (0, 0, 1)
  Vector_3 normal_vector(0, 0, 1);

  // Tạo mặt phẳng từ một điểm và một vector pháp tuyến
  Plane_3 plane(point_on_plane, normal_vector);

  return plane;
}

Segment_3 intersection2Plane(Point p0, Point p1, Point p2, Plane_3 plane2) {
  Point_3 p3_0 = convertToPoint_3(p0);
  Point_3 p3_1 = convertToPoint_3(p1);
  Point_3 p3_2 = convertToPoint_3(p2);
  // cout << "intersection2Plane 1" << endl;
  Triangle_3 triangle = Triangle_3(p3_0, p3_1, p3_2);
  // cout << "intersection2Plane 2" << endl;
  // Thực hiện phép giao cắt giữa hai mặt phẳng
  Object result = CGAL::intersection(triangle, plane2);
  // cout << "intersection2Plane 3" << endl;
  // Kiểm tra kết quả và trả về Segment_3 nếu phép giao cắt là một đoạn thẳng
  if (const Segment_3 *s = CGAL::object_cast<Segment_3>(&result)) {
    // cout << "intersection2Plane 4" << endl;
    return *s;
  } else {
    // cout << "intersection2Plane intersection2Plane: Giao điểm không phải là một đoạn thẳng hoặc "
    //         "không tồn tại."
    // << endl;
    // Giao điểm không phải là một đoạn thẳng hoặc không tồn tại
    // Xử lý trường hợp này theo yêu cầu cụ thể của bạn
    throw std::runtime_error(
        "intersection2Plane: Giao điểm không phải là một đoạn thẳng hoặc không tồn tại.");
  }
}

vector<Point> getFacePoint(vector<Point> shapePoints, vector<int> faceSet) {
  vector<Point> facePoints;
  for (int i : faceSet) {
    facePoints.push_back(shapePoints[i]);
  }
  return facePoints;
}

Point convertFromPoint_3(Point_3 p3) { return Point(p3.x(), p3.y(), p3.z()); }
Point_3 convertToPoint_3(Point p) { return Point_3(p.x, p.y, p.z); }

pair<Point, Point> intersectionZ(vector<Point> plane, int z) {
  Plane_3 planez = createPlane3Z(z);
  // cout << "intersectionZ 2" << endl;
  try {
    // cout << "intersectionZ 3" << endl;
    auto intersectionSegment = intersection2Plane(plane[0], plane[1], plane[2], planez);
    // cout << "intersectionZ 4" << endl;
    auto pointSource = convertFromPoint_3(intersectionSegment.source());
    // cout << "intersectionZ 5" << endl;
    auto pointTarget = convertFromPoint_3(intersectionSegment.target());
    // cout << "intersectionZ 6" << endl;
    return pair<Point, Point>(pointSource, pointTarget);
  } catch (const std::runtime_error &e) {
    throw runtime_error("intersectionZ: An error occurred: { " + std::string(e.what()) + " }");
  }
  return pair<Point, Point>(Point(0, 0, 0), Point(0, 0, 0));
}

bool isPointsInSegment(Point x, Point a, Point b) {
  // Kiểm tra xem X có trùng với A hoặc B không
  if ((x.x == a.x && x.y == a.y && x.z == a.z) || (x.x == b.x && x.y == b.y && x.z == b.z)) {
    return false;  // X trùng với A hoặc B
  }

  // Tính vectơ AX và AB
  Point ax = {x.x - a.x, x.y - a.y, x.z - a.z};
  Point ab = {b.x - a.x, b.y - a.y, b.z - a.z};

  // Tính tích vô hướng (dot product) và tích chéo (cross product) để kiểm tra cùng phương
  float dotProduct = ax.x * ab.x + ax.y * ab.y + ax.z * ab.z;
  Point crossProduct = {ax.y * ab.z - ax.z * ab.y, ax.z * ab.x - ax.x * ab.z,
                        ax.x * ab.y - ax.y * ab.x};

  // Kiểm tra tích chéo bằng 0 (cùng phương) và dot product dương (cùng hướng)
  if (crossProduct.x == 0 && crossProduct.y == 0 && crossProduct.z == 0 && dotProduct > 0) {
    // Kiểm tra X có nằm giữa A và B không
    float abLengthSquared = ab.x * ab.x + ab.y * ab.y + ab.z * ab.z;
    return dotProduct < abLengthSquared;
  }

  return false;
}

vector<Point> excludePoints(vector<Point> sourcePoints, vector<Point> removePoints) {
  std::vector<Point> result;

  // Duyệt qua từng điểm trong sourcePoints
  for (const auto &sourcePoint : sourcePoints) {
    // Sử dụng std::find và toán tử == đã định nghĩa để kiểm tra xem sourcePoint có trong
    // removePoints không
    if (std::find(removePoints.begin(), removePoints.end(), sourcePoint) == removePoints.end()) {
      // Nếu không tìm thấy điểm trong removePoints, thêm vào kết quả
      result.push_back(sourcePoint);
    }
  }

  return result;
}

vector<Point> findPointsInSegment(vector<Point> segmentPoints) {
  vector<Point> output;
  for (int ix = 0; ix < segmentPoints.size(); ix++) {
    for (int ia = 0; ia < segmentPoints.size(); ia++) {
      for (int ib = 0; ib < segmentPoints.size(); ib++) {
        if (ix == ia || ix == ib || ia == ib) {
          continue;
        }

        if (isPointsInSegment(segmentPoints[ix], segmentPoints[ia], segmentPoints[ib])) {
          output.push_back(segmentPoints[ix]);
        }
      }
    }
  }

  return output;
}

float direction(const Point &pi, const Point &pj, const Point &pk) {
  return (pk.x - pi.x) * (pj.y - pi.y) - (pj.x - pi.x) * (pk.y - pi.y);
}

bool onSegment(const Point &pi, const Point &pj, const Point &pk) {
  if (std::min(pi.x, pj.x) <= pk.x && pk.x <= std::max(pi.x, pj.x) &&
      std::min(pi.y, pj.y) <= pk.y && pk.y <= std::max(pi.y, pj.y)) {
    return true;
  }
  return false;
}

// Đoạn AB tạo thàn từ điểm A và B, đoạn CD tạo thành từ điểm C và D
// Return TRUE nếu:
// - AB cắt CD
// - AB trùng CD
bool isCrossSegment(const Point &A, const Point &B, const Point &C, const Point &D) {
  float d1 = direction(C, D, A);
  float d2 = direction(C, D, B);
  float d3 = direction(A, B, C);
  float d4 = direction(A, B, D);

  if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) && ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) {
    return true;
  }

  // if (d1 == 0 && onSegment(C, D, A))
  //   return true;
  // if (d2 == 0 && onSegment(C, D, B))
  //   return true;
  // if (d3 == 0 && onSegment(A, B, C))
  //   return true;
  // if (d4 == 0 && onSegment(A, B, D))
  //   return true;

  return false;
}

// Return thành nhiều đa giác con đơn giản
// {sourcePolygon} đa giác phức tạp, có thể có các cạnh giao nhau
std::vector<std::vector<Point>> partitionPolygon(std::vector<Point> sourcePolygon) {
  vector<vector<Point>> result;
  vector<Segment> footprintSegment;
  vector<Point> polygon;
  polygon.push_back(sourcePolygon[0]);
  for (int i = 1; i < sourcePolygon.size(); i++) {
    Point A = sourcePolygon[i - 1];
    Point B = sourcePolygon[i];
    Segment AB(A, B);
    bool isCrossLastSegment = false;
    for (auto ls : footprintSegment) {
      if (isCrossSegment(ls.p1, ls.p2, A, B)) {
        isCrossLastSegment = true;
        break;
      }
    }

    if (!isCrossLastSegment) {
      footprintSegment.push_back(AB);
      if (!hasExistedPoint(polygon, B.x, B.y, B.z)) {
        polygon.push_back(B);
      }
    } else if (polygon.size() > 2) {
      result.push_back(polygon);
      polygon.clear();
    }
  }

  if (polygon.size() > 2) {
    result.push_back(polygon);
  }

  return result;
}

// Return true nếu {point} nằm trong polygon định nghĩa bỏi {polygonPoints}
// Sử dụng thư viên CGAL
bool __checkPointInSidePolygon(Point point, vector<Point> polygonPoints) {
  // Chuyển đổi danh sách điểm từ Point sang Point_2 của CGAL
  Polygon_2 poly;
  for (const auto &p : polygonPoints) {
    poly.push_back(Point_2(p.x, p.y));
  }

  // Chuyển đổi điểm kiểm tra
  Point_2 checkPoint(point.x, point.y);

  // Sử dụng CGAL để kiểm tra xem điểm có nằm bên trong đa giác không
  auto isInside = poly.bounded_side(checkPoint) == CGAL::ON_BOUNDED_SIDE;

  return isInside;
}

bool checkPointInSidePolygon(Point point, vector<Point> polygonPoints) {
  auto multiPolygons = partitionPolygon(polygonPoints);
  bool isInside = false;

  for (auto &polygon : multiPolygons) {
    printVectorPoints(polygon);
    if (__checkPointInSidePolygon(point, polygon)) {
      isInside = true;
      break;
    }
  }

  cout << "POINT: (" << point.x << ", " << point.y << ") ";
  if (isInside) {
    cout << "isInside" << endl;
  } else {
    cout << "Outside" << endl;
  }

  return isInside;
}

vector<vector<Point>> polygonAtZ(vector<Point> shapePoints, vector<vector<int>> faceSet, int z) {
  // vector<Point> output;
  vector<Segment> segments;
  cout << "faceSet " << faceSet.size() << endl;
  for (auto face : faceSet) {
    try {
      // cout << "polygonAtZ 2" << endl;
      auto facePoint = getFacePoint(shapePoints, face);
      // cout << "polygonAtZ 3" << endl;
      auto segmentPairPoint = intersectionZ(facePoint, z);
      // cout << "polygonAtZ 4" << endl;
      auto sourcePoint = segmentPairPoint.first;
      auto targetPoint = segmentPairPoint.second;
      Segment mySegment(sourcePoint.x, sourcePoint.y, sourcePoint.z, targetPoint.x, targetPoint.y,
                        targetPoint.z);
      // if (hasExistedSegment(segments, sourcePoint, targetPoint)) {
      //   continue;
      // }

      segments.push_back(mySegment);
      // segments.push_back(mySegment);

      // if (!hasExistedPoint(output, sourcePoint)) {
      //   // cout << "Add point " << sourcePoint.x << " " << sourcePoint.y << endl;
      //   output.push_back(sourcePoint);
      // } else {
      //   // cout << "\tExisted point " << sourcePoint.x << " " << sourcePoint.y << endl;
      //   output.push_back(sourcePoint);
      // }

      // if (!hasExistedPoint(output, targetPoint)) {
      //   // cout << "Add point " << targetPoint.x << " " << targetPoint.y << endl;
      //   output.push_back(targetPoint);
      // } else {
      //   // cout << "\tExisted point " << targetPoint.x << " " << targetPoint.y << endl;
      //   output.push_back(targetPoint);
      // }

    } catch (const std::runtime_error &e) {
      // throw runtime_error("polygonAtZ: An error occurred: " + string(e.what()));
      // std::cerr << "polygonAtZ: An error occurred: " << e.what() << std::endl;
    }
  }
  return findPolygonPath(createConnections(segments));
}
