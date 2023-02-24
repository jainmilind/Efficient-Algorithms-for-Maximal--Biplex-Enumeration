#include "Graph.h"

using namespace std;

// class Graph{

//     public:
//     int size;
//     int left_vertices;
//     int k;
//     vector<vector<int>> adj;
//     vector<bool> solution;
//     set<int> R_keep, R1, R2;

//     Graph(vector<vector<int>>&, int, int);
//     bool is_kbiplex();
//     void initial_soultion();
// };

Graph::Graph(vector<vector<int>> &adj, int left_vertices, int k){
    this -> adj = adj;
    this -> left_vertices = left_vertices;
    this -> k = k;
    size = adj.size();
    R_keep.clear();
    R1.clear();
    R2.clear();
    solution.resize(size, false);
}



void Graph::initial_soultion(){
    fill(solution.begin() + left_vertices, solution.end(), 1);

}