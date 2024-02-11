#include "StringUtils.h"

#include <algorithm>
#include <algorithm>  // Cho std::remove
#include <boost/algorithm/string/trim.hpp>
#include <cctype>
#include <cstring>  // Cho strtok và strlen
#include <initializer_list>
#include <iostream>
#include <locale>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "Debug.h"

bool isFloat(const std::string &str) {
  std::istringstream stream(str);
  float number;
  stream >> number;  // Cố gắng đọc một số thực từ chuỗi
  if (stream.fail()) {
    return false;  // Nếu đọc thất bại, chuỗi không phải là số thực
  }
  // Kiểm tra xem có còn ký tự nào không được đọc không
  // Nếu có, chuỗi có thể chứa ký tự không phải số sau số thực
  char remaining;
  if (stream >> remaining) {
    return false;  // Còn ký tự không được đọc, chuỗi không chỉ chứa số thực
  }
  return true;  // Đã đọc hết chuỗi và không gặp lỗi, chuỗi là một số thực
}

bool isInt(const std::string &str) {
  if (str.empty()) {
    return false;  // Trả về false nếu chuỗi rỗng
  }
  std::istringstream stream(str);
  int number;
  stream >> number;
  return stream.eof() && !stream.fail();  // Kiểm tra đã đọc hết chuỗi và không có lỗi
}

std::vector<std::string> splitString(const std::string &str) {
  std::vector<std::string> components;

  // Tạo bản sao của chuỗi đầu vào vì strtok sẽ thay đổi chuỗi
  char *cstr = new char[str.length() + 1];
  std::strcpy(cstr, str.c_str());

  // Delimiters là khoảng trắng và dấu phẩy
  const char *delimiters = " ,";

  // Sử dụng strtok để tách chuỗi
  char *token = strtok(cstr, delimiters);
  while (token != nullptr) {
    components.push_back(token);          // Thêm token vào vector
    token = strtok(nullptr, delimiters);  // Tiếp tục tách chuỗi tiếp theo
  }

  delete[] cstr;  // Giải phóng bộ nhớ đã cấp phát

  return components;
}

void removeNullCharacter(std::string &str) {
  // std::remove sẽ dời tất cả các ký tự không phải NULL về đầu chuỗi
  // và trả về một iterator đến phần tử "mới" đầu tiên có giá trị NULL
  // sau đó, phương thức erase sẽ xóa phần từ đó iterator trở đi
  str.erase(std::remove(str.begin(), str.end(), '\0'), str.end());
}

int ascii(char c) {
  return static_cast<int>(c);  // Ép kiểu ký tự sang số nguyên để lấy mã ASCII
}

bool isEqual(const std::string &str1, const std::string &str2) {
  // Nếu độ dài của hai chuỗi khác nhau, chúng không bằng nhau
  if (str1.length() != str2.length()) {
    // cout << "str1.length() != str2.length()" << std::endl;
    return false;
  }

  // Duyệt qua từng ký tự và so sánh
  for (size_t i = 0; i < str1.length(); ++i) {
    if (ascii(str1[i]) != ascii(str2[i])) {
      return false;  // Nếu tìm thấy một ký tự không khớp, trả về false
    }
  }

  // Tất cả các ký tự đều khớp, trả về true
  return true;
}

bool isMatch(string test, vector<std::string> matchs) {
  // Bước 1: Trim khoảng trắng đầu và cuối chuỗi
  test = trim(test);

  // Bước 2: Chia chuỗi {test} thành các thành phần dựa trên khoảng trắng
  std::vector<std::string> components;
  std::stringstream ss(test);
  std::string component;
  while (ss >> component) {
    components.push_back(component);
  }

  for (auto text : components) {
    if (matchs.size() == 0) {
      return true;
    }

    if (isEqual(matchs[0], text)) {
      matchs.erase(matchs.begin());
    }
  }

  return matchs.size() == 0;
}

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

vector<int> splitInts(const string &input) {
  vector<int> result;
  vector<string> components = splitString(input);
  // cout << "splitInts components";
  printVectorString(components);
  for (auto com : components) {
    if (isInt(com)) {
      result.push_back(stoi(com));
    }
  }

  return result;
}

vector<float> splitFloats(const string &input, const string &separator) {
  vector<float> result;
  string::size_type start = 0;
  string::size_type end = 0;

  while ((end = input.find(separator, start)) != string::npos) {
    string str = input.substr(start, end - start);
    result.push_back(stof(str));
    start = end + separator.length();
  }
  string str = input.substr(start);
  if (!str.empty()) {
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

bool isSpace(char test) {
  vector<int> whitespaces = {9, 10, 11, 13, 32, 0};
  int testInt = static_cast<int>(test);
  for (auto c : whitespaces) {
    if (c == testInt) {
      return true;
    }
  }

  // cout << "Not Space:" << test << "...ascii=" << testInt << endl;
  return false;
}

// Return chuỗi sau khi cắt các ký tự khoảng trắng ở đầu và cuối chuỗi {str}
std::string trim(const std::string str) {
  string result;

  int start = 0;
  int end = str.size() - 1;

  for (; start < str.size() - 1; start++) {
    if (!isSpace(str[start])) {
      break;
    }
  }

  for (; end >= 0 && end != start; end--) {
    if (!isSpace(str[end])) {
      break;
    }
  }

  if (end <= start) {
    return "";
  }

  for (int i = start; i <= end; i++) {
    result.push_back(str[i]);
  }
  // cout << "StartEnd:" << start << "->" << end << endl;
  // cout << "TRIMRESULT:" << result << endl;
  return result;
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