#ifndef ED4F3CE3_8488_4B5C_8BB2_C42FB8AD02CE
#define ED4F3CE3_8488_4B5C_8BB2_C42FB8AD02CE
#include <cctype>  // Cho hàm tolower
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> splitString(const string &input, const string &separator);
vector<int> splitInts(const string &input);
vector<float> splitFloats(const string &input, const string &separator);
string joinVector(const vector<int> &vec, const string &separator);
vector<string> splitStringBySpace(const string &input);

string toLowerCase(const string &str);
string intToString(int number);
// Loại bỏ các ký tự khoảng trắng, tab, xuống dòng ở đầu và cuối {str}.
// Return chuỗi mới sau khi xử lý
// Không sử dụng isspace, hãy trim các ký tự sau "\t\n\v\f\r "
string trim(const string str);

bool isNumber(const std::string &s);

std::string &replaceAll(std::string &str, const std::string &from, const std::string &to);

// Thực hiện các bước xử lý sau:
// 1 - trim các khoảng trắng đầu và cuối {test}
// 2 = chia {test} bằng các khoảng trắng thành mảng {components}
// 3 - Kiểm tra và return TRUE nếu: mảng {components} có chứa đủ các chuỗi trong mảng {matchs} và
// theo đúng thứ tự trong {matchs}
bool isMatch(string test, vector<string> matchs);

int ascii(char c);

// Loại bỏ các ký tự NULL (có mã ascii là 0) trong chuỗi {str}
void removeNullCharacter(string &str);
#endif /* ED4F3CE3_8488_4B5C_8BB2_C42FB8AD02CE */
