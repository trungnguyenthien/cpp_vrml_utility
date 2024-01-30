#include "DrawBoard.h"

#include <climits>
#include <stdexcept>  // For std::invalid_argument

std::vector<std::pair<double, double>> makePair(std::vector<float> &source) {
  std::vector<std::pair<double, double>> result;
  cout << "makePair 1 " << source.size() << endl;
  // Nếu source không chẵn, loại bỏ số cuối cùng
  if (source.size() % 2 != 0) {
    source.pop_back();
  }
  cout << "makePair 2" << endl;

  // Nếu source rỗng, trả về vector rỗng
  if (source.empty()) {
    return result;
  }
  cout << "makePair 3" << endl;

  // Tạo các cặp từ các phần tử liên tiếp trong source
  for (size_t i = 0; i < source.size(); i += 2) {
    result.emplace_back(source[i], source[i + 1]);
  }
  cout << "makePair 4" << endl;
  cout << "END makePair" << endl;
  return result;
}

void printPairDouble(vector<pair<double, double>> &pair) {
  for (const auto &p : pair) {
    std::cout << "(" << p.first << ", " << p.second << ")\n";
  }
}

// void DBBoard::addObject(DBObject &obj) {
//   cout << typeid(obj).name() << endl;
//   DBPoint *point = dynamic_cast<DBPoint *>(&obj);
//   DBPolyline *pline = dynamic_cast<DBPolyline *>(&obj);
//   DBShape *shape = dynamic_cast<DBShape *>(&obj);
//   dbObjects.push_back(&obj);

//   if (shape != NULL) {
//     cout << "shape != NULL" << endl;
//     shape->calculateMinMax(zoom);
//     return;
//   }

//   if (pline != NULL) {
//     cout << "pline != NULL" << endl;
//     pline->calculateMinMax(zoom);
//     return;
//   }

//   if (point != NULL) {
//     cout << "point != NULL" << endl;
//     point->calculateMinMax(zoom);
//     return;
//   }
// }

void DBBoard::addObject(DBObject *obj) {
  cout << typeid(obj).name() << endl;
  DBPoint *point = dynamic_cast<DBPoint *>(obj);
  DBPolyline *pline = dynamic_cast<DBPolyline *>(obj);
  DBShape *shape = dynamic_cast<DBShape *>(obj);
  dbObjects.push_back(obj);

  if (shape != NULL) {
    cout << "shape != NULL" << endl;
    shape->calculateMinMax(zoom);
    return;
  }

  if (pline != NULL) {
    cout << "pline != NULL" << endl;
    pline->calculateMinMax(zoom);
    return;
  }

  if (point != NULL) {
    cout << "point != NULL" << endl;
    point->calculateMinMax(zoom);
    return;
  }
}

void DBBoard::render() {
  int bSize = boardSizeValue(size);
  int tx = bSize / 2;
  int ty = bSize / 2;

  cairo_surface_t *surface;
  cairo_t *cr;

  int width = bSize, height = bSize;
  cout << "Board Size " << width << "x" << height << endl;
  surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
  cr = cairo_create(surface);
  cout << "Step 1" << endl;
  // Màu nền backgroundColor
  cairo_set_source_rgb(cr, backgroundColor.redF(), backgroundColor.greenF(),
                       backgroundColor.blueF());
  cout << "Step 2" << endl;
  cairo_paint(cr);
  cout << "Step 3" << endl;
  for (DBObject *obj : dbObjects) {
    cout << "Step 3.1" << endl;
    obj->self_render(cr, tx, ty);
    cout << "Step 3.2" << endl;
  }

  cout << "Step 4" << endl;

  // Lưu file ảnh PNG
  string outputFilePNG = this->outFile + ".png";
  cout << "Step 5" << endl;
  cairo_surface_write_to_png(surface, outputFilePNG.c_str());
  cout << "Step 6" << endl;

  // Dọn dẹp
  cairo_destroy(cr);
  cout << "Step 7" << endl;
  cairo_surface_destroy(surface);
  cout << "Step 8" << endl;
}

