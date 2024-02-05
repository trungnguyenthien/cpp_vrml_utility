#include "Geometry3D.h"

#include <iostream>
#include <vector>

#include "../common/Debug.h"

// Trả về true nếu trong segment có đoạn AB hoặc BA
bool hasExistedSegment(vector<Segment> &segments, Point &pA, Point &pB) {
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

// Hàm để giải phóng bộ nhớ của polyhedron
// void deleteShape3D(void *shape) { delete static_cast<Polyhedron *>(shape); }

// Tạo Plane_3 bởi 3 điểm p0, p1, p2
Plane_3 createPlane3(Point p0, Point p1, Point p2) {
  // Chuyển đổi Point sang Point_3 của CGAL
  Point_3 point0(p0.x, p0.y, p0.z);
  Point_3 point1(p1.x, p1.y, p1.z);
  Point_3 point2(p2.x, p2.y, p2.z);

  // Tạo Plane_3 từ ba điểm
  Plane_3 plane(point0, point1, point2);
  return plane;
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
  // Plane_3 plane1 = createPlane3(plane[0], plane[1], plane[2]);
  Plane_3 planez = createPlane3Z(z);
  try {
    auto intersectionSegment = intersection2Plane(plane[0], plane[1], plane[2], planez);
    auto pointSource = convertFromPoint_3(intersectionSegment.source());
    auto pointTarget = convertFromPoint_3(intersectionSegment.target());
    return pair<Point, Point>(pointSource, pointTarget);
  } catch (const std::runtime_error &e) {
    throw runtime_error("intersectionZ: An error occurred: { " + std::string(e.what()) + " }");
  }
  return pair<Point, Point>(Point(0, 0, 0), Point(0, 0, 0));
}

// Return TRUE nếu: Điểm X nằm trên đoạn thẳng AB
// Các trường hợp return FALSE: X không nằm trên đoạn AB, X trùng với A hoặc B
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

// Trả về danh sách Points sau khi {sourcePoints} đã excude các point trong {removePoints}
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

vector<Point> polygonAtZ(vector<Point> shapePoints, vector<vector<int>> faceSet, int z) {
  vector<Point> output;
  vector<Segment> segments;

  for (auto face : faceSet) {
    try {
      auto facePoint = getFacePoint(shapePoints, face);
      auto segmentPairPoint = intersectionZ(facePoint, z);
      auto sourcePoint = segmentPairPoint.first;
      auto targetPoint = segmentPairPoint.second;
      Segment mySegment(sourcePoint.x, sourcePoint.y, sourcePoint.z, targetPoint.x, targetPoint.y,
                        targetPoint.z);
      if (hasExistedSegment(segments, sourcePoint, targetPoint)) {
        continue;
      }
      segments.push_back(mySegment);

      if (!hasExistedPoint(output, sourcePoint)) {
        cout << "Add point " << sourcePoint.x << " " << sourcePoint.y << endl;
        output.push_back(sourcePoint);
      } else {
        cout << "\tExisted point " << sourcePoint.x << " " << sourcePoint.y << endl;
        output.push_back(sourcePoint);
      }

      if (!hasExistedPoint(output, targetPoint)) {
        cout << "Add point " << targetPoint.x << " " << targetPoint.y << endl;
        output.push_back(targetPoint);
      } else {
        cout << "\tExisted point " << targetPoint.x << " " << targetPoint.y << endl;
        output.push_back(targetPoint);
      }

    } catch (const std::runtime_error &e) {
      std::cerr << "polygonAtZ: An error occurred: " << e.what() << std::endl;
    }
  }

  auto removePoint = findPointsInSegment(output);
  output = excludePoints(output, removePoint);

  return output;
}
