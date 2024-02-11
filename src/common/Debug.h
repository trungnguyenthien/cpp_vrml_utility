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

void printVector2Int(const string &message, const vector<vector<int>> &contents);
void printVectorInt(const std::string &message, const std::vector<int> &contents);
void printVectorPoints(const vector<Point> &points);
void printSetOfSets(const std::set<std::set<int>> &setOfSets);
void printPairDouble(const vector<pair<double, double>> &pair);

// In Xmin, Xmax, Ymin, Ymax, Zmin, Zmax của {points}
void printMinMaxPoint(const vector<Point> &points);

// Mỗi vector<Point> là tập điểm có thứ tự trong Polygon
// In các điểm trong các polygon.
void printSetVectorPoint(const set<vector<Point>> &setPolygons);

// Cấu trúc Point(float x,y,z)
// Cấu trúc Segment(Point p1, p2)
// In tất cả Segment trong set<Segment>
void printSetSegment(const set<Segment> &segments);

void printVectorVectorPoint(const vector<vector<Point>> &points);

void printVectorVectorInt(const vector<vector<int>> &ints);

void printVectorString(const vector<string> &strings);

// Trả về tổng của các mã ascii của các ký tự trên chuỗi {text}
int sumAscii(const std::string &text);

// Return chuỗi mã ascii của text được cách nhau bằng dấu chấm `.`
string all_ascii(const std::string &text);

#endif  // COMMON_DEBUG_H_