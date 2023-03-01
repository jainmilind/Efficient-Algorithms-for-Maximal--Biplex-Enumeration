#include "Graph.h"
#include <vector>

using namespace std;


Graph::Graph(vector<vector<int>> &adj, int left_vertices, int k){
    this -> adj = adj;
    this -> left_vertices = left_vertices;
    this -> k = k;
    size = adj.size();
    solution.resize(size, 0);
}

bool Graph::is_kbiplex(vector<int> &solution){

    for(int i = 0; i < size; i++){
        if(!solution[i])
            continue;
        
        int disconnections = 0;

        for(auto v : adj[i]){
            solution[v] *= -1;
        }
        
        // i is a vertex in the left part
        if(i < left_vertices){
            for(int j = left_vertices; j < size; j++)
                disconnections += solution[j] == 1;
        }

        // i is a vertex in the right part
        else{
            for(int j = 0; j < left_vertices; j++)
                disconnections += solution[j] == 1;
        }   

        for(auto v : adj[i]){
            solution[v] *= -1;
        }   

        if(disconnections > k)
            return false;
    }

    return true;
}

vector<int> Graph::initial_soultion(){

    vector<int> solution(size, 0);

    fill(solution.begin() + left_vertices, solution.end(), 1);
    
    for(int i = 0; i < left_vertices; i++){
        solution[i] = 1;
        if(!is_kbiplex(solution)){
            solution[i] = 0;
        }
    }

    return solution;

}

void Graph::partition_R(vector<int> &L, vector<int> &R, int v, vector<int> &R_keep, vector<int> &R1, vector<int> &R2, vector<vector<int>> &H_adj){
    
    for(auto u : H_adj[v]){
        R_keep.push_back(u);
    }

    for(auto u : R){
        if(find(R_keep.begin(), R_keep.end(), u) != R.end())
            continue;
        if(H_adj[u].size() == k + 1)
            R2.push_back(u);
        else
            R1.push_back(u);
    }


}

vector<vector<int>> Graph::enumAlmostSat(vector<int> &L, vector<int> &R, int v,  vector<vector<int>> &H_adj){
    
    vector<int> R_keep, R1, R2;
    partition_R(L, R, v, R_keep, R1, R2, H_adj);

}