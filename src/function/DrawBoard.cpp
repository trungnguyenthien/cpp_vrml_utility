#include "DrawBoard.h"

#include <climits>
#include <stdexcept>  // For std::invalid_argument

void test_cairo2() {
  cairo_surface_t *surface;
  cairo_t *cr;

  int width = 500, height = 500;

  // Tạo surface để vẽ
  surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
  cr = cairo_create(surface);

  // Màu nền trắng
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

  // Thiết lập màu và độ dày cho đường vẽ
  cairo_set_source_rgb(cr, 0, 0, 0);  // Màu đen
  cairo_set_line_width(cr, 2);

  // Vẽ hình vuông
  cairo_rectangle(cr, 50, 50, 400, 400);
  cairo_stroke(cr);

  // Lưu hình ảnh dưới dạng PNG
  cairo_surface_write_to_png(surface, "square.png");

  // Dọn dẹp
  cairo_destroy(cr);
  cairo_surface_destroy(surface);

  std::cout << "Đã vẽ hình vuông và lưu vào 'square.png'" << std::endl;
}

std::vector<std::pair<double, double>> makePair(std::vector<float> source) {
  std::vector<std::pair<double, double>> result;

  // Nếu source không chẵn, loại bỏ số cuối cùng
  if (source.size() % 2 != 0) {
    source.pop_back();
  }

  // Nếu source rỗng, trả về vector rỗng
  if (source.empty()) {
    return result;
  }

  // Tạo các cặp từ các phần tử liên tiếp trong source
  for (size_t i = 0; i < source.size(); i += 2) {
    result.emplace_back(source[i], source[i + 1]);
  }

  return result;
}

void DBBoard::addObject(DBObject &obj) {
  cout << typeid(obj).name() << endl;
  DBPoint *point = dynamic_cast<DBPoint *>(&obj);
  DBPolyline *pline = dynamic_cast<DBPolyline *>(&obj);
  DBShape *shape = dynamic_cast<DBShape *>(&obj);

  if (point != NULL) {
    cout << "point != NULL" << endl;
    point->calculateMinMax(zoom);
  } else if (pline != NULL) {
    cout << "pline != NULL" << endl;
    pline->calculateMinMax(zoom);
  } else if (shape != NULL) {
    cout << "shape != NULL" << endl;
    shape->calculateMinMax(zoom);
  }

  dbObjects.push_back(&obj);
}

void DBBoard::render() {
  int xmin = INT_MAX, ymin = INT_MAX, xmax = INT_MIN, ymax = INT_MIN;
  for (DBObject *obj : dbObjects) {
    xmin = min_value(xmin, obj->xmin);
    xmax = max_value(xmax, obj->xmax);
    ymin = min_value(ymin, obj->ymin);
    ymax = max_value(ymax, obj->ymax);
  }
  cout << "XY MINMAX " << xmin << " " << xmax << " " << ymin << " " << ymax << endl;
  int tx = xmin + margin;
  int ty = ymin + margin;

  xmin -= margin;
  xmax += margin;
  ymin -= margin;
  ymax += margin;

  cairo_surface_t *surface;
  cairo_t *cr;

  int width = xmax - xmin, height = ymax - ymin;
  cout << "Board Size " << width << "x" << height << endl;
  surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
  cr = cairo_create(surface);

  // Màu nền backgroundColor
  cairo_set_source_rgb(cr, backgroundColor.redF(), backgroundColor.greenF(),
                       backgroundColor.blueF());
  cairo_paint(cr);

  for (DBObject *obj : dbObjects) {
    obj->self_render(cr, tx, ty);

    // DBPoint *point = dynamic_cast<DBPoint *>(obj);
    // DBPolyline *pline = dynamic_cast<DBPolyline *>(obj);
    // DBShape *shape = dynamic_cast<DBShape *>(obj);

    // if (point != NULL) {
    //   point->self_render(cr, tx, ty);
    // } else if (pline != NULL) {
    //   pline->self_render(cr, tx, ty);
    // } else if (shape != NULL) {
    //   shape->self_render(cr, tx, ty);
    // }
  }

  // Lưu file ảnh PNG
  string outputFilePNG = this->outFile + ".png";
  cairo_surface_write_to_png(surface, outputFilePNG.c_str());

  // Dọn dẹp
  cairo_destroy(cr);
  cairo_surface_destroy(surface);
}

int zoomValue(int zoomValue, float value) { return 0; }

float min_value(vector<float> values) {
  if (values.empty()) {
    return INT_MIN;
  }
  return *std::min_element(values.begin(), values.end());
}

