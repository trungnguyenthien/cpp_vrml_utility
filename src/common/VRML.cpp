#include "VRML.h"

#include <algorithm>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Debug.h"
#include "FileUtils.h"
#include "StringUtils.h"

// Hàm kiểm tra sự giao nhau của hai đoạn thẳng
bool doIntersect(Point p1, Point q1, Point p2, Point q2) {
  // Hàm phụ để kiểm tra sự sắp xếp theo chiều của ba điểm
  // và kiểm tra xem điểm q có nằm trên đoạn thẳng pr không
  auto onSegment = [](Point p, Point q, Point r) {
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) && q.y <= max(p.y, r.y) &&
        q.y >= min(p.y, r.y))
      return true;
    return false;
  };

  // Hàm phụ để tìm hướng xoay: trả về các giá trị khác nhau
  // cho các trường hợp Colinear (0), Clockwise (1) và Counterclockwise (2)
  auto orientation = [](Point p, Point q, Point r) {
    float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0)
      return 0;                // colinear
    return (val > 0) ? 1 : 2;  // clock or counterclock wise
  };

  // Kiểm tra bốn điều kiện của sự giao nhau
  int o1 = orientation(p1, q1, p2);
  int o2 = orientation(p1, q1, q2);
  int o3 = orientation(p2, q2, p1);
  int o4 = orientation(p2, q2, q1);

  // Trường hợp chính: các điểm cắt nhau không phải là đầu mút
  if (o1 != o2 && o3 != o4)
    return true;

  // Trường hợp đặc biệt: các điểm colinear và giao nhau tại đầu mút
  // Được loại bỏ vì yêu cầu của câu hỏi

  return false;  // Không có trường hợp giao nhau
}

// {polyPoints} là tập các Point(float x, float y, float z) theo thứ tự tạo thành 1 polygon
// Return TRUE nếu polygon có 2 cạnh bất kỳ cắt nhau tại điểm giữa mỗi cạnh.
// Chú ý: điểm cắt nhau phải khác điểm đầu và cuối mỗi cạnh
bool isSelfCross(vector<Point> polyPoints) {
  int n = polyPoints.size();
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      // Bỏ qua kiểm tra giao nhau cho các cạnh kề nhau và cặp cạnh cuối cùng với cạnh đầu tiên
      if (abs(i - j) > 1 && i != 0 && j != n - 1) {
        if (doIntersect(polyPoints[i], polyPoints[(i + 1) % n], polyPoints[j],
                        polyPoints[(j + 1) % n])) {
          return true;
        }
      }
    }
  }
  return false;
}

bool isSelfCross(set<vector<Point>> setPolyPoints) {
  for (auto poly : setPolyPoints) {
    if (isSelfCross(poly)) {
      return true;
    }
  }

  return false;
}

bool is2PolyCross(vector<Point> poly1, vector<Point> poly2) {
  if (poly1.size() < 2 || poly2.size() < 2) {
    return false;
  }
  poly1.push_back(poly1[0]);
  poly2.push_back(poly2[0]);

  for (int i1 = 0; i1 < poly1.size() - 1; i1++) {
    for (int i2 = 0; i2 < poly2.size() - 1; i2++) {
      if (doIntersect(poly1[i1], poly1[i1 + 1], poly2[i2], poly2[i2 + 1])) {
        return true;
      }
    }
  }

  return false;
}

bool is2PolyCross(set<vector<Point>> setPoly) {
  vector<vector<Point>> vectorPoly;
  for (auto poly : setPoly) {
    vectorPoly.push_back(poly);
  }

  for (int i = 0; i < vectorPoly.size() - 1; i++) {
    if (is2PolyCross(vectorPoly[i], vectorPoly[i + 1])) {
      return true;
    }
  }

  return false;
}

// Nếu {vertices1} và {vertices2} có 1 set<int> giống nhau thì return true
bool hasCommonEdge(set<set<int>> vertices1, set<set<int>> vertices2) {
  for (set<int> set1 : vertices1) {
    for (set<int> set2 : vertices2) {
      if (set1 == set2)
        return true;
    }
  }
  return false;
}

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
  cout << "Point parsePoint XYZ " << x << " " << y << " " << z << endl;
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
  FaceDef face = splitInts(token);
  // std::istringstream iss(token);
  // int num;
  // char delim;

  // while (iss >> num) {
  //   face.push_back(num);
  //   // Đọc qua dấu phẩy hoặc khoảng trắng
  //   if (iss.peek() == ',' || iss.peek() == ' ') {
  //     iss >> delim;
  //   }
  // }
  cout << "parseFace Token:" << token << endl;
  // cout << "Face Parse Face: ";
  printVectorInt("Face: ", face);

  return face;
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
        auto multiFaces = splitFace3P(currentFace);
        for (auto f : multiFaces) {
          faces.push_back(f);
        }
        // faces.push_back(currentFace);
        currentFace.clear();
      }
    }
  }

  // Add the last face if there was no -1 at the end
  if (!currentFace.empty()) {
    auto multiFaces = splitFace3P(currentFace);
    for (auto f : multiFaces) {
      faces.push_back(f);
    }
  }
  return faces;
}

