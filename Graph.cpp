#include "Graph.h"
#include <algorithm>
#include <iterator>
#include <vector>

using namespace std;


Graph::Graph(vector<set<int>>& adj, int left_vertices, int k) {
    this->adj = adj;
    this->left_vertices = left_vertices;
    this->k = k;
    size = adj.size();
    solution.resize(size, 0);
}

bool Graph::is_kbiplex(vector<int>& solution) {

    int cnt_left_solution = 0;
    for (int i = 0; i < left_vertices; ++i)
        cnt_left_solution += solution[i];

    for (int i = left_vertices; i < size; i++) {
        if (!solution[i]) continue;

        int cnt_left_taken = 0;
        for (int x : adj[i])
            cnt_left_taken += solution[x];

        int disconnections = cnt_left_solution - cnt_left_taken;
        if (disconnections > k)
            return false;
    }

    int cnt_right_solution = 0;
    for (int i = left_vertices; i < size; ++i)
        cnt_right_solution += solution[i];

    for (int i = 0; i < left_vertices; i++) {
        if (!solution[i]) continue;

        int cnt_right_taken = 0;
        for (int x : adj[i])
            cnt_right_taken += solution[x];

        int disconnections = cnt_right_solution - cnt_right_taken;
        if (disconnections > k)
            return false;
    }

    
    return true;
}

vector<int> Graph::initial_soultion() {

    vector<int> solution(size, 0);

    fill(solution.begin() + left_vertices, solution.end(), 1);

    for (int i = 0; i < left_vertices; i++) {
        solution[i] = 1;
        if (!is_kbiplex(solution)) {
            solution[i] = 0;
        }
    }

    return solution;

}

void Graph::partition_R(vector<int>& L, vector<int>& R, int v, vector<int>& R_keep, vector<int>& R1, vector<int>& R2) {

    for (auto u : R) {
        if (adj[v].count(u))
            R_keep.push_back(u);
        else {
            int cnt = 0;
            for (auto v : L) {
                if (!adj[u].count(v))
                    cnt++;
            }
            if (cnt < k)
                R1.push_back(v);
            else
                R2.push_back(v);
        }
    }
}

vector<int> now;
void generate_all_subset(int i, int cnt, int limit, vector<int>& R, vector<vector<int>>& all_subset) {
    if (cnt > limit) return;
    if (i == R.size()) {
        all_subset.push_back(now);
        return;
    }

    // Swapped recursive calls so that the subsets returned would be sorted by size.
    generate_all_subset(i + 1, cnt, limit, R, all_subset);  
    now.push_back(R[i]);
    generate_all_subset(i + 1, cnt + 1, limit, R, all_subset);
    now.pop_back();
}


void enumerate_r_enum_subset(vector<int>& R1, vector<int>& R2, int k, vector<vector<int>> &all_subset_r1, vector<vector<int>> &all_subset_r2, int *pos) {
    int total_size = R1.size() + R2.size();
    // R1 U R2 all subsets
    now.clear();
    generate_all_subset(0, 0, k, R1, all_subset_r1);

    now.clear();
    generate_all_subset(0, 0, k, R2, all_subset_r2);

    /* Commented because they will be sorted by size anyway **/
    // sort(all_subset_r1.begin(), all_subset_r1.end(), [](auto& a, auto& b) {return a.size() < b.size(); });
    // sort(all_subset_r2.begin(), all_subset_r2.end(), [](auto& a, auto& b) {return a.size() < b.size(); });

    //pos[i] gives the index of the first susbet of r2 with size i.
    pos[0] = 0;
    pos[k+1] = all_subset_r2.size();
    for(int i = 1; i < all_subset_r2.size(); i++){
        if(all_subset_r2[i] != all_subset_r2[i - 1])
            pos[all_subset_r2[i].size()] = i;
    }
}


bool is_local_solution(vector<int> &L, vector<int> &R, vector<int> &L_sub, vector<int> &R_sub, int v){
    
}

/**
L -> Left vertices in the previous solution 
R -> Right vertices in the previous solution
v -> left vertex to add and find local solutions of
*/
vector<vector<int>> Graph::enumAlmostSat(vector<int>& L, vector<int>& R, int v) {

    // Make sure the function sends sorted L and R so these lines can be removed.
    sort(R.begin(), R.end());
    sort(L.begin(), L.end());


    vector<int> R_keep, R1, R2;
    partition_R(L, R, v, R_keep, R1, R2);

    vector<vector<int>> r1_powset, r2_powset;
    int pos[k+2];
    enumerate_r_enum_subset(R1, R2, k, r1_powset, r2_powset, pos);

    for(auto r1_subset : r1_powset){
        
        int size1 = r1_subset.size();
        int size2 = k - size1;

        int start = 0;
        int end = pos[k+1];

        // If the subset of R1 is not equal to R1, then according to Lemma 4.2, the size of the union of the subsets of R1 and R2 should be exactly k.
        if(r1_subset.size() != R1.size()){
            start = pos[size2];
            end = pos[size2 + 1];
        }

        for(int i = start; i < end; i++){

            vector<int> r2_subset = r2_powset[i];
            vector<int> r_enum_subset, r_subset;
            
            set_union(r1_subset.begin(), r1_subset.end(), r2_subset.begin(), r2_subset.end(), back_inserter(r_enum_subset));
            set_union(R_keep.begin(), R_keep.end(), r_enum_subset.begin(), r_enum_subset.end(), back_inserter(r_subset));


            vector<int> L_remo;
            for(auto v : L){
                for(auto u : r2_subset){
                    if(!adj[v].count(u)){
                        L_remo.push_back(v);
                        break;
                    }
                }
            }

            vector<vector<int>> l_remo_powset;
            // We generate susbsets of L_remo with size less than R''
            generate_all_subset(0, 0, r2_subset.size(), L_remo, l_remo_powset);

            for(auto l_remo_subset : l_remo_powset){
                vector<int> l_subset;
                set_difference(L.begin(), L.end(), l_remo_subset.begin(), l_remo_subset.end(), back_inserter(l_subset));

                if(is_local_solution(L, R, l_subset, r1_subset, v)){

                }

            }

        }
    }



}
