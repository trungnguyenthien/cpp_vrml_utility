#include "Geometry3D.h"

#include <algorithm>
#include <initializer_list>
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

// Hàm tính vector từ 2 điểm
Point vectorBetweenPoints(const Point &from, const Point &to) {
  return Point(to.x - from.x, to.y - from.y, to.z - from.z);
}

// Hàm tính độ dài của vector
float vectorLength(const Point &v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }

// Hàm tính tích vô hướng của 2 vector
float dotProduct(const Point &v1, const Point &v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// Hàm tính góc giữa 2 vector (trả về đơn vị độ)
float angleBetweenVectors(const Point &v1, const Point &v2) {
  float dot = dotProduct(v1, v2);
  float lengthProduct = vectorLength(v1) * vectorLength(v2);
  float cosAngle = dot / lengthProduct;
  return acos(cosAngle) * (180 / M_PI);
}

// {points} là tập điểm theo thứ tự của một polyline
// Giả sử có các điểm sau: A, B, C, D, E, F, G, H, I, J, K, L
// Nếu góc C và G có số đo góc trong khoảng [0, 1] độ và [359, 360] độ thì loại bỏ 2 điểm C và G.
// Kết quả mảng {points} sau khi xử lý là A, B, D, E, F, H, I, J, K, L
void removePoint(std::vector<Point> &points) {
  std::vector<size_t> pointsToRemove;

  for (size_t i = 1; i < points.size() - 1; ++i) {
    Point v1 = vectorBetweenPoints(points[i - 1], points[i]);
    Point v2 = vectorBetweenPoints(points[i], points[i + 1]);
    float angle = angleBetweenVectors(v1, v2);

    if ((angle >= 0 && angle <= 1) || (angle >= 359 && angle <= 360)) {
      // cout << "pointsToRemove.push_back " << i << endl;
      pointsToRemove.push_back(i);
    }
  }

  // Loại bỏ các điểm từ cuối danh sách để không làm thay đổi chỉ số của các điểm cần xóa tiếp theo
  for (auto it = pointsToRemove.rbegin(); it != pointsToRemove.rend(); ++it) {
    points.erase(points.begin() + *it);
  }
}

// Cấu trúc Point(float x, float y, float z)
// Giả sử vector {points} có các điểm {A1, A2, A3, A4, C, B1, B2, B3, B4, B5, C, A6, A7, A8, A9}
// Hãy tách thành 2 vector: { {A1, A2, A3, A4, C, A6, A7, A8, A9 }, {C, B1, B2, B3, B4, B5} } Mỗi
// vector chỉ có 1 điểm C
vector<vector<Point>> splitPolygon(const vector<Point> &points, Point C) {
  std::vector<std::vector<Point>> result(2);  // Chuẩn bị 2 vector để chứa kết quả
  int firstCIndex = -1, secondCIndex = -1;

  // Tìm chỉ số của 2 điểm C
  for (int i = 0; i < points.size(); ++i) {
    if (points[i] == C) {
      if (firstCIndex == -1) {
        firstCIndex = i;
      } else {
        secondCIndex = i;
        break;  // Tìm thấy 2 điểm C, dừng tìm kiếm
      }
    }
  }

  if (firstCIndex != -1 && secondCIndex != -1) {
    // Tạo vector đầu tiên từ A1 đến C và từ C đến A9
    for (int i = 0; i < firstCIndex; ++i) {
      result[0].push_back(points[i]);
    }
    for (int i = secondCIndex; i < points.size(); ++i) {
      result[0].push_back(points[i]);
    }

    // Tạo vector thứ hai từ C đến B5
    for (int i = firstCIndex; i < secondCIndex; ++i) {
      result[1].push_back(points[i]);
    }
  }

  return result;
}

vector<vector<Point>> splitPolygon(const vector<Point> &points) {
  // cout << "splitPolygon points.size()" << points.size() << endl;
  // cout << "splitPolygon input " << points.size() << endl;
  int n = points.size();
  // Kiểm tra từng cặp cạnh của đa giác
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      int next_i = (i + 1) % n;
      int next_j = (j + 1) % n;
      // Không kiểm tra cạnh kề
      if (i == j || next_i == j || i == next_j || (i == 0 && next_j == n - 1))
        continue;
      // cout << "check point [" << i << " " << next_i << "] x [" << j << " " << next_j << "]" <<
      // endl;
      Point *ipoint = intersectionPoint(points[i], points[next_i], points[j], points[next_j]);
      if (ipoint != NULL) {
        // cout << "splitPolygon ipoint " << ipoint->toString() << endl;
        auto copyPoints = points;
        auto splitPoint = *ipoint;
        delete ipoint;
        insertPoint(splitPoint, copyPoints, j);
        insertPoint(splitPoint, copyPoints, i);
        // cout << "splitPolygon copyPoints.size() " << copyPoints.size() << endl;
        auto result = splitPolygon(copyPoints, splitPoint);
        // cout << "splitPolygon return " << result.size() << endl;
        // cout << "splitPolygon return result.size() " << points.size() << endl;
        // printVectorVectorPoint(result);
        return result;
      }
    }
  }
  cout << "splitPolygon return {}" << endl;
  return {};
}