vector<vector<int>> splitFace3P(vector<int> points) {
  vector<vector<int>> output;
  for (int i = 1; i < points.size() - 1; i++) {
    int p0 = points[0];
    int p1 = points[i];
    int p2 = points[i + 1];
    output.push_back({p0, p1, p2});
  }
  return output;
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
vector<string> vrml2tokenStrings(string &line) {
  // Step 1
  // cout << "line1:" << line << endl;
  auto line2 = trim(line);
  // cout << "line2:" << line2 << endl;

  // Steps 2 and 3
  replaceAll(line2, "}", "} _END_OBJECT");
  replaceAll(line2, "]", "] _END_ARRAY");

  // Step 4: Split
  std::vector<std::string> tokens;
  std::istringstream iss(line2);
  std::string token;
  const std::string delimiters = "{}[]";
  size_t prev = 0, next = 0;
  while ((next = line2.find_first_of(delimiters, prev)) != std::string::npos) {
    if (next - prev != 0) {
      token = line2.substr(prev, next - prev);
      token = trim(token);  // Trim each token
      if (!token.empty()) {
        tokens.push_back(token);
      }
    }
    prev = next + 1;
  }
  if (prev < line2.size()) {
    token = line2.substr(prev);
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
  int pointIndex = 0;
  for (const auto &point : points) {
    oss << pointIndex << "_" << point.toString();
    pointIndex++;
    if (pointIndex % 4 == 0) {
      oss << "\n";
    } else {
      oss << "\t\t";
    }
  }

  // Mô tả các mặt
  oss << "Faces: " << faces.size() << "\n";
  int fIndex = 0;
  for (const auto &face : faces) {
    oss << "(";
    for (int fi = 0; fi < face.size(); fi++) {
      oss << face[fi];
      if (fi != face.size() - 1) {
        oss << " ";
      }
    }

    oss << ")";
    fIndex++;
    if (fIndex % 4 == 0) {
      oss << "\n";
    } else {
      oss << "\t\t";
    }
  }
  oss << "\nEND --------- VrmlFaceSet -------------------------------------------------------\n";
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
    // geometry PointSet
    if (isMatch(token, {"geometry", "IndexedFaceSet"}) ||
        isMatch(token, {"geometry", "PointSet"})) {
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

    // PRINT_DEBUG_INFO(token, number_set, flag_read);

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

    // PRINT_DEBUG_INFO(token, number_set, flag_read);
  }

  cout << "vrmlObjects.size()=" << vrmlObjects.size() << endl;
  for (VrmlObject *vrml : vrmlObjects) {
    VrmlFaceSet *faceSet = dynamic_cast<VrmlFaceSet *>(vrml);
    if (faceSet != NULL) {
      cout << faceSet->debug_description() << endl;
    }
  }

  return vrmlObjects;
}

VrmlObject::VrmlObject() : diffuseColor(0, 0, 0), emissiveColor(0, 0, 0) {}

// Với mỗi FaceDef có {n} vertex sẽ có cạnh đầu được tạo bởi FaceDef[0] và FaceDef[1]
// Và cạnh cuối FaceDef được tạo bởi FaceDef[n - 2] và FaceDef[n-1]
// Tôi muốn sort lại thứ tự f2 sao cho cạnh cuối f1 trùng với cạnh đầu f2.
// Ví dụ: f1[1,2,3] và f2[7,6,2,3] --> sort f2[2,3,7, 6]
void sortVertex(FaceDef &f1, FaceDef &f2) {
  if (f1.size() < 2 || f2.size() < 2) {
    // Không đủ đỉnh để tạo cạnh
    return;
  }

  // Tìm cạnh cuối cùng của f1
  int lastVertex1 = f1[f1.size() - 2];
  int lastVertex2 = f1[f1.size() - 1];

  // Tìm vị trí của cạnh này trong f2
  auto it = std::find(f2.begin(), f2.end(), lastVertex1);
  if (it != f2.end() && it + 1 != f2.end() && *(it + 1) == lastVertex2) {
    // Xoay f2 để cạnh cuối của f1 trở thành cạnh đầu của f2
    cout << "std::rotate(f2.begin(), it, f2.end());" << endl;
    std::rotate(f2.begin(), it, f2.end());
  }
}

// Hãy gom các cặp vertex cạnh nhau thành 1 set.
// Ví dụ: [0,3,2,1] ==> [0,3], [3,2], [2,1], [1,0]
set<set<int>> makeSet2Verties(std::vector<int> vertices) {
  set<set<int>> edgeSet;
  for (size_t i = 0; i < vertices.size(); ++i) {
    set<int> edge;
    edge.insert(vertices[i]);
    edge.insert(vertices[(i + 1) % vertices.size()]);
    edgeSet.insert(edge);
  }
  // printSetOfSets(edgeSet);
  return edgeSet;
}

// Kiểm tra xem hai FaceDef có cạnh chung hay không
bool haveCommonEdge(FaceDef &f1, FaceDef &f2) {
  auto edges1 = makeSet2Verties(f1);
  auto edges2 = makeSet2Verties(f2);
  auto result = hasCommonEdge(edges1, edges2);

  cout << "COMPARE SET" << endl;
  printSetOfSets(edges1);
  printSetOfSets(edges2);
  cout << "RESULT = " << result << endl;

  return result;
}