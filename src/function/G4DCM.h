#ifndef G4DCM_H
#define G4DCM_H

#include <iostream>
#include <functional>
#include <vector>
#include <unordered_map>
using namespace std;
class G4DCM
{
private:
    long line_per_matrix = 0;
    long line_start_matrix(int matrix_number);

public:
    string filePath;
    unordered_map<int, string> matetials_map;
    int nx, ny, nz;

    float x_min, x_max, y_min, y_max, z_min, z_max;
    vector<vector<float>> matetials_matrix;
    vector<vector<float>> densities_matrix;
    vector<vector<float>> structures_matrix;

    int readInfoWithMaterial(string file);
    void forEachMatrix(int matrix_index, function<void(float, int, int, int)> func);
};

#endif /* G4DCM_H */
