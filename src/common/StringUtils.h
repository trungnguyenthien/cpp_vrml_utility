#ifndef ED4F3CE3_8488_4B5C_8BB2_C42FB8AD02CE
#define ED4F3CE3_8488_4B5C_8BB2_C42FB8AD02CE
#include <cctype>  // Cho hàm tolower
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> splitString(const string &input, const string &separator);
vector<int> splitInts(const string &input, const string &separator);
vector<float> splitFloats(const string &input, const string &separator);
string joinVector(const vector<int> &vec, const string &separator);
vector<string> splitStringBySpace(const string &input);

string toLowerCase(const string &str);
string trim(const string &str);
bool isNumber(const std::string &s);

#endif /* ED4F3CE3_8488_4B5C_8BB2_C42FB8AD02CE */