int boardSizeValue(BOARD_SIZE value) {
  switch (value) {
    case BOARD_SIZE::_1_000:
      return 1000;
    case BOARD_SIZE::_5_000:
      return 5000;
    case BOARD_SIZE::_10_000:
      return 10000;
    case BOARD_SIZE::_20_000:
      return 20000;
    default:
      return 10000;
  }
}

int zoomValue(ZOOM_RATIO zoom, float value) {
  switch (zoom) {
    case ZOOM_RATIO::X1:
      return value;
    case ZOOM_RATIO::X10:
      return 10 * value;
    case ZOOM_RATIO::X100:
      return 100 * value;
  }
  return value;
}

void DBPoint::calculateMinMax(ZOOM_RATIO zoom) {
  x = zoomValue(zoom, x);
  y = zoomValue(zoom, y);
  cout << "Point.calculateMinMax " << x << " " << y << endl;
}

void DBShape::calculateMinMax(ZOOM_RATIO zoom) {
  for (int i = 0; i < xyList.size(); i++) {
    xyList[i] = zoomValue(zoom, xyList[i]);
  }
}

void DBPolyline::calculateMinMax(ZOOM_RATIO zoom) {
  for (int i = 0; i < xyList.size(); i++) {
    xyList[i] = zoomValue(zoom, xyList[i]);
  }
}

void DBPoint::self_render(cairo_t *cr, int tx, int ty) {
  cout << "DBPoint::self_render" << endl;
  cout << "DBPoint " << color.wrlColorStr() << endl;
  cout << "tx ty " << tx << " " << ty << endl;
  float x = this->x + tx, y = this->y + ty, point_size = 10;
  cout << "Draw ARC at " << x << ", " << y << endl;
  cairo_arc(cr, x, y, point_size, 0, 2 * M_PI);

  cairo_set_source_rgb(cr, color.redF(), color.greenF(), color.blueF());
  cairo_set_line_width(cr, 2);  // Line thickness
  // Fill điểm
  cairo_fill(cr);
  cairo_stroke(cr);
}

void DBPolyline::self_render(cairo_t *cr, int tx, int ty) {
  // Vẽ polyline
  std::vector<std::pair<double, double>> points = makePair(this->xyList);
  printPairDouble(points);
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

  // Thiết lập màu và độ dày cho đường
  cairo_set_source_rgb(cr, color.redF(), color.greenF(), color.blueF());
  cairo_set_line_width(cr, 10);  // Line thickness
  // Áp dụng các đường vẽ
  cairo_stroke(cr);
}

void DBPolyline::appendPoints(vector<Point> points) {
  for (const Point &point : points) {
    xyList.push_back(point.x);
    xyList.push_back(point.y);
  }
}

void DBShape::self_render(cairo_t *cr, int tx, int ty) {
  cout << " DBShape::self_render " << tx << " " << ty << endl;
  // Định nghĩa các điểm của polygon
  std::vector<std::pair<double, double>> points = makePair(this->xyList);

  printPairDouble(points);
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

  // Thiết lập màu viền và vẽ viền
  cairo_set_source_rgb(cr, color.redF(), color.greenF(), color.blueF());
  cairo_set_line_width(cr, 10);  // Line thickness
  cairo_stroke(cr);
}

void DBShape::appendPoints(vector<Point> points) {
  for (const Point &point : points) {
    xyList.push_back(point.x);
    xyList.push_back(point.y);
  }
}

// void test_cairo() {
//   DBBoard db;
//   db.size = BOARD_SIZE::_5_000;
//   db.zoom = ZOOM_RATIO::X10;
//   DBPoint point;
//   point.x = 10;
//   point.y = 30;
//   point.color = Color::parseColorFromHex("#FF4433");
//   db.addObject(point);

//   DBShape shape;
//   shape.xyList = {20, 30, 40, 20, 50, 50};
//   shape.color = Color::parseColorFromHex("#3366FF");
//   shape.fillColor = Color::parseColorFromHex("#E5E968");
//   db.addObject(shape);

//   db.render();
// }