// Sử dụng hàm `bool isSimplePolygon(const vector<Point> &points)` để kiểm tra {sourcePolygon} có tự
// cắt nhau hay không (FALSE là có tự cắt). Nếu {sourcePolygon} có tự cắt nhau như hình chữ số 8.
// Hãy tách thành nhiều polygon.
vector<vector<Point>> simplePolygons(vector<vector<Point>> sourcePolygons) {
  // cout << "__simplePolygons() " << sourcePolygons.size() << endl;
  vector<vector<Point>> result;
  for (auto polygon : sourcePolygons) {
    // cout << "simplePolygons Before removePoint " << polygon.size() << endl;
    removePoint(polygon);
    // cout << "simplePolygons After removePoint " << polygon.size() << endl;
    if (isSimplePolygon(polygon)) {
      // cout << "simplePolygons isSimplePolygon = TRUE " << endl;
      result.push_back(polygon);
      continue;
    }

    // cout << "simplePolygons = FALSE, start splitPolygon" << endl;

    auto splitPolys = splitPolygon(polygon);
    // if(splitPolys.size() > ) {}
    auto result2 = simplePolygons(splitPolys);
    for (auto p : result2) {
      result.push_back(p);
    }
  }
  return result;
}

// Hàm để kiểm tra hướng của ba điểm: p, q, r.
// Trả về 0 nếu colinear, 1 nếu clockwise, -1 nếu counterclockwise
int orientation(const Point &p, const Point &q, const Point &r) {
  float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
  if (val == 0)
    return 0;                 // colinear
  return (val > 0) ? 1 : -1;  // clock or counterclock wise
}

bool onSegment(const Point &p, const Point &q, const Point &r) {
  return (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) && q.y <= std::max(p.y, r.y) &&
          q.y >= std::min(p.y, r.y));
}

// Insert {p} sau vị trí {afterIndex}
void insertPoint(const Point p, vector<Point> &points, int afterIndex) {
  // Kiểm tra xem chỉ số có hợp lệ không
  if (afterIndex >= 0 && afterIndex < points.size()) {
    // Sử dụng phương thức insert() của vector để chèn điểm
    // Lưu ý: iterator trỏ đến vị trí afterIndex + 1, vì chúng ta muốn chèn sau afterIndex
    points.insert(points.begin() + afterIndex + 1, p);
  } else {
    std::cout << "Index out of range. Point not inserted." << std::endl;
  }
}

