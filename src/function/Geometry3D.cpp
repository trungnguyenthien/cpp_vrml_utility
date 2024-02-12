#include "Geometry3D.h"

#include <iostream>
#include <limits>
#include <map>
#include <random>
#include <set>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../common/Debug.h"
#include "../common/VRML.h"

Point randomPoint(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax) {
  // Khởi tạo một đối tượng phát sinh số ngẫu nhiên
  std::random_device rd;  // Sử dụng để khởi tạo (seed) cho đối tượng sinh số ngẫu nhiên
  std::mt19937 gen(rd());  // Mersenne Twister: đối tượng sinh số ngẫu nhiên

  // Khởi tạo đối tượng phân phối cho mỗi trục
  std::uniform_real_distribution<float> disX(xMin, xMax);
  std::uniform_real_distribution<float> disY(yMin, yMax);
  std::uniform_real_distribution<float> disZ(zMin, zMax);

  // Sinh số ngẫu nhiên cho mỗi trục
  float x = disX(gen);
  float y = disY(gen);
  float z = disZ(gen);

  return Point(x, y, z);
}

pair<Point, Point> getMinMaxPoint(const vector<vector<Point>> &points) {
  // Khởi tạo min và max với giá trị cực đại và cực tiểu
  Point minPoint(numeric_limits<float>::max(), numeric_limits<float>::max(),
                 numeric_limits<float>::max());
  Point maxPoint(numeric_limits<float>::lowest(), numeric_limits<float>::lowest(),
                 numeric_limits<float>::lowest());

  // Duyệt qua tất cả các vector chứa điểm
  for (const auto &pointVector : points) {
    // Duyệt qua từng điểm trong vector
    for (const auto &point : pointVector) {
      // Cập nhật min và max cho mỗi trục
      minPoint.x = min(minPoint.x, point.x);
      minPoint.y = min(minPoint.y, point.y);
      minPoint.z = min(minPoint.z, point.z);

      maxPoint.x = max(maxPoint.x, point.x);
      maxPoint.y = max(maxPoint.y, point.y);
      maxPoint.z = max(maxPoint.z, point.z);
    }
  }

  return {minPoint, maxPoint};
}

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
  Triangle_3 triangle = Triangle_3(p3_0, p3_1, p3_2);
  // Thực hiện phép giao cắt giữa hai mặt phẳng
  Object result = CGAL::intersection(triangle, plane2);
  // Kiểm tra kết quả và trả về Segment_3 nếu phép giao cắt là một đoạn thẳng
  if (const Segment_3 *s = CGAL::object_cast<Segment_3>(&result)) {
    return *s;
  } else {
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

bool checkPointInSidePolygon(Point point, vector<Point> polygonPoints) {
  // Chuyển đổi điểm và đa giác từ cấu trúc Point sang kiểu CGAL
  Point_2 cgalPoint(point.x, point.y);
  Polygon_2 polygon;
  for (const auto &p : polygonPoints) {
    polygon.push_back(Point_2(p.x, p.y));
  }

  // Sử dụng hàm CGAL để kiểm tra điểm có nằm bên trong đa giác không
  return polygon.bounded_side(cgalPoint) == CGAL::ON_BOUNDED_SIDE;
}

vector<vector<Point>> polygonAtZ(vector<Point> shapePoints, vector<vector<int>> faceSet, int z) {
  vector<Segment> segments;
  cout << "faceSet " << faceSet.size() << endl;
  for (auto face : faceSet) {
    try {
      auto facePoint = getFacePoint(shapePoints, face);
      auto segmentPairPoint = intersectionZ(facePoint, z);
      auto sourcePoint = segmentPairPoint.first;
      auto targetPoint = segmentPairPoint.second;
      Segment mySegment(sourcePoint.x, sourcePoint.y, sourcePoint.z, targetPoint.x, targetPoint.y,
                        targetPoint.z);
      segments.push_back(mySegment);
    } catch (const std::runtime_error &e) {
    }
  }
  return findPolygonPath(createConnections(segments));
}
