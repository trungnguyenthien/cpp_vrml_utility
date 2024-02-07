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

std::string &replaceAll(std::string &str, const std::string &from, const std::string &to);

// Thực hiện các bước xử lý sau:
// 1 - trim các khoảng trắng đầu và cuối {test}
// 2 = chia {test} bằng các khoảng trắng thành mảng {components}
// 3 - Kiểm tra và return TRUE nếu: mảng {components} có chứa đủ các chuỗi trong mảng {matchs} và
// theo đúng thứ tự trong {matchs}
bool isMatch(string test, vector<string> matchs);

#endif /* ED4F3CE3_8488_4B5C_8BB2_C42FB8AD02CE */
