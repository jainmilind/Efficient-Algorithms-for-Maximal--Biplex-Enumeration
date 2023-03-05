#include <bits/stdc++.h>

using namespace std;

class Graph{

    public:
    int size; // number of vertices
    int left_vertices; // number of vertices in the left subset of vertices
    int k; // maximum number of disconnections

    vector<set<int>> adj; // adjaceny list of the graph
    vector<bool> solution; // a maximal k-biplex, true -> vertex is in solution else not present

    Graph(vector<set<int>>&, int, int);
    bool is_kbiplex(vector<int>&);
    vector<int> initial_soultion();
    vector<vector<int>> enumAlmostSat(vector<int>&, vector<int>&, int);
    void partition_R(vector<int>&, vector<int>&, int, vector<int>&, vector<int>&, vector<int>&);    
    bool is_local_solution(vector<int>&, vector<int>&, vector<int>&, vector<int>&, int);
};

