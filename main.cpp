#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_set>
#include "vamana.h"
using namespace std;

vector<vector<float>> getData(const string& filename){
    vector<vector<float>> data;
    string line;

    ifstream dataFile(filename);
    while (getline(dataFile, line)){
        vector<float> v;
        stringstream ss(line);
        float value;
        while (ss >> value){
            v.push_back(value);
        };
        data.push_back(v);
    };
    dataFile.close();
    return data;
};

void printData(const vector<vector<float>>& data){
    cout << "DATA:" << endl;
    for (const auto& row: data){
        for (const auto& val: row){
            cout << val << " "; 
        };
        cout << endl;
    };
};

int main(){
    const string filename = "data.txt";
    vector<vector<float>> data = getData(filename);

    float alpha = 1.2; // Example distance threshold
    int R = 2; // Maximum number of out-neighbors for the node
    int L_size = 3;
    vector<vector<int>> G = vamana(data, R, L_size, alpha);

    cout << "Final Graph After Vamana:\n";
    for (const auto &row: G){
        for (const auto &elem: row){
            cout << elem << " ";
        };
        cout << endl;
    };

    cout << "You are going to University of California, Riverside Mr. Unique Karki but soon Dr. Unique Karki\n";
    return 0;
}