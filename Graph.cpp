#include "Graph.h"
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
    if (cnt > k) return;
    if (i == R.size()) {
        all_subset.push_back(now);
        return;
    }
    now.push_back(R[i]);
    generate_all_subset(i + 1, cnt + 1, limit, R, all_subset);
    now.pop_back();
    generate_all_subset(i + 1, cnt, limit, R, all_subset);
}


void enumerate_r_enum_subset(vector<int>& R1, vector<int>& R2) {
    int total_size = R1.size() + R2.size();
    // R1 U R2 all subsets
    vector<vector<int>> all_subset_r1;
    now.clear();
    generate_all_subset(0, 0, k, R1, all_subset_r1);

    vector<vector<int>> all_subset_r2;
    now.clear();
    generate_all_subset(0, 0, k, R2, all_subset_r2);

    sort(all_subset_r1.begin(), all_subset_r1.end(), [](auto& a, auto& b) {return a.size() < b.size(); });
    sort(all_subset_r2.begin(), all_subset_r2.end(), [](auto& a, auto& b) {return a.size() < b.size(); });

    for (auto& subset : all_subset_r1) {

    }
}

/**
L -> Left vertices in the previous solution
R -> Right vertices in the previous solution
v -> left vertex to add and find local solutions of
*/
vector<vector<int>> Graph::enumAlmostSat(vector<int>& L, vector<int>& R, int v) {

    vector<int> R_keep, R1, R2;
    partition_R(L, R, v, R_keep, R1, R2);

    enumerate_r_enum_subset(R1, R2);
}