// Tìm điểm giao nhau của đoạn (segment) a và b.
// Nếu a và b không giao nhau hoặc giao tại điểm đầu hoặc cuối đoạn thì trả về NULL
Point *intersectionPoint(const Point &p1, const Point &q1, const Point &p2, const Point &q2) {
  // Chuyển đổi Point sang Point_2 của CGAL
  Point_2 start1(p1.x, p1.y), end1(q1.x, q1.y);
  Point_2 start2(p2.x, p2.y), end2(q2.x, q2.y);

  // Tạo các đoạn thẳng từ điểm
  Segment_2 seg1(start1, end1), seg2(start2, end2);

  // Kiểm tra giao nhau
  auto result = CGAL::intersection(seg1, seg2);

  if (result) {
    // cout << "intersectionPoint if (result) {" << endl;
    // Nếu có giao nhau, kiểm tra loại giao nhau
    if (const Point_2 *ipoint = boost::get<Point_2>(&*result)) {
      // Điểm giao nhau không nằm ở đầu hoặc cuối của đoạn thẳng
      if (*ipoint != start1 && *ipoint != end1 && *ipoint != start2 && *ipoint != end2) {
        // cout << "intersectionPoint OK" << endl;
        return new Point(CGAL::to_double(ipoint->x()), CGAL::to_double(ipoint->y()),
                         CGAL::to_double(p1.z));
      }
    }
  }
  // cout << "intersectionPoint NULL" << endl;
  return nullptr;
}
// Hàm kiểm tra xem hai đoạn thẳng có giao nhau không
bool doIntersect(const Point &p1, const Point &q1, const Point &p2, const Point &q2) {
  // Tìm bốn hướng cho hai cặp điểm và đoạn thẳng
  int o1 = orientation(p1, q1, p2);
  int o2 = orientation(p1, q1, q2);
  int o3 = orientation(p2, q2, p1);
  int o4 = orientation(p2, q2, q1);

  // Kiểm tra điều kiện chung
  if (o1 != o2 && o3 != o4)
    return true;

  return false;
}

// Kiểm tra và xử lý đa giác:
// Return TRUE nếu: đa giác không tự giao và không có cạnh nào cắt nhau hoặc chồng lên nhau
// Không sử dụng thư viện CGAL
bool isSimplePolygon(const vector<Point> &points) {
  int n = points.size();
  // Kiểm tra từng cặp cạnh của đa giác
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      int next_i = (i + 1) % n;
      int next_j = (j + 1) % n;
      // Không kiểm tra cạnh kề
      if (i == j || next_i == j || i == next_j || (i == 0 && next_j == n - 1))
        continue;
      if (doIntersect(points[i], points[next_i], points[j], points[next_j])) {
        // cout << "n = " << n << ", i = " << i << ", j = " << j << ", next_i = " << next_i
        //      << ", next_j =" << next_j << endl;
        return false;
      }
    }
  }
  return true;
}

float randomFloat(float min, float max) {
  std::vector<float> numbers;
  for (float num = min; num < max; num += 0.01f) {
    numbers.push_back(num);
  }
  std::random_shuffle(numbers.begin(), numbers.end());
  auto result = numbers.front();

  // srand(static_cast<unsigned>(time(0)));
  // auto result = min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
  // cout << "randomFloat(" << min << ", " << max << ") = " << result << endl;
  return result;
}

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

// Hàm tìm min và max point
pair<Point, Point> getMinMaxPoint1(const vector<Point> &points) {
  // cout << "getMinMaxPoint1 " << points.size() << endl;
  // Khởi tạo min và max points với giá trị cực đại và cực tiểu
  Point minPoint(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(),
                 std::numeric_limits<float>::max());
  Point maxPoint(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(),
                 std::numeric_limits<float>::lowest());

  // Duyệt qua từng điểm để cập nhật min và max points
  for (const auto &point : points) {
    minPoint.x = std::min(minPoint.x, point.x);
    minPoint.y = std::min(minPoint.y, point.y);
    minPoint.z = std::min(minPoint.z, point.z);

    maxPoint.x = std::max(maxPoint.x, point.x);
    maxPoint.y = std::max(maxPoint.y, point.y);
    maxPoint.z = std::max(maxPoint.z, point.z);
  }
  // cout << "output getMinMaxPoint1 " << minPoint.toString() << " " << maxPoint.toString() << endl;
  return {minPoint, maxPoint};
}

