#ifndef B80A09D8_E1A2_4CE2_8CCB_A090877B6574
#define B80A09D8_E1A2_4CE2_8CCB_A090877B6574

#include <functional>
#include <iostream>

using namespace std;

int forEachLine(string file, function<void(long, string)> func);
void appendAfterLine(const std::string &filePath, const std::string &outFilePath, const std::string &searchLine, const std::string &appendText);

#endif /* B80A09D8_E1A2_4CE2_8CCB_A090877B6574 */
