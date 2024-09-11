#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <algorithm>
#include <unordered_set>

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

vector<vector<int>> create_graph(const vector<vector<float>> &P, int R){
    vector<vector<int>> G;
    int n = P.size();

    srand(time(0));
    for (int i=0; i<n; i++){
        vector<int> edges;
        while (edges.size() < R){

            // The random integer created would be the random edge for vector in i-th position
            int randomNum = rand() % n;
            
            // Avoid self and duplicates
            if(randomNum!=i && find(edges.begin(), edges.end(), randomNum) == edges.end() ){
                edges.push_back(randomNum);
            };
        };
        G.push_back(edges);
    };

    return G;
};

int get_index(const vector<vector<float>> &P,const vector<float> &point){
    auto it = find(P.begin(), P.end(), point);
    return it - P.begin();
};

pair<vector<int>, unordered_set<int>> greedy_search(
    const vector<vector<float>> &P, // Dataset of points
    const vector<vector<int>> &G, // Adjacency list graph
    vector<float> &s, // Start point (we use medoid mostly but can be other points)
    vector<float> &xq, // Query point
    int k, // Number of nearest neighbors to return
    int L_size // Size of the search list
){
    // Initialization
    vector<int> L = {get_index(P, s)};
    unordered_set<int> V;

    float mindist = numeric_limits<float>::max();
    int minNode;

    while(true){
        bool has_unvisited = false;
        for (const auto &elem: L){
            if(V.find(elem) == V.end()){
                has_unvisited = true;

                break; // Looped over all elements in L. If any elem is not in V the loop is broken to tell that at least one elem is unvisited
            };
        };

        // Stops the process if L \ V is null
        if(!has_unvisited){
            break;
        };

        int p_star = -1;
        float mindist = numeric_limits<float>::max();

        // Gets the min distance element from L \ V and sets it as p*
        for(const auto &elem: L){
            if(V.find(elem) == V.end()){
                float distance = euclideanDistance(P[elem],xq);
                if(distance < mindist){
                    mindist = distance;
                    p_star = elem;
                };
            };
        };

        for(const auto &out: G[p_star]){
            if(V.find(out) == V.end() && find(L.begin(), L.end(), out) == L.end()){
                L.push_back(out);
            };
        };

        V.insert(p_star);

        if (L.size() > L_size) {
            sort(L.begin(), L.end(), [&](int a, int b) {
                return euclideanDistance(P[a], xq) < euclideanDistance(P[b], xq);  // Sort by distance to xq
            });
            L.resize(L_size);  // Keep only the closest L_size points
        }
    };
    
    if (L.size() > k) {
        L.resize(k); // Resize to contain only the closest k points
    }

    return make_pair(L, V); 
};