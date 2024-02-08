#ifndef COMMON_DEBUG_H_
#define COMMON_DEBUG_H_

#include <climits>
#include <functional>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "Model.h"

using namespace std;

void printVector2Int(string message, vector<vector<int>> contents);
void printVectorInt(const std::string &message, const std::vector<int> &contents);
void printVectorPoints(vector<Point> points);
void printSetOfSets(const std::set<std::set<int>> &setOfSets);
void printPairDouble(vector<pair<double, double>> &pair);

// In Xmin, Xmax, Ymin, Ymax, Zmin, Zmax của {points}
void printMinMaxPoint(vector<Point> points);

// Mỗi vector<Point> là tập điểm có thứ tự trong Polygon
// In các điểm trong các polygon.
void printSetVectorPoint(set<vector<Point>> setPolygons);

// Cấu trúc Point(float x,y,z)
// Cấu trúc Segment(Point p1, p2)
// In tất cả Segment trong set<Segment>
void printSetSegment(set<Segment> segments);

void printVectorVectorPoint(vector<vector<Point>> points);

void printVectorVectorInt(vector<vector<int>> ints);

#endif  // COMMON_DEBUG_H_