pair<Point, Point> getMinMaxPoint2(const vector<vector<Point>> &points) {
  // cout << "getMinMaxPoint2 " << points.size() << endl;
  // Khởi tạo min và max với giá trị cực đại và cực tiểu
  Point minPoint(numeric_limits<float>::max(), numeric_limits<float>::max(),
                 numeric_limits<float>::max());
  Point maxPoint(numeric_limits<float>::lowest(), numeric_limits<float>::lowest(),
                 numeric_limits<float>::lowest());
  // cout << "Find min max in Points (size) " << points.size() << endl;
  // Duyệt qua tất cả các vector chứa điểm
  for (const auto &pointVector : points) {
    // Duyệt qua từng điểm trong vector
    for (const auto &point : pointVector) {
      // cout << "Find min max in pointVector (size) " << points.size() << endl;
      // Cập nhật min và max cho mỗi trục
      minPoint.x = min(minPoint.x, point.x);
      minPoint.y = min(minPoint.y, point.y);
      minPoint.z = min(minPoint.z, point.z);

      maxPoint.x = max(maxPoint.x, point.x);
      maxPoint.y = max(maxPoint.y, point.y);
      maxPoint.z = max(maxPoint.z, point.z);
    }
  }

  // cout << "output getMinMaxPoint2 " << minPoint.toStringXY() << " " << maxPoint.toStringXY() << endl;

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
        // cout << "Found path" << endl;
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

Plane_3 createPlane3Z(float z) {
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

pair<Point, Point> intersectionZ(vector<Point> plane, float z) {
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

bool checkPointInSidePolygon(const Point &point, const vector<Point> &polygonPoints) {
  // cout << "checkPointInSidePolygon polygonPoints.size() = " << polygonPoints.size() << endl;
  // Chuyển đổi điểm và đa giác từ
  // cấu trúc Point sang kiểu CGAL
  // cout << "polygonPoints Origins " << endl;
  // printVectorPoints(polygonPoints);
  if (!isSimplePolygon(polygonPoints)) {
    // cout << "\nNOT SIMPLE POLYGON" << endl;
    // printVectorPoints(polygonPoints);

    auto poly2s = simplePolygons({polygonPoints});
    // cout << "checkPointInSidePolygon poly2s.size() = " << poly2s.size() << endl;
    int countPoly2 = 0;
    for (auto p2 : poly2s) {
      // cout << "checkPointInSidePolygon polygonPoints After make Simple " << countPoly2++ << "  "
      // << endl; cout << "checkPointInSidePolygon p2.size() After make Simple " << p2.size() << "
      // " << endl; printVectorPoints(p2);
      if (checkPointInSidePolygon(point, p2)) {
        // cout << "checkPointInSidePolygon TRUE" << endl;
        return true;
      }
    }
    // cout << "checkPointInSidePolygon FALSE" << endl;
    return false;
  }

  // cout << "Simple Polygons" << endl;
  Point_2 cgalPoint(point.x, point.y);
  Polygon_2 polygon;
  for (const auto &p : polygonPoints) {
    if (polygon.size() > 0 && polygon[0].x() == static_cast<double>((p.x))) {
      continue;
    }
    polygon.push_back(Point_2(p.x, p.y));
  }
  try {
    // Sử dụng hàm CGAL để kiểm tra điểm có nằm bên trong đa giác không
    return polygon.bounded_side(cgalPoint) == CGAL::ON_BOUNDED_SIDE;
  } catch (...) {
    return false;
  }
}

bool checkPointInSidePolygons(const Point &point, const vector<vector<Point>> &polygonPoints) {
  for (const auto &p : polygonPoints) {
    // cout << "Polygon :" << p.size() << endl;
    // printVectorPoints(p);
    if (checkPointInSidePolygon(point, p)) {
      return true;
    }
  }
  return false;
}

vector<vector<Point>> polygonAtZ(const vector<Point> &shapePoints,
                                 const vector<vector<int>> &faceSet, float z) {
  vector<Segment> segments;
  // cout << "polygonAtZ shapePoints.size() = " << shapePoints.size() << endl;
  // cout << "polygonAtZ faceSet.size() = " << faceSet.size() << endl;
  // cout << "polygonAtZ z = " << z << endl;
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
  // cout << "polygonAtZ segments.size() = " << segments.size() << endl;
  auto polygonPath = findPolygonPath(createConnections(segments));

  return polygonPath;
}
