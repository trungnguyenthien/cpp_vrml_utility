#include "function/G4DCM.h"
#include <CLI/CLI.hpp>
#include "common/StringUtils.h"

int main(int argc, char **argv)
{
    CLI::App app{"---"};
    std::string filename = "";
    app.add_option("-f,--file", filename, "Path to input file");

    float targetValue = 1;
    app.add_option("-v,--value", targetValue, "Value should change to 1");

    CLI11_PARSE(app, argc, argv);
    if (filename.empty())
    {
        return 0;
    }
    string outputFilePath = filename + "." + to_string(int(targetValue)) + ".out";
    std::ofstream outputFile(outputFilePath, std::ios::out);
    if (!outputFile.is_open())
    {
        std::cerr << "Can not write out to file:" << outputFilePath << std::endl;
        return 1;
    }

    G4DCM *g4dcm = new G4DCM();
    g4dcm->readInfoWithMaterial(filename);
    vector<int> numberInLine;
    numberInLine.clear();
    g4dcm->forEachMatrix(2, [g4dcm, targetValue, &numberInLine, &outputFile](float value, int xi, int yi, int zi)
                         {
                             int convert = value == targetValue ? 1 : 0;
                             numberInLine.push_back(convert);
                             if (xi == g4dcm->nx - 1) {
                                string outline = joinVector(numberInLine, " ");
                                outputFile << outline << std::endl;
                                numberInLine.clear();
                             } });
    outputFile.close();
    return 1;
}