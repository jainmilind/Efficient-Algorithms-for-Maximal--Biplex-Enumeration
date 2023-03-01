#include <bits/stdc++.h>

using namespace std;

class Graph{

    public:
    int size;
    int left_vertices;
    int k;
    vector<vector<int>> adj;
    vector<int> solution;

    Graph(vector<vector<int>>&, int, int);
    bool is_kbiplex(vector<int>&);
    vector<int> initial_soultion();
    vector<vector<int>> enumAlmostSat(vector<int>&, vector<int>&, int, vector<vector<int>>&);
    void partition_R(vector<int>&, vector<int>&, int, vector<int>&, vector<int>&, vector<int>&, vector<vector<int>>&);    
};

