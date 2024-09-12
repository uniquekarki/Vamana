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

    // printData(data);

    // Calculate the medoid
    vector<float> medoid = calc_medoid(data);

    // Print the medoid
    // cout << "Medoid: ";
    // for (const auto &val : medoid) {
    //     cout << val << " ";
    // }
    // cout << endl;
    
    vector<vector<int>> graph = create_graph(data, 5);
    vector<float> xq = {0.555555,0.555555};
    pair<vector<int>, unordered_set<int>> result = greedy_search(data, graph, medoid, xq, 2, 3);

    // Extract the nearest k elements and the set of visited nodes from the result
    vector<int> nearest_k_elements = result.first;
    unordered_set<int> visited_nodes = result.second;
    
    
    // cout << "Nearest K elems: \n";
    // for (const auto &elem: nearest_k_elements){
    //     cout << elem << " ";
    // };

    // cout << "\nVisited Nodes: \n";
    // for (const auto &elem: visited_nodes){
    //     cout << elem << " ";
    // };
    // cout << endl;
    

    // Parameters for robust pruning
    float alpha = 1.2; // Example distance threshold
    int R = 2; // Maximum number of out-neighbors for the node
    int p = nearest_k_elements[0]; // Assume the first nearest neighbor as the point of reference
    
    cout << "P: "<< p << endl;
    
    cout << "graph[p]: "<< endl;
    for(const auto &elem: graph[p]){
        cout << elem << " "; 
    };

    graph = robust_prune(p, alpha, R, data, graph);

    cout << endl;

    cout << "graph[p]: "<< endl;
    for(const auto &elem: graph[p]){
        cout << elem << " "; 
    };

    return 0;
}