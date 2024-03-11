#include "Debug.h"

#include <iostream>
#include <limits>
#include <sstream>  // Để sử dụng stringstream
#include <string>
#include <vector>

std::string all_ascii(const std::string &text) {
  std::stringstream ss;  // Tạo một stringstream để dễ dàng xây dựng chuỗi kết quả
  for (size_t i = 0; i < text.length(); ++i) {
    if (i > 0) {
      ss << '.';  // Thêm dấu chấm giữa các mã ASCII, trừ trước mã đầu tiên
    }
    ss << static_cast<int>(text[i]);  // Thêm mã ASCII của ký tự hiện tại
  }
  return ss.str();  // Trả về chuỗi kết quả
}

int sumAscii(const std::string &text) {
  int sum = 0;
  for (char c : text) {
    sum += static_cast<int>(c);  // Chuyển đổi ký tự sang mã ASCII và cộng dồn vào sum
  }
  return sum;
}

void printVectorString(const std::vector<std::string> &strings) {
  for (const std::string &str : strings) {
    std::cout << str << std::endl;
  }
}

void printVectorVectorInt(const std::vector<std::vector<int>> &ints) {
  int groupIndex = 0;
  for (const auto &vec : ints) {  // Duyệt qua từng vector<int>
    cout << "------ GROUP " << groupIndex << ": ";
    groupIndex++;
    for (const auto &val : vec) {  // Duyệt qua từng int trong vector
      std::cout << val << " ";
    }
    std::cout << std::endl;  // In dấu xuống dòng sau mỗi vector<int>
  }
}

void printVectorVectorPoint(const vector<vector<Point>> &points) {
  for (const auto &vec : points) {  // Duyệt qua từng vector<Point>
    cout << "-------------" << endl;
    for (const auto &point : vec) {  // Duyệt qua từng Point trong vector
      std::cout << point.toString() << "," << endl;
    }
    std::cout << std::endl;  // In dấu xuống dòng sau mỗi vector<Point>
  }
}

void printSetSegment(const set<Segment> &segments) {
  for (auto &segment : segments) {
    std::cout << "Segment: "
              << "(" << segment.p1.x << ", " << segment.p1.y << ", " << segment.p1.z << ") "
              << "to "
              << "(" << segment.p2.x << ", " << segment.p2.y << ", " << segment.p2.z << ")"
              << std::endl;
  }
}

// Hàm để in các điểm trong các polygon
void printSetVectorPoint(const set<vector<Point>> &setPolygons) {
  for (const auto &polygon : setPolygons) {  // Duyệt qua mỗi polygon trong set
    std::cout << "Polygon: ";
    for (const auto &point : polygon) {  // Duyệt qua mỗi điểm trong polygon
      std::cout << "(" << point.x << ", " << point.y << ", " << point.z << ") ";
    }
    std::cout << std::endl;
  }
}

void printMinMaxPoint(const std::vector<Point> &points) {
  if (points.empty()) {
    std::cout << "Vector is empty." << std::endl;
    return;
  }

  float Xmin = std::numeric_limits<float>::max();
  float Xmax = std::numeric_limits<float>::lowest();
  float Ymin = std::numeric_limits<float>::max();
  float Ymax = std::numeric_limits<float>::lowest();
  float Zmin = std::numeric_limits<float>::max();
  float Zmax = std::numeric_limits<float>::lowest();

  for (const auto &point : points) {
    Xmin = std::min(Xmin, point.x);
    Xmax = std::max(Xmax, point.x);
    Ymin = std::min(Ymin, point.y);
    Ymax = std::max(Ymax, point.y);
    Zmin = std::min(Zmin, point.z);
    Zmax = std::max(Zmax, point.z);
  }

  std::cout << "Xmin: " << Xmin << ", Xmax: " << Xmax << ", "
            << "Ymin: " << Ymin << ", Ymax: " << Ymax << ", "
            << "Zmin: " << Zmin << ", Zmax: " << Zmax << std::endl;
}

void printSetOfSets(const std::set<std::set<int>> &setOfSets) {
  for (const auto &innerSet : setOfSets) {
    std::cout << "{ ";
    for (int num : innerSet) {
      std::cout << num << " ";
    }
    std::cout << "} ";
  }
  std::cout << std::endl;
}

void printPairDouble(const vector<pair<double, double>> &pair) {
  for (const auto &p : pair) {
    std::cout << "(" << p.first << ", " << p.second << ")\n";
  }
}

void printVectorPoints(const vector<Point> &points) {
  int index = 0;
  cout << "printVectorPoints { " << endl;
  for (const Point &point : points) {
    std::cout << point.toString() << "," << std::endl;
  }
  cout << " }" << endl;
}

void printVector2Int(const string &message, const vector<vector<int>> &contents) {
  std::cout << message << std::endl;
  for (const auto &subVec : contents) {
    for (int num : subVec) {
      std::cout << num << " ";
    }
    std::cout << std::endl;
  }
}

void printVectorInt(const std::string &message, const std::vector<int> &contents) {
  std::cout << message << std::endl;
  for (int num : contents) {
    std::cout << num << " ";
  }
  std::cout << std::endl;
}
