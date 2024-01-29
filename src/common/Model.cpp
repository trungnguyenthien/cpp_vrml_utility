#include "Model.h"

void printVectorPoints(vector<Point> points) {
  for (const Point &point : points) {
    std::cout << point.toString() << std::endl;
  }
}
