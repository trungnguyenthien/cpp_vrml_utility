#include "Debug.h"

#include <iostream>
#include <limits>
#include <vector>

void printMinMaxPoint(const std::vector<Point> points) {
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

void printPairDouble(vector<pair<double, double>> &pair) {
  for (const auto &p : pair) {
    std::cout << "(" << p.first << ", " << p.second << ")\n";
  }
}

void printVectorPoints(vector<Point> points) {
  cout << "printVectorPoints { " << endl;
  for (const Point &point : points) {
    std::cout << "  " << point.toString() << std::endl;
  }
  cout << " }" << endl;
}

void printVector2Int(string message, vector<vector<int>> contents) {
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
