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
typedef Kernel::Vector_3 Vector_3;
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
  Triangle_3 triangle =
      Triangle_3(convertToPoint_3(p0), convertToPoint_3(p1), convertToPoint_3(p2));
  cout << " intersection2Plane 1" << endl;
  // Thực hiện phép giao cắt giữa hai mặt phẳng
  Object result = CGAL::intersection(triangle, plane2);

  cout << " intersection2Plane 2" << endl;
  // Kiểm tra kết quả và trả về Segment_3 nếu phép giao cắt là một đoạn thẳng
  if (const Segment_3 *s = CGAL::object_cast<Segment_3>(&result)) {
    cout << " intersection2Plane 3" << endl;
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
  cout << "Intersection Z 1" << endl;
  // Plane_3 plane1 = createPlane3(plane[0], plane[1], plane[2]);
  cout << "Intersection Z 2" << endl;
  Plane_3 planez = createPlane3Z(z);
  cout << "Intersection Z 3" << endl;
  try {
    auto intersectionSegment = intersection2Plane(plane[0], plane[1], plane[2], planez);
    cout << "Intersection Z 4" << endl;
    auto pointSource = convertFromPoint_3(intersectionSegment.source());
    cout << "Intersection Z 5" << endl;
    auto pointTarget = convertFromPoint_3(intersectionSegment.target());
    cout << "Intersection Z 6" << endl;
    return pair<Point, Point>(pointSource, pointTarget);
  } catch (const std::runtime_error &e) {
    throw runtime_error("intersectionZ: An error occurred: " + std::string(e.what()));
    // std::cerr << "intersectionZ: An error occurred: " << e.what() << std::endl;
  }
  return pair<Point, Point>(Point(0, 0, 0), Point(0, 0, 0));
}

vector<Point> polygonAtZ(vector<Point> shapePoints, vector<vector<int>> faceSet, int z) {
  vector<Point> output;
  vector<Segment> segments;

  for (auto face : faceSet) {
    try {
      auto facePoint = getFacePoint(shapePoints, face);
      // cout << "facePoint ";
      // printVectorPoints(facePoint);
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
        output.push_back(sourcePoint);
      }

      if (!hasExistedPoint(output, targetPoint)) {
        output.push_back(targetPoint);
      }

    } catch (const std::runtime_error &e) {
      std::cerr << "polygonAtZ: An error occurred: " << e.what() << std::endl;
    }
  }
  return output;
}
