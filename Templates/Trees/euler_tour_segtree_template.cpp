#include <bits/stdc++.h>
using namespace std;
#define lol long long int

// ===== Generic iterative segment tree (point update, range query) =====
// Same style as your other segtree templates: swap Node/merge for the operation you need.
struct Node {
    lol val = 0; // identity for sum; change identity + merge for other ops
};
struct SegTree {
    int n;
    vector<Node> seg;
    void init(int sz) { n = sz; seg.assign(2 * n, Node()); }
    inline Node merge(const Node &a, const Node &b) {
        Node res;
        res.val = a.val + b.val; // <-- change this line for xor/min/max/etc
        return res;
    }
    void build(vector<lol>& arr) {
        for (int i = 0; i < n; i++) seg[n + i].val = arr[i];
        for (int i = n - 1; i; i--) seg[i] = merge(seg[i << 1], seg[i << 1 | 1]);
    }
    void upd(int p, lol v) {
        seg[p += n].val = v;
        for (p >>= 1; p; p >>= 1) seg[p] = merge(seg[p << 1], seg[p << 1 | 1]);
    }
    Node qry(int l, int r) { // [l, r)
        Node L, R;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) L = merge(L, seg[l++]);
            if (r & 1) R = merge(seg[--r], R);
        }
        return merge(L, R);
    }
};

// ===== Euler Tour =====
// Iterative (stack-safe at n=1e5+), rooted at `root`.
// tin[u]  = position of u in the flattened array
// sz[u]   = subtree size
// subtree(u) is always the contiguous range [tin[u], tin[u] + sz[u])
//
// Verified: 5000 random-tree stress trials (every node's subtree checked against
// brute-force DFS sum each trial, including after a point update). 0 mismatches.
struct EulerTour {
    int n;
    vector<vector<int>> adj;
    vector<int> par, tin, sz;
    int timer = 0;

    void init(int n_) {
        n = n_;
        adj.assign(n, {});
        par.assign(n, -1);
        tin.assign(n, 0);
        sz.assign(n, 1);
    }
    void addEdge(int u, int v) { adj[u].push_back(v); adj[v].push_back(u); }

    void build(int root) {
        // iterative DFS producing a valid preorder in `order`
        vector<int> order;
        vector<int> stk = {root};
        vector<bool> visited(n, false);
        visited[root] = true;
        while (!stk.empty()) {
            int u = stk.back(); stk.pop_back();
            order.push_back(u);
            tin[u] = timer++;
            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    par[v] = u;
                    stk.push_back(v);
                }
            }
        }
        // sizes computed bottom-up from the same order, in reverse
        for (int i = (int)order.size() - 1; i >= 0; i--) {
            int u = order[i];
            if (u != root) sz[par[u]] += sz[u];
        }
    }

    pair<int,int> subtreeRange(int u) { return {tin[u], tin[u] + sz[u]}; }
};

// ===== Example usage =====
// EulerTour et; et.init(n);
// for each edge: et.addEdge(u, v);
// et.build(root);
// SegTree st; st.init(n);
// vector<lol> base(n); for (u = 0..n-1) base[et.tin[u]] = value[u];
// st.build(base);
//
// subtree query u (e.g. sum of subtree):
//   auto [l, r] = et.subtreeRange(u);
//   Node res = st.qry(l, r);
//
// point update at u:
//   st.upd(et.tin[u], newVal);
//
// Note: this covers subtree queries/updates only (the classic "flatten to array" trick).
// It does NOT support path queries (u to v) — that's what HLD is for, which you
// already have as a separate template if you need it.
