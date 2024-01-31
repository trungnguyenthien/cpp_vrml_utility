#include "Model.h"

void printVectorPoints(vector<Point> points) {
  cout << "printVectorPoints { " << endl;
  for (const Point &point : points) {
    std::cout << point.toString() << std::endl;
  }
  cout << " } printVectorPoints" << endl;
}
