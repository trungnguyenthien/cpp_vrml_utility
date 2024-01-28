#include <cairo.h>

#include <iostream>
#include <vector>

int main() {
  int width = 800, height = 600;
  cairo_surface_t *surface;
  cairo_t *cr;

  surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
  cr = cairo_create(surface);

  // Màu nền trắng
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

  // Thiết lập màu và độ dày cho đường
  cairo_set_source_rgb(cr, 0, 0, 0);  // màu đen
  cairo_set_line_width(cr, 2);

  // Vẽ polyline
  std::vector<std::pair<double, double>> points = {{100, 100}, {200, 200}, {300, 100}, {400, 200}};
  bool firstPoint = true;
  for (const auto &point : points) {
    if (firstPoint) {
      cairo_move_to(cr, point.first, point.second);
      firstPoint = false;
    } else {
      cairo_line_to(cr, point.first, point.second);
    }
  }

  // Áp dụng các đường vẽ
  cairo_stroke(cr);

  // Lưu hình ảnh
  cairo_surface_write_to_png(surface, "output.png");

  // Dọn dẹp
  cairo_destroy(cr);
  cairo_surface_destroy(surface);

  std::cout << "Hình ảnh đã được lưu thành 'output.png'" << std::endl;

  return 0;
}

#include <cairo.h>

#include <iostream>

int main() {
  cairo_surface_t *surface;
  cairo_t *cr;

  // Tạo surface với kích thước 400x400 và định dạng ARGB32
  surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 400, 400);
  cr = cairo_create(surface);

  // Màu nền trắng
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

  // Thiết lập màu vẽ là đen
  cairo_set_source_rgb(cr, 0, 0, 0);

  // Đặt vị trí và kích thước của điểm
  float x = 200, y = 200, point_size = 2;
  cairo_arc(cr, x, y, point_size, 0, 2 * M_PI);

  // Fill điểm
  cairo_fill(cr);

  // Lưu file ảnh PNG
  cairo_surface_write_to_png(surface, "point.png");

  // Dọn dẹp
  cairo_destroy(cr);
  cairo_surface_destroy(surface);

  std::cout << "Đã vẽ điểm và lưu vào file 'point.png'" << std::endl;

  return 0;
}
#include <cairo.h>

#include <iostream>
#include <vector>

int main() {
  cairo_surface_t *surface;
  cairo_t *cr;

  // Tạo surface với kích thước 400x400 và định dạng ARGB32
  surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 400, 400);
  cr = cairo_create(surface);

  // Màu nền trắng
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

  // Định nghĩa các điểm của polygon
  std::vector<std::pair<double, double>> points = {{100, 100}, {300, 100}, {300, 300}, {100, 300}};

  // Bắt đầu vẽ từ điểm đầu tiên
  cairo_move_to(cr, points[0].first, points[0].second);

  // Vẽ các cạnh của polygon
  for (size_t i = 1; i < points.size(); ++i) {
    cairo_line_to(cr, points[i].first, points[i].second);
  }

  // Đóng polygon
  cairo_close_path(cr);

  // Thiết lập màu để fill
  cairo_set_source_rgb(cr, 0, 0.5, 0);  // Màu xanh lá
  cairo_fill_preserve(cr);

  // Thiết lập màu viền và vẽ viền
  cairo_set_source_rgb(cr, 0, 0, 0);  // Màu đen
  cairo_stroke(cr);

  // Lưu file ảnh PNG
  cairo_surface_write_to_png(surface, "polygon.png");

  // Dọn dẹp
  cairo_destroy(cr);
  cairo_surface_destroy(surface);

  std::cout << "Đã vẽ polygon và lưu vào file 'polygon.png'" << std::endl;

  return 0;
}
