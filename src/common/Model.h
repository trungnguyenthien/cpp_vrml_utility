#ifndef COMMON_MODEL_H_
#define COMMON_MODEL_H_
#include <iomanip>  // Cho std::setprecision
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
struct Point {
  float x, y, z;

  Point(float x, float y, float z) : x(x), y(y), z(z) {}
  Point(float x, float y) : x(x), y(y), z(0) {}

  std::string toString() const {
    std::ostringstream oss;
    oss << "XYZ: (" << x << ", " << y << ", " << z << ")";
    return oss.str();
  }
};

void printVectorPoints(vector<Point> points);

struct Size3D {
  float width, height, depth;
  Size3D(float width, float height, float depth) : width(width), height(height), depth(depth) {}
  Size3D(float size) : width(size), height(size), depth(size) {}
};

struct Color {
  int red;
  int green;
  int blue;
  int alpha;  // Đối với độ trong suốt, không bắt buộc

  float redF() { return float(red) / 256; }
  float greenF() { return float(green) / 256; }
  float blueF() { return float(blue) / 256; }

  std::string toString() const {
    std::ostringstream oss;
    oss << "Color(R: " << red << ", G: " << green << ", B: " << blue << ", A: " << alpha << ")";
    return oss.str();
  }

  Color(int r, int g, int b, int a = 255) : red(r), green(g), blue(b), alpha(a) {}

  static Color parseColorFromHex(std::string colorHex) {
    // Kiểm tra định dạng chuỗi có hợp lệ không (đủ độ dài và bắt đầu bằng '#')
    if (colorHex.size() != 7 || colorHex[0] != '#') {
      throw std::invalid_argument("Định dạng màu hex không hợp lệ.");
    }

    // Bỏ qua ký tự '#' đầu tiên và chuyển đổi từng phần màu thành số nguyên
    int red = std::stoi(colorHex.substr(1, 2), nullptr, 16);
    int green = std::stoi(colorHex.substr(3, 2), nullptr, 16);
    int blue = std::stoi(colorHex.substr(5, 2), nullptr, 16);

    // Trả về một đối tượng Color với alpha mặc định là 255 (không trong suốt)
    return Color(red, green, blue);
  }

  std::string wrlColorStr() {
    // Tạo một stream để định dạng chuỗi
    std::ostringstream stream;

    // Định dạng và làm tròn số float đến 2 chữ số sau dấu chấm
    stream << std::fixed << std::setprecision(2) << red / 256.0f << " "  // r
           << green / 256.0f << " "                                      // g
           << blue / 256.0f;                                             // b

    // Trả về chuỗi định dạng
    return stream.str();
  }
};

#endif  // COMMON_MODEL_H_