#include "VRML.h"

#include "FileUtils.h"
#include "StringUtils.h"

#define PRINT_DEBUG_INFO(token, number_set, flag_read) \
  std::cout << "Token:" << token << "-- "              \
            << "Number Set:" << number_set << "-- "    \
            << "Flag Read:" << flag_read << std::endl;

// Trả về true nếu str có format như dưới đây
// 0, 3, 2, 1, -1,
// 0, 3, 2, 1, -1
// 1 -5 -15,
// 1 -5 -15
// 0.883634 0.00439464 20.0001,
// 0.883634 0.00439464
// Giữa các number có thể phân cách bằng khoảng trắng hoặc dấu phẩy (,)
// Cuối str có thể có dấu phẩy (,) hoặc không có
bool isNumberArray(string str) {
  std::istringstream iss(str);
  std::string token;

  while (iss >> token) {
    // Loại bỏ dấu phẩy nếu có
    if (token.back() == ',') {
      token.pop_back();
    }

    if (!isNumber(token)) {
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
  // cout << "Point parsePoint TOKEN " << token << endl;
  std::istringstream iss(token);
  std::vector<float> values;
  std::string number;

  while (std::getline(iss, number, ' ')) {
    if (!number.empty() && number.find(',') != std::string::npos) {
      number.erase(number.find(','), 1);
    }
    if (!number.empty()) {
      values.push_back(std::stof(number));
    }
  }

  float x = 0, y = 0, z = 0;

  if (values.size() >= 1)
    x = values[0];
  if (values.size() >= 2)
    y = values[1];
  if (values.size() >= 3)
    z = values[2];
  // cout << "Point parsePoint XYZ " << x << " " << y << " " << z << endl;
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
// Trả về đối tượng FaceDef chứa các chữ số còn lại (theo đúng thứ tự)
FaceDef parseFace(string token) {
  FaceDef face;
  std::istringstream iss(token);
  int num;
  char delim;

  while (iss >> num) {
    face.push_back(num);
    // Đọc qua dấu phẩy hoặc khoảng trắng
    if (iss.peek() == ',' || iss.peek() == ' ') {
      iss >> delim;
    }
  }

  return face;
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

/**
 Vector points chứa các vector (kiểu int như sau:)
 {0, 3, 2, 1, -1, 0, 3, 2, 1, -1,}
 {0, 3, 2, 1}
 {-1, 5, 1, 2}

 Hãy thực hiện các bước sau:
 Step 1: Join lại thành 1 vector {0, 3, 2, 1, -1, 0, 3, 2, 1, -1 0, 3, 2, 1, -1, 5, 1, 2}
 Step 2: Chia ra thành nhiều vector<int> tại các giá trị -1 (các vector<int> mới không chứa -1)
 Step 3: Trả về vector<vector<int>>
 */
vector<vector<int>> makeFaces(vector<vector<int>> points) {
  // Step 1: Join all sub-vectors into a single vector
  std::vector<int> joined;
  for (const auto &subVec : points) {
    joined.insert(joined.end(), subVec.begin(), subVec.end());
  }

  // Step 2: Split the joined vector at -1 and exclude -1 from sub-vectors
  std::vector<std::vector<int>> faces;
  std::vector<int> currentFace;
  for (int num : joined) {
    if (num != -1) {
      currentFace.push_back(num);
    } else {
      if (!currentFace.empty()) {
        faces.push_back(currentFace);
        currentFace.clear();
      }
    }
  }

  // Add the last face if there was no -1 at the end
  if (!currentFace.empty()) {
    faces.push_back(currentFace);
  }
  return faces;
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

// Step 1: Loại bỏ tất cả khoảng trắng ở đầu và cuối line
// Step 2: replace chuỗi "}" thành "} _END_OBJECT"
// Step 3: replace chuỗi "]" thành "] _END_ARRAY"
// Step 4: chia line thành nhiều phần bởi các ký tự: {,}, [,]
// Step 5: Loại bỏ các phần tử empty,  {,}, [,]
// Step 6: return vector các chuỗi đã qua xử lý
vector<string> vrml2tokenStrings(string line) {
  // Step 1
  trim(line);

  // Steps 2 and 3
  replaceAll(line, "}", "} _END_OBJECT");
  replaceAll(line, "]", "] _END_ARRAY");

  // Step 4: Split
  std::vector<std::string> tokens;
  std::istringstream iss(line);
  std::string token;
  const std::string delimiters = "{}[]";
  size_t prev = 0, next = 0;
  while ((next = line.find_first_of(delimiters, prev)) != std::string::npos) {
    if (next - prev != 0) {
      token = line.substr(prev, next - prev);
      token = trim(token);  // Trim each token
      if (!token.empty()) {
        tokens.push_back(token);
      }
    }
    prev = next + 1;
  }
  if (prev < line.size()) {
    token = line.substr(prev);
    token = trim(token);  // Trim last token
    if (!token.empty()) {
      tokens.push_back(token);
    }
  }

  return tokens;
}

VrmlFaceSet::VrmlFaceSet() {}

string VrmlFaceSet::debug_description() {
  ostringstream oss;
  oss << "\nSTART--------- VrmlFaceSet -------------------------------------------------------\n";
  // Mô tả các điểm
  oss << "Points: " << points.size() << endl;
  for (const auto &point : points) {
    oss << point.toString() << "\n";
  }

  // Mô tả các mặt
  oss << "Faces: " << faces.size() << "\n";
  for (const auto &face : faces) {
    for (int index : face) {
      oss << index << "\t";
    }
    oss << "\n";
  }
  oss << "END --------- VrmlFaceSet -------------------------------------------------------\n";
  return oss.str();
}

vector<VrmlObject *> read_vrml_file(string file) {
  vector<string> tokens;
  forEachLine(file, [&tokens](long index, string line) {
    vector<string> tokensInLine = vrml2tokenStrings(line);
    for (string token : tokensInLine) {
      tokens.push_back(token);
    }
  });

  const string TOKEN_END_ARRAY = "_END_ARRAY";
  const string TOKEN_END_OBJECT = "_END_OBJECT";

  const int FLG_READ_END = 0;
  const int FLG_READ_START = 1;

  const int NUMBERSET_UNKNOWN = 0;
  const int NUMBERSET_POINT = 1;
  const int NUMBERSET_COORINDEX = 2;

  int number_set = NUMBERSET_UNKNOWN;
  int flag_read = FLG_READ_END;
  int count_shape = 0;

  vector<VrmlObject *> vrmlObjects;
  vector<Point> points = {};
  vector<FaceDef> faces = {};

  for (string token : tokens) {
    if (token == "geometry IndexedFaceSet") {
      flag_read = FLG_READ_START;
      number_set = NUMBERSET_UNKNOWN;

      PRINT_DEBUG_INFO(token, number_set, flag_read);
      continue;
    }

    if (flag_read == FLG_READ_END) {
      continue;
    }

    if (token == "point") {
      number_set = NUMBERSET_POINT;
    } else if (token == "coordIndex") {
      number_set = NUMBERSET_COORINDEX;
    }

    PRINT_DEBUG_INFO(token, number_set, flag_read);

    if (number_set == NUMBERSET_UNKNOWN) {
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

    if (token == TOKEN_END_OBJECT && !faces.empty() && !points.empty()) {
      VrmlFaceSet *faceSet = new VrmlFaceSet();
      faceSet->points = points;
      faceSet->faces = makeFaces(faces);
      vrmlObjects.push_back(faceSet);

      points = {};
      faces = {};
      flag_read = FLG_READ_END;
      number_set = NUMBERSET_UNKNOWN;
    }

    PRINT_DEBUG_INFO(token, number_set, flag_read);
  }

  for (VrmlObject *vrml : vrmlObjects) {
    VrmlFaceSet *faceSet = dynamic_cast<VrmlFaceSet *>(vrml);
    if (faceSet != NULL) {
      cout << faceSet->debug_description() << endl;
    }
  }

  return vrmlObjects;
}

VrmlObject::VrmlObject() : diffuseColor(0, 0, 0), emissiveColor(0, 0, 0) {}
