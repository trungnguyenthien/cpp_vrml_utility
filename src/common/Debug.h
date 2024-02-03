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

#endif  // COMMON_DEBUG_H_