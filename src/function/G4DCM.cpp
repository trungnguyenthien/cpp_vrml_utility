#include "G4DCM.h"
#include <iostream>
#include <fstream>
#include <string>
#include "../common/StringUtils.h"
#include "../common/FileUtils.h"

long G4DCM::line_start_matrix(int matrix_index)
{
    // cout << "line_per_matrix " << line_per_matrix << endl;
    return matetials_map.size() + 5 + line_per_matrix * matrix_index - 1;
    // Do index tính từ 0 nên phải -1
}

int G4DCM::readInfoWithMaterial(string file)
{
    this->filePath = file;

    ifstream inputFile(file);

    if (!inputFile.is_open())
    {
        cerr << "Không thể mở file." << endl;
        return 1;
    }

    string line;
    int matt_num = -1;
    int matt_count = 0;

    bool read_n_xyz = false;
    bool read_size_x = false;
    bool read_size_y = false;
    bool read_size_z = false;

    while (getline(inputFile, line))
    {
        // Line dau tien la mat_num
        if (matt_num == -1)
        {
            matt_num = std::stoi(line);
            continue;
        }

        if (matt_count < matt_num)
        {
            vector<string> parts = splitString(line, " ");
            if (parts.size() >= 2)
            {
                float value = stof(parts[0]);
                string matt_name = parts[1];
                matetials_map[value] = matt_name;
                cout << "value: " << value << "  matt_name" << matt_name << endl;
            }
            matt_count++;
            continue;
        }

        if (!read_n_xyz)
        {
            vector<string> parts = splitString(line, " ");
            if (parts.size() >= 2)
            {
                nx = stoi(parts[0]);
                ny = stoi(parts[1]);
                nz = stoi(parts[2]);
                line_per_matrix = ny * nz;
                cout << "nx:" << nx << "  ny:" << ny << " nz:" << nz << endl;
            }
            read_n_xyz = true;
            continue;
        }

        if (!read_size_x)
        {
            read_size_x = true;
            vector<string> parts = splitString(line, " ");
            if (parts.size() >= 2)
            {
                x_min = stof(parts[0]);
                x_max = stof(parts[1]);
                cout << "x_min: " << x_min << "  x_max" << x_max << endl;
            }
            continue;
        }

        if (!read_size_y)
        {
            read_size_y = true;
            vector<string> parts = splitString(line, " ");
            if (parts.size() >= 2)
            {
                y_min = stof(parts[0]);
                y_max = stof(parts[1]);
                cout << "y_min: " << y_min << "  y_max" << y_max << endl;
            }
            continue;
        }

        if (!read_size_z)
        {
            read_size_z = true;
            vector<string> parts = splitString(line, " ");
            if (parts.size() >= 2)
            {
                z_min = stof(parts[0]);
                z_max = stof(parts[1]);
                cout << "z_min: " << z_min << "  z_max" << z_max << endl;
            }
            continue;
        }
    }

    inputFile.close();
    return 1;
}

void G4DCM::forEachMatrix(int matrix_index, function<void(float, int, int, int)> func)
{
    long startIndex = line_start_matrix(matrix_index);
    long endIndex = startIndex + line_per_matrix;
    cout << "startIndex" << startIndex << endl;
    cout << "endIndex" << endIndex << endl;

    forEachLine(this->filePath, [startIndex, endIndex, this, func](long lineIndex, string line)
                {
                    if (lineIndex < startIndex || lineIndex > endIndex)
                    {
                        return;
                    }

                    // if(0 == lineIndex % 1000) {
                    //     cout << startIndex << " - " << lineIndex << " - " << endIndex << endl;
                    //     cout << line << endl;
                    // }
                    
                    // cout << "Read Line: " << line << endl;
                    long matrixLine = lineIndex - startIndex;
                    int yi = matrixLine % this->ny;
                    int zi = int(matrixLine / this->ny);
                    vector<float> xs = splitFloats(line, " ");

                    for (size_t xi = 0; xi < xs.size(); ++xi)
                    {
                        // if(xs[xi] > 40) {
                        //     cout << "lineIndex " << lineIndex << endl;
                        //     cout << "Line = " << line << endl;
                        // }
                        func(xs[xi], xi, yi, zi);
                    } });
}
