#include "FileUtils.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int forEachLine(string file, function<void(long, string)> func) {
  ifstream inputFile(file);

  if (!inputFile.is_open()) {
    cerr << "Can not read file: " << file << endl;
    return 0;
  }
  string line;
  long line_index = 0;
  while (std::getline(inputFile, line)) {
    func(line_index, line);
    line_index++;
  }
  inputFile.close();
  return 1;
}

void appendAfterLine(const std::string &filePath, const std::string &outFilePath,
                     const std::string &searchLine, const std::string &appendText) {
  std::ifstream inputFile(filePath);
  std::ofstream outputFile(outFilePath);
  std::string line;
  bool found = false;

  if (!inputFile.is_open() || !outputFile.is_open()) {
    std::cerr << "Không thể mở file đầu vào hoặc đầu ra." << std::endl;
    return;
  }

  // Đọc file và tìm vị trí của dòng cần tìm
  while (std::getline(inputFile, line)) {
    if (line == searchLine) {
      outputFile << appendText << "\n";
      outputFile << line << "\n";
      found = true;
      break;
    } else {
      outputFile << line << "\n";
    }
  }

  // Sao chép phần còn lại của file nếu tìm thấy dòng
  if (found) {
    while (std::getline(inputFile, line)) {
      outputFile << line << "\n";
    }
  }

  // Nếu không tìm thấy dòng, thêm nội dung vào cuối file
  if (!found) {
    outputFile << appendText << "\n";
  }

  inputFile.close();
  outputFile.close();
}
