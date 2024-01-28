#ifndef FUNCTION_DRAWBOARD_H_
#define FUNCTION_DRAWBOARD_H_

#include <cairo/cairo.h>

#include <climits>
#include <initializer_list>
#include <utility>
#include <vector>

#include "../common/Model.h"

using namespace std;

enum class ZOOM_RATIO { X1, X10, X100, X1000 };
int zoomValue(int zoomValue, float value);

float min_value(vector<float> values);
float max_value(vector<float> values);
float min_value(float value1, float value2);
float max_value(float value1, float value2);

// source là vector gồm số chẵn các float.
// Nếu source không chẵn thì loại bỏ số cuối cùng.
// Nếu source empty thì return vector empty
// Cứ 2 float liên tiếp nhau thì nhóm thành 1 cặp (pair)
// Trả về vector<pair<double, double>>
vector<pair<double, double>> makePair(vector<float> source);

struct DBObject {
  Color color = Color(0, 0, 0);
  int thickness = 1;
  DBObject() = default;
  int xmin = INT_MAX, ymin = INT_MAX, xmax = INT_MIN, ymax = INT_MIN;
  virtual void calculateMinMax(ZOOM_RATIO zoom){};
  virtual void self_render(cairo_t *cr, int tx, int ty){};
};

struct DBPoint : public DBObject {
  float x = 0, y = 0;
  DBPoint() = default;
  void calculateMinMax(ZOOM_RATIO zoom) override;
  void self_render(cairo_t *cr, int tx, int ty) override;
};

struct DBShape : public DBPoint {
  vector<float> xyList;
  Color fillColor = Color(0, 0, 0);
  DBShape() = default;
  void calculateMinMax(ZOOM_RATIO zoom) override;
  void self_render(cairo_t *cr, int tx, int ty) override;
};

struct DBPolyline : public DBPoint {
  vector<float> xyList;
  DBPolyline() = default;
  void calculateMinMax(ZOOM_RATIO zoom) override;
  void self_render(cairo_t *cr, int tx, int ty) override;
};

struct DBBoard {
  DBBoard() = default;
  int margin = 20;
  ZOOM_RATIO zoom = ZOOM_RATIO::X1000;
  Color backgroundColor = Color(100, 150, 200);
  string outFile = "DBBoard";

  vector<DBObject *> dbObjects;

  void addObject(DBObject &obj);

  void render();
};

#endif  // FUNCTION_DRAWBOARD_H_