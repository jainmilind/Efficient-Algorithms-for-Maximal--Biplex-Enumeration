#include <bits/stdc++.h>

using namespace std;

class Graph{

    public:
    int size;
    int left_vertices;
    int k;
    vector<vector<int>> adj;
    vector<bool> solution;
    set<int> R_keep, R1, R2;

    Graph(vector<vector<int>>&, int, int);
    bool is_kbiplex();
    void initial_soultion();
};

