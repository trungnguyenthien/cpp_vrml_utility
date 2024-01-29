#ifndef FUNCTION_GEOMETRY3D_H_
#define FUNCTION_GEOMETRY3D_H_
#include <vector>

#include "../common/Model.h"

using namespace std;

// Tạo Polyhedron từ tập hợp các điểm {points} và danh sách các mặt phẳng {faces} dựa trên danh sách
// các điểm Trả về con trỏ Polyhedron (CGAL)
void *createShape3D(vector<Point> &points, vector<vector<int>> &faces);

// Hàm để giải phóng bộ nhớ của polyhedron
void deleteShape3D(void *shape);

// struct Point {
//   float x, y, z;
// }

// shape là con trỏ kiểu Polyhedron
// Hãy trả về cho tôi vector<Point> chứa các point tạo thành polygon của mặt cắt của Polyhedron và
// mặt cắt tại toạ độ z
vector<Point> polygonAtZ(void *shape, int z);

#endif  // FUNCTION_GEOMETRY3D_H_