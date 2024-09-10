#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

using namespace std;

float euclideanDistance(const vector<float> &x, const vector<float> &y){
    float sq_dist = 0.0;
    for(int i=0;i<x.size();i++){
        sq_dist += pow(y[i] - x[i], 2);
    };
    return sqrt(sq_dist);
};

vector<float> calc_medoid(const vector<vector<float>> &data){
    vector<float> medoid;
    float min_dist = numeric_limits<float>::max();
    for(int i = 0; i < data.size(); i++){
        float dist_total = 0;
        for(int j = 0; j < data.size(); j++){
            if(i!=j){
                dist_total += euclideanDistance(data[i], data[j]);
            };
        };

        if(dist_total < min_dist){
            min_dist = dist_total;
            medoid = data[i];
        };
    };
    return medoid;
};