float max_value(vector<float> values) {
  if (values.empty()) {
    return INT_MAX;
  }
  return *std::max_element(values.begin(), values.end());
}

float min_value(float value1, float value2) { return (value1 < value2) ? value1 : value2; }

float max_value(float value1, float value2) { return (value1 > value2) ? value1 : value2; }

int zoomValue(ZOOM_RATIO zoom, float value) {
  switch (zoom) {
    case ZOOM_RATIO::X1:
      return value;
    case ZOOM_RATIO::X10:
      return 10 * value;
    case ZOOM_RATIO::X100:
      return 100 * value;
    case ZOOM_RATIO::X1000:
      return 1000 * value;
  }
  return value;
}

void DBPoint::calculateMinMax(ZOOM_RATIO zoom) {
  x *= zoomValue(zoom, x);
  y *= zoomValue(zoom, y);
  cout << "Point.calculateMinMax " << x << " " << y << endl;
  xmin = x;
  xmax = x;
  ymin = y;
  ymax = y;
}

void DBShape::calculateMinMax(ZOOM_RATIO zoom) {
  vector<float> xPoints, yPoints;

  for (int i = 0; i < xyList.size(); i++) {
    xyList[i] = zoomValue(zoom, xyList[i]);

    if (i % 2 == 0) {
      xPoints.push_back(xyList[i]);
    } else {
      yPoints.push_back(xyList[i]);
    }
  }

  xmin = min_value(xPoints);
  xmax = max_value(xPoints);
  ymin = min_value(yPoints);
  ymax = max_value(yPoints);
}

void DBPolyline::calculateMinMax(ZOOM_RATIO zoom) {
  vector<float> xPoints, yPoints;

  for (int i = 0; i < xyList.size(); i++) {
    xyList[i] = zoomValue(zoom, xyList[i]);

    if (i % 2 == 0) {
      xPoints.push_back(xyList[i]);
    } else {
      yPoints.push_back(xyList[i]);
    }
  }

  xmin = min_value(xPoints);
  xmax = max_value(xPoints);
  ymin = min_value(yPoints);
  ymax = max_value(yPoints);
}

void DBPoint::self_render(cairo_t *cr, int tx, int ty) {
  cout << "DBPoint::self_render" << endl;
  cout << "DBPoint " << color.wrlColorStr() << endl;
  cairo_set_source_rgb(cr, color.redF(), color.greenF(), color.blueF());
  cairo_set_line_width(cr, 2);  // Line thickness
  cout << "tx ty " << tx << " " << ty << endl;
  float x = this->x + tx, y = this->y + ty, point_size = 5;
  cout << "Draw ARC at " << x << ", " << y << endl;
  cairo_arc(cr, x, y, point_size, 0, 2 * M_PI);

  // Fill điểm
  cairo_fill(cr);
  cairo_stroke(cr);
}

void DBPolyline::self_render(cairo_t *cr, int tx, int ty) {
  // Thiết lập màu và độ dày cho đường
  cairo_set_source_rgb(cr, color.redF(), color.greenF(), color.blueF());
  cairo_set_line_width(cr, 2);  // Line thickness

  // Vẽ polyline
  std::vector<std::pair<double, double>> points = makePair(this->xyList);
  bool firstPoint = true;
  for (const auto &point : points) {
    if (firstPoint) {
      // Di chuyển (MOVE) đến điểm đầu tiên
      cairo_move_to(cr, point.first + tx, point.second + ty);
      firstPoint = false;
    } else {
      // Vẽ line đến điểm tiếp theo
      cairo_line_to(cr, point.first + tx, point.second + ty);
    }
  }

  // Áp dụng các đường vẽ
  cairo_stroke(cr);
}

void DBShape::self_render(cairo_t *cr, int tx, int ty) {
  // Thiết lập màu viền và vẽ viền
  cairo_set_source_rgb(cr, color.redF(), color.greenF(), color.blueF());
  cairo_set_line_width(cr, 2);  // Line thickness

  // Định nghĩa các điểm của polygon
  std::vector<std::pair<double, double>> points = makePair(this->xyList);

  // Bắt đầu vẽ từ điểm đầu tiên
  cairo_move_to(cr, points[0].first + tx, points[0].second + ty);

  // Vẽ các cạnh của polygon
  for (size_t i = 1; i < points.size(); ++i) {
    cairo_line_to(cr, points[i].first + tx, points[i].second + ty);
  }

  // Đóng polygon
  cairo_close_path(cr);

  // Thiết lập màu để fill
  cairo_set_source_rgb(cr, fillColor.redF(), fillColor.greenF(), fillColor.blueF());
  cairo_fill_preserve(cr);
  cairo_stroke(cr);
}