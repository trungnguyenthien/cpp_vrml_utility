#include "VRML.h"

#include "FileUtils.h"
#include "StringUtils.h"

// Trả về true nếu str có format như dưới đây
// 0, 3, 2, 1, -1,
// 0, 3, 2, 1, -1
// 1 -5 -15,
// 1 -5 -15
// 0.883634 0.00439464 20.0001,
// 0.883634 0.00439464
bool isNumberArray(string str) {
  std::istringstream iss(str);
  std::string token;
  while (std::getline(iss, token, ',')) {
    token.erase(std::remove_if(token.begin(), token.end(), ::isspace), token.end());
    if (!token.empty() && !isNumber(token)) {
      return false;
    }
  }
  return true;
}

// Token có các định dạng như dưới đây
// 1 -5 -25,
// 11, -5, -25,
// 11 5 -25
// 1 5
// 1, -5,
// Chuỗi có thể có 2 hoặc 3 số. Nếu chuỗi chỉ có 2 số thì mặc định số thứ 3 là 0
// Tạo vector point bằng các giá trị đọc được trong token
Point parsePoint(string token) {
  std::istringstream iss(token);
  float x, y, z = 0;  // Mặc định z là 0
  char delim;

  iss >> x;
  if (iss.peek() == ',' || iss.peek() == ' ') {
    iss >> delim;  // Đọc dấu phẩy hoặc khoảng trắng
  }
  iss >> y;
  if (iss.peek() == ',' || iss.peek() == ' ') {
    iss >> delim;  // Đọc dấu phẩy hoặc khoảng trắng
  }
  iss >> z;  // Nếu không thể đọc thêm số, z sẽ giữ nguyên giá trị 0

  return Point(x, y, z);
}

// using FaceDef = vector<int>;

// Token có các định dạng như dưới đây
// 0, 3, 2, 1, -1,
// 4, 7, 3, 0, -1,
// 7, 6, 2, 3, -1,
// 6, 5, 1, 2, -1,
// 5 4 0 -1
// 5 4 0 -1,
// Số lượng number trong token không giới hạn.
// Các token có thể phân cách bằng khoảng trắng hoặc dấu phẩy (,)
// Loại bỏ số -1 cuối token, Trả về đối tượng FaceDef chứa các chữ số còn lại (theo đúng thứ tự)
FaceDef parseFace(string token) {
  FaceDef face;
  std::istringstream iss(token);
  int num;
  char delim;

  while (iss >> num) {
    if (num != -1) {
      face.push_back(num);
    }
    if (iss.peek() == ',' || iss.peek() == ' ') {
      iss >> delim;  // Đọc dấu phẩy hoặc khoảng trắng
    }
  }

  return face;
}

// Nếu token có định dạng sau
// {colorName} {floatRed} {floatGreen} {floatBlue}
// Ví dụ: "diffuseColor 0 0 1", "emissiveColor 0 0 1"
// Trả về vector chứa các giá trị floatRed, floatGreen, floatBlue
vector<float> parseColor(string token) {
  std::vector<float> colors;
  std::istringstream iss(token);
  std::string colorName;
  float red, green, blue;

  // Đọc tên màu
  if (!(iss >> colorName)) {
    return colors;  // Trả về vector rỗng nếu không đọc được tên màu
  }

  // Đọc ba giá trị màu
  if (iss >> red >> green >> blue) {
    colors.push_back(red);
    colors.push_back(green);
    colors.push_back(blue);
  }

  return colors;
}

// Loại bỏ tất cả khoảng trắng ở đầu và cuối line
// chia line thành nhiều phần bởi các ký tự: {,}, [,]
// Loại bỏ các phần tử empty
// return vector các chuỗi đã qua xử lý
vector<string> vrml2tokenStrings(string line) {
  std::vector<std::string> tokens;
  std::string trimmedLine = trim(line);

  std::string token;
  for (char ch : trimmedLine) {
    if (ch == '{' || ch == '}' || ch == '[' || ch == ']') {
      if (!token.empty()) {
        tokens.push_back(token);
        token.clear();
      }
      tokens.push_back(std::string(1, ch));
    } else if (!std::isspace(ch)) {
      token += ch;
    } else if (!token.empty()) {
      tokens.push_back(token);
      token.clear();
    }
  }

  if (!token.empty()) {
    tokens.push_back(token);
  }

  // Loại bỏ các phần tử rỗng
  tokens.erase(
      std::remove_if(tokens.begin(), tokens.end(), [](const std::string &s) { return s.empty(); }),
      tokens.end());

  return tokens;
}

vector<VrmlObject> read_vrml_file(string file) {
  vector<string> tokens;
  forEachLine(file, [&tokens](long index, string line) {
    vector<string> tokensInLine = vrml2tokenStrings(line);
    for (string token : tokensInLine) {
      tokens.push_back(token);
    }
  });

  vector<VrmlObject> vrmlObject;

  const int FLG_READ_END = 0;
  const int FLG_READ_START = 1;

  const int NUMBERSET_UNKNOWN = 0;
  const int NUMBERSET_POINT = 1;
  const int NUMBERSET_COORINDEX = 2;

  int number_set = NUMBERSET_UNKNOWN;
  int flag_read = FLG_READ_END;
  int count_shape = 0;

  vector<Point> points = {};
  vector<FaceDef> faces = {};

  for (string token : tokens) {
    if (token == "Shape") {
      flag_read = FLG_READ_END;
      number_set = NUMBERSET_UNKNOWN;
      continue;
    }

    if (token == "geometry IndexedFaceSet") {
      flag_read = FLG_READ_START;
      continue;
    }

    if (flag_read == FLG_READ_END) {
      continue;
    }

    if (token == "point") {
      number_set = NUMBERSET_POINT;
      continue;
    }

    if (token == "coordIndex") {
      number_set = NUMBERSET_COORINDEX;
      continue;
    }

    bool isNumbers = isNumberArray(token);

    if (isNumbers && number_set == NUMBERSET_POINT) {
      points.push_back(parsePoint(token));
      continue;
    }

    if (isNumbers && number_set == NUMBERSET_COORINDEX) {
      faces.push_back(parseFace(token));
      continue;
    }

    if (!isNumbers) {
    }
  }

  return vrmlObject;
}