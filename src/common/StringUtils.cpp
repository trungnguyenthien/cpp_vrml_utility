#include "StringUtils.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>

vector<string> splitString(const string &input, const string &separator) {
  vector<string> result;
  string::size_type start = 0;
  string::size_type end = 0;

  while ((end = input.find(separator, start)) != string::npos) {
    result.push_back(input.substr(start, end - start));
    start = end + separator.length();
  }
  result.push_back(input.substr(start));

  return result;
}

vector<int> splitInts(const string &input, const string &separator) {
  vector<int> result;
  string::size_type start = 0;
  string::size_type end = 0;

  while ((end = input.find(separator, start)) != string::npos) {
    result.push_back(stoi(input.substr(start, end - start)));
    start = end + separator.length();
  }
  result.push_back(stoi(input.substr(start)));

  return result;
}

vector<float> splitFloats(const string &input, const string &separator) {
  vector<float> result;
  string::size_type start = 0;
  string::size_type end = 0;

  while ((end = input.find(separator, start)) != string::npos) {
    string str = input.substr(start, end - start);
    // cout << "str1-" << str << endl;
    result.push_back(stof(str));
    start = end + separator.length();
  }
  string str = input.substr(start);
  if (!str.empty()) {
    // cout << "str2-" << str << endl;
    result.push_back(stof(str));
  }

  return result;
}

// Hàm nhận một vector<int> và trả về một chuỗi
string joinVector(const vector<int> &vec, const string &separator) {
  ostringstream oss;
  for (size_t i = 0; i < vec.size(); ++i) {
    oss << vec[i];
    if (i < vec.size() - 1) {
      oss << separator;
    }
  }
  return oss.str();
}

vector<string> splitStringBySpace(const string &inputString) {
  vector<std::string> tokens;
  istringstream iss(inputString);

  // Sử dụng istringstream để tách chuỗi theo khoảng trắng và tab
  string token;
  while (iss >> token) {
    tokens.push_back(token);
  }

  // // In các phần con đã tách được
  // for (const string &str : tokens)
  // {
  //     std::cout << str << std::endl;
  // }
  return tokens;
}

std::string toLowerCase(const std::string &str) {
  std::string lowerCaseStr = str;  // Tạo một bản sao của chuỗi đầu vào
  for (char &c : lowerCaseStr) {
    c = std::tolower(static_cast<unsigned char>(c));  // Chuyển từng ký tự sang chữ thường
  }
  return lowerCaseStr;  // Trả về chuỗi mới đã được chuyển đổi
}

std::string trim(const std::string &str) {
  auto is_not_space = [](unsigned char ch) { return !std::isspace(ch); };

  size_t first = std::find_if(str.begin(), str.end(), is_not_space) - str.begin();
  size_t last = std::find_if(str.rbegin(), str.rend(), is_not_space).base() - str.begin();

  if (first == str.size() || last == 0)
    return "";

  return str.substr(first, last - first);
}

bool isNumber(const std::string &s) {
  std::istringstream iss(s);
  double val;
  char ch;
  if (!(iss >> val)) {
    return false;  // không phải số
  }
  if (iss >> ch) {
    return false;  // có ký tự không phải là số sau số
  }
  return true;
}

std::string &replaceAll(std::string &str, const std::string &from, const std::string &to) {
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length();
  }
  return str;
}