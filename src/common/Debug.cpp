#include "Debug.h"

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
    std::cout << "\t" << point.toString() << std::endl;
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
