#include "Graph.cpp"
#include <bits/stdc++.h>    
using namespace std;

// template <typename A, typename B> ostream& operator<<(ostream& os, const pair<A, B>& p) { return os << "(" << p.first << ", " << p.second << ")"; }
// template <typename T_container, typename T = typename enable_if < !is_same<T_container, string>::value, typename T_container::value_type >::type > ostream& operator<<(ostream& os, const T_container& v) { os << '{'; string sep; for (const T& x : v) os << sep << x, sep = ", "; return os << '}'; }
// template <typename T> void debug_out(string s, T t) { cout << "[" << s << ": " << t << "]\n"; }
// template <typename T, typename... U> void debug_out(string s, T t, U... u) { int w = 0, c = 0; while (w < (int)s.size()) { if (s[w] == '(') c++; if (s[w] == ')') c--; if (!c and s[w] == ',') break; w++; } cout << "[" << s.substr(0, w) << ": " << t << "] "; debug_out(s.substr(w + 2, (int)s.size() - w - 1), u...); }
// #define dbg(x...) debug_out(#x, x)

int main(int c, char* argv[]) {
    int n;
    cin >> n;

    int left_cnt;
    cin >> left_cnt;

    int M;
    cin >> M;

    int K = atoi(argv[1]);
    vector<set<int>> adj(n);

    getchar();
    for (int i = 0; i < n; ++i) {
        string s, c; getline(cin, s);
        stringstream st(s);

        st >> c;
        int fst = stoi(c);
        while (st >> c)
            adj[fst].insert(stoi(c));
    }

    Graph g(adj, left_cnt, K);
     time_t s1 = clock();
    auto ans = g.iTraversal();
    time_t s2 = clock();
    cout<<"Running Time: "<<(double)(s2-s1)<<"ms"<<endl;

    // cout << "Number of answers" << ans.size() << endl;
    // for (auto v : ans) {
    //     for (int i = 0; i < n; ++i) {
    //         if (v[i]) {
    //             cout << i << "&";
    //         }
    //     }
    //     cout << '\n';
    // }
}
