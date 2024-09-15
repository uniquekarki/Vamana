#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <algorithm>
#include <unordered_set>
#include <random>

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
    int xq_idx = get_index(P, xq);

    // Ensure xq is not in the initial list
    if (L[0] == xq_idx) {
        L.clear(); // Clear L if it mistakenly starts with xq
    };

    float mindist = numeric_limits<float>::max();
    int minNode;

    while(true){
        bool has_unvisited = false;
        for (const auto &elem: L){
            if(V.find(elem) == V.end() && elem != xq_idx){  // Skip xq in unvisited check
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
            if(V.find(elem) == V.end() && elem != xq_idx){  // Skip xq in min distance search
                float distance = euclideanDistance(P[elem], xq);
                if(distance < mindist){
                    mindist = distance;
                    p_star = elem;
                };
            };
        };

        for(const auto &out: G[p_star]){
            if(V.find(out) == V.end() && find(L.begin(), L.end(), out) == L.end() && out != xq_idx){  // Skip xq in neighbors
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

vector<vector<int>> robust_prune(
    const int p,
    unordered_set<int> V,
    const float alpha,
    const int R,
    const vector<vector<float>> &P,
    vector<vector<int>> &G
){
    vector<int> N_out_p;

    if (V.empty()) {
        // If V is empty, just return the current state of G
        G[p] = N_out_p;
        return G;
    }

    int p_star =-1;
    float minDist = numeric_limits<float>::max();
    for(const auto &elem: V){
        float dist = euclideanDistance(P[elem], P[p]);

        if(dist < minDist){
            minDist = dist;
            p_star = elem;
        };
    };

    N_out_p.push_back(p_star);
    if (N_out_p.size() == R) {
        G[p] = N_out_p;
        return G;
    };

    for(const auto &elem: V) {
        float dist_p_star = euclideanDistance(P[p_star], P[elem]);
        float dist_p = euclideanDistance(P[p], P[elem]);

        if (alpha * dist_p_star >= dist_p && find(N_out_p.begin(), N_out_p.end(), elem) == N_out_p.end()) {
            N_out_p.push_back(elem);
            if (N_out_p.size() == R) {
                break;
            };
        };
    };

    G[p] = N_out_p;
    return G;

};

vector<vector<int>> vamana(
    const vector<vector<float>> &P,
    const int R,
    const int L_size,
    const float alpha
){
    vector<vector<int>> G = create_graph(P, 5);
    cout << "Initial Graph: \n";
    for (const auto &row: G){
        for (const auto &elem: row){
            cout << elem << " ";
        };
        cout << endl;
    };
    vector<float> s = calc_medoid(P);


    vector<int> perm_list; 
    for(int i = 0; i < P.size(); i++){
        perm_list.push_back(i);
    };

    // Shuffle the nodes
    random_device rd; 
    mt19937 g(rd());     
    shuffle(perm_list.begin(), perm_list.end(), g);

    for(int i: perm_list){
        vector<float> xq = P[i];

        if (xq == s){
            continue;
        };

        // Greedy search from medoid to p[i]
        pair<vector<int>, unordered_set<int>> result = greedy_search(P, G, s, xq, 1, L_size);

        // Extract the nearest k elements and the set of visited nodes from the result
        int nearest_neighbor = result.first[0];
        unordered_set<int> visited_nodes = result.second;

        G = robust_prune(i, visited_nodes, alpha, R, P, G);

        vector<int> i_edge;
        for (const int x: G[i]){
            i_edge.push_back(x);
        };

        for (const int j: i_edge){
            

            if ((G[j].size() + 1) > R){
                unordered_set<int> visited_nodes_j;
                for (const auto &elem: G[j]){
                    visited_nodes_j.insert(elem);
                };
                visited_nodes_j.insert(i);
                G = robust_prune(j, visited_nodes_j, alpha, R, P, G);
            }
            else if (find(G[j].begin(), G[j].end(), i) == G[j].end()) {
                G[j].push_back(i);
            };
        };

    };

    return G;
};