#include "Graph.h"
#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
using namespace std;

template <typename A, typename B> ostream& operator<<(ostream& os, const pair<A, B>& p) { return os << "(" << p.first << ", " << p.second << ")"; }
template <typename T_container, typename T = typename enable_if < !is_same<T_container, string>::value, typename T_container::value_type >::type > ostream& operator<<(ostream& os, const T_container& v) { os << '{'; string sep; for (const T& x : v) os << sep << x, sep = ", "; return os << '}'; }
template <typename T> void debug_out(string s, T t) { cout << "[" << s << ": " << t << "]\n"; }
template <typename T, typename... U> void debug_out(string s, T t, U... u) { int w = 0, c = 0; while (w < (int)s.size()) { if (s[w] == '(') c++; if (s[w] == ')') c--; if (!c and s[w] == ',') break; w++; } cout << "[" << s.substr(0, w) << ": " << t << "] "; debug_out(s.substr(w + 2, (int)s.size() - w - 1), u...); }
#define dbg(x...) debug_out(#x, x)



Graph::Graph(vector<set<int>>& adj, int left_vertices, int k) {
    this->adj = adj;
    this->left_vertices = left_vertices;
    this->k = k;
    size = adj.size();
    solution.resize(size, 0);
}

// boolean array of taken
bool Graph::is_kbiplex(vector<bool>& solution) {

    int cnt_left_solution = 0;
    for (int i = 0; i < left_vertices; ++i)
        cnt_left_solution += solution[i];

    for (int i = left_vertices; i < size; i++) {
        if (!solution[i]) continue;

        int cnt_left_taken = 0;
        for (int x : adj[i])
            cnt_left_taken += solution[x];

        int disconnections = cnt_left_solution - cnt_left_taken;
        if (disconnections > k) {
            return false;
        }
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
        if (disconnections > k) {
            return false;
        }
    }


    return true;
}

vector<bool> Graph::initial_soultion() {

    vector<bool> solution(size);

    fill(solution.begin() + left_vertices, solution.end(), true);

    for (int i = 0; i < left_vertices; i++) {
        solution[i] = true;
        if (!is_kbiplex(solution)) {
            solution[i] = false;
        }
    }

    return solution;
}

// Not O(|L|*|R|)
// TODO: use set minus to make it O(L * R)
bool Graph::is_local_solution(vector<int>& L, vector<int>& R, vector<int>& L_sub, vector<int>& R_sub, vector<int>& R_enum_sub, vector<int>& R2_sub, int v) {
    
    vector<int> L_rem, R_rem;
    set_difference(L.begin(), L.end(), L_sub.begin(), L_sub.end(), back_inserter(L_rem));
    set_difference(R.begin(), R.end(), R_sub.begin(), R_sub.end(), back_inserter(R_rem));

    // dbg(L, R, L_sub, R_sub, R_enum_sub, R2_sub, v);

    vector<int> R_k_disc;

    for(auto r : R2_sub){
        int connections = 0;
        for(auto l : L_sub){
            if(adj[l].count(r))
                connections++;
        }
        int disconnections = (int)L_sub.size() + 1 - connections;
        if(disconnections > k)
            return false;

        if(disconnections == k)
            R_k_disc.push_back(r);
    }

    // dbg(R_k_disc);

    for(auto l : L_rem){
        bool flag = true;
        for(auto r : R_k_disc){
            if(!adj[l].count(r)){
                flag = false;
                break;
            }
        }
        if(flag)
            return false;
    }

    if(R_enum_sub.size() == k)
        return true;
    
    for(auto r : R_rem){
        int connections = 0;
        for(auto l : L_sub){
            if(adj[l].count(r))
                connections++;
        }
        if((int)L_sub.size() + 1 - connections <= k)
            return false;
    }

    return true;
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
                R1.push_back(u);
            else
                R2.push_back(u);
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


void enumerate_r_enum_subset(vector<int>& R1, vector<int>& R2, int k, vector<vector<int>>& all_subset_r1, vector<vector<int>>& all_subset_r2, vector<int>& pos) {
    // R1 U R2 all subsets
    now.clear();
    generate_all_subset(0, 0, k, R1, all_subset_r1);

    now.clear();
    generate_all_subset(0, 0, k, R2, all_subset_r2);

    sort(all_subset_r1.begin(), all_subset_r1.end(), [](auto& a, auto& b) {return a.size() < b.size(); });
    sort(all_subset_r2.begin(), all_subset_r2.end(), [](auto& a, auto& b) {return a.size() < b.size(); });

    // pos[i] gives the index of the first subset of r2 with size i.
    pos[0] = 0;
    pos[k + 1] = all_subset_r2.size();
    for (int i = 1; i < all_subset_r2.size(); i++) {
        if (all_subset_r2[i].size() != all_subset_r2[i - 1].size())
            pos[all_subset_r2[i].size()] = i;
    }
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
    vector<int> pos(k + 2, -1);
    enumerate_r_enum_subset(R1, R2, k, r1_powset, r2_powset, pos);

    for (int i = 0; i <= k + 1;) {
        int j = i + 1;
        while (j <= k + 1 and pos[j] == -1) {
            pos[j] = pos[k + 1];
            j++;
        }
        i = j;
    }

    vector<vector<int>> local_solutions;

    for (auto& r1_subset : r1_powset) {
        int size1 = r1_subset.size();
        // size2 is R2 subset size limit
        int size2 = k - size1;

        int start = 0;
        int end = pos[size2 + 1];

        // If the subset of R1 is not equal to R1, then according to Lemma 4.2, the size of the union of the subsets of R1 and R2 should be exactly k.
        if (r1_subset.size() != R1.size()) {
            start = pos[size2];
            end = pos[size2 + 1];
        }

        for (int i = start; i < end; ++i) {
            assert(r2_powset.size() > i);
            vector<int>& r2_subset = r2_powset[i];
            vector<int> r_enum_subset, r_subset, r2_rem;

            set_union(r1_subset.begin(), r1_subset.end(), r2_subset.begin(), r2_subset.end(), back_inserter(r_enum_subset));
            set_union(R_keep.begin(), R_keep.end(), r_enum_subset.begin(), r_enum_subset.end(), back_inserter(r_subset));
            // set_difference(R2.begin(), R2.end(), r2_subset.begin(), r2_subset.end(), back_inserter(r2_rem));

            vector<int> L_remo;
            for (auto v : L) {
                for (auto u : r2_subset) {
                    if (!adj[v].count(u)) {
                        L_remo.push_back(v);
                        break;
                    }
                }
            }

            vector<vector<int>> l_remo_powset;
            // We generate susbsets of L_remo with size less than R''
            generate_all_subset(0, 0, r2_subset.size(), L_remo, l_remo_powset);
            sort(l_remo_powset.begin(), l_remo_powset.end(), [](auto& a, auto& b) {return a.size() < b.size(); });

            for (auto& l_remo_subset : l_remo_powset) {

                // Pruning all supersets of L_remo_subset
                bool skip = false;
                for (auto& cur : local_solutions) {
                    vector<int> temp;
                    set_intersection(cur.begin(), cur.end(), l_remo_subset.begin(), l_remo_subset.end(), back_inserter(temp));
                    if (temp == cur) {
                        skip = true;
                        break;
                    }
                }
                if (skip) continue;

                vector<int> l_subset;
                set_difference(L.begin(), L.end(), l_remo_subset.begin(), l_remo_subset.end(), back_inserter(l_subset));

                /*DELETE begin*/
                vector<bool> now(size);
                for (int i : l_subset) now[i] = 1;
                for (int i : r_subset) now[i] = 1;
                now[v] = 1;

                if (is_kbiplex(now)) {
                    local_solutions.push_back(vector<int>());
                    set_union(l_subset.begin(), l_subset.end(), r_subset.begin(), r_subset.end(), back_inserter(local_solutions.back()));
                    local_solutions.back().push_back(v);
                    // TODO Use insert to make it O(n)
                    sort(local_solutions.back().begin(), local_solutions.back().end());
                }
                /*DELETE end*/
                
                // /*
                if (is_local_solution(L, R, l_subset, r_subset, r_enum_subset, r2_subset, v)) {
                    local_solutions.push_back(vector<int>());
                    set_union(l_subset.begin(), l_subset.end(), r_subset.begin(), r_subset.end(), back_inserter(local_solutions.back()));
                    local_solutions.back().push_back(v);
                    // TODO Use insert to make it O(n)
                    sort(local_solutions.back().begin(), local_solutions.back().end());
                    vector<bool> temp(size);
                    for (int i : local_solutions.back()) temp[i] = 1;
                    if (!is_kbiplex(temp)) {
                        cout  << "OK VREO" << " " << temp << endl;
                    }
                }
                // */
            }

        }
    }

    /*DELETE begin*/
    // vector<vector<int>> new_local;
    // for (int i = 0; i < local_solutions.size(); ++i) {
    //     bool take = true;
    //     for (int j = 0; j < local_solutions.size(); ++j) {
    //         if (i == j) continue;
    //         vector<int> temp;
    //         set_intersection(local_solutions[i].begin(), local_solutions[i].end(), local_solutions[j].begin(), local_solutions[j].end(), back_inserter(temp));
    //         if (temp == local_solutions[i]) {
    //             take = false;
    //             break;
    //         }
    //     }
    //     if (take) new_local.push_back(local_solutions[i]);
    // }
    // local_solutions = new_local;
    /*DELETE end*/
    
    return local_solutions;
}

vector<bool> Graph::extendToMax(vector<int>& L_loc, vector<int>& R_loc) {
    vector<int> L(left_vertices);
    iota(L.begin(), L.end(), 0);

    vector<int> L_diff;
    set_difference(L.begin(), L.end(), L_loc.begin(), L_loc.end(), back_inserter(L_diff));

    vector<bool> taken(size);
    for (int i : L_loc) taken[i] = true;
    for (int i : R_loc) taken[i] = true;

    for (auto i : L_diff) {
        taken[i] = true;
        if (!is_kbiplex(taken))
            taken[i] = false;
    }

    return taken;
}


void Graph::ThreeStep(vector<bool>& h0, set<vector<bool>>& ans) {
    // V(G) / V(H)
    vector<int> v_diff;
    vector<int> L, R;
    for (int i = 0; i < size; ++i) {
        if (h0[i] != true)
            v_diff.push_back(i);
        else {
            if (i < this->left_vertices)
                L.push_back(i);
            else R.push_back(i);
        }
    }
    for (int v : v_diff) {
        for (auto& H_loc : enumAlmostSat(L, R, v)) {
            vector<int> L_loc, R_loc;
            for (int i : H_loc) {
                if (i < this->left_vertices)
                    L_loc.push_back(i);
                else R_loc.push_back(i);
            }
            vector<bool> H_max = extendToMax(L_loc, R_loc);

            if (!ans.count(H_max)) {
                ans.insert(H_max);
                ThreeStep(H_max, ans);
            }
        }
    }
}

set<vector<bool>> Graph::bTraversal() {
    vector<bool> h0 = initial_soultion();

    set<vector<bool>> ans;
    ans.insert(h0);
    ThreeStep(h0, ans);

    return ans;
}

void Graph::iThreeStep(vector<bool>& h0, set<vector<bool>>& ans) {
    vector<int> v_diff;
    vector<int> L, R;
    for (int i = 0; i < size; ++i) {
        if (h0[i] != true and i < left_vertices)
            v_diff.push_back(i);
        else if (h0[i] == true) {
            if (i < this->left_vertices)
                L.push_back(i);
            else R.push_back(i);
        }
    }

    vector<int> R_full;
    for (int i = left_vertices; i < size; ++i)
        R_full.push_back(i);

    for (int v : v_diff) {
        for (auto& H_loc : enumAlmostSat(L, R, v)) {
            vector<bool> h_loc_bool(size);
            vector<int> L_loc, R_loc;
            for (int i : H_loc) {
                h_loc_bool[i] = 1;
                if (i < this->left_vertices)
                    L_loc.push_back(i);
                else R_loc.push_back(i);
            }

            bool skip = false;
            vector<int> r_diff;
            set_difference(R_full.begin(), R_full.end(), H_loc.begin(), H_loc.end(), back_inserter(r_diff));
            for (int u : r_diff) {
                h_loc_bool[u] = 1;
                if (is_kbiplex(h_loc_bool)) {
                    skip = true;
                    break;
                }
                h_loc_bool[u] = 0;
            }

            if (skip)
                continue;

            vector<bool> H_max = extendToMax(L_loc, R_loc);

            if (!ans.count(H_max)) {
                cout << "sol here: " << H_max << endl;
                ans.insert(H_max);
                iThreeStep(H_max, ans);
            }
        }
    }
}

set<vector<bool>> Graph::iTraversal() {
    vector<bool> h0 = initial_soultion();

    set<vector<bool>> ans;
    ans.insert(h0);
    cout << "initial here: " << h0 << endl;
    iThreeStep(h0, ans);

    return ans;
}
