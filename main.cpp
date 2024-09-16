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

int main(){
    const string filename = "data/data.txt";
    vector<vector<float>> data = getData(filename);

    float alpha = 1.2; // Example distance threshold
    int R = 2; // Maximum number of out-neighbors for the node
    int L_size = 3; // Max number of search list
    vector<vector<int>> G = vamana(data, R, L_size, alpha);

    saveAdjacencyListToFile(G, "data/final_adjacency_list.txt");

    return 0;
}