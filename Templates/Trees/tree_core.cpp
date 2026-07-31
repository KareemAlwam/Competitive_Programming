// ================== TREE CORE TEMPLATE ==================
// Covers: adjacency build, iterative DFS (stack-safe for n up to ~2e5+),
//         Euler tour (tin/tout) for subtree->range queries,
//         subtree size, LCA + kth ancestor via binary lifting, dist(u,v).
//
// 0-indexed nodes. Call addEdge() for every edge, then build(root).
// Complexity: O(n log n) preprocessing, O(log n) per LCA / kthAncestor query.
//
// If a problem ONLY needs Euler tour / subtree size / depth (no LCA),
// you can still use this struct as-is — the binary lifting table costs
// O(n log n) memory (~14MB at n=2e5), which is cheap enough it's not
// worth maintaining a separate lighter struct.
// =========================================================

struct Tree {
    int n, LOG;
    vector<vector<int>> adj, up;
    vector<int> depth, tin, tout, sz;
    int timer;

    Tree(int n_) : n(n_) {
        LOG = 1;
        while ((1 << LOG) <= n) LOG++;

        adj.assign(n, {});
        up.assign(LOG, vector<int>(n, -1));
        depth.assign(n, 0);
        tin.assign(n, 0);
        tout.assign(n, 0);
        sz.assign(n, 1);
        timer = 0;
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Iterative DFS. Preserves the same fill-order guarantee as the
    // recursive version: a node's full up[][] row is completed BEFORE
    // any of its children are discovered, so up[i-1][up[i-1][v]] is
    // always already valid when we compute up[i][v].
    void build(int root = 0) {
        vector<int> parent(n, -1), childIdx(n, 0);
        vector<char> visited(n, 0);
        vector<int> stck;
        stck.reserve(n);

        auto initNode = [&](int u, int p, int d) {
            visited[u] = 1;
            parent[u] = p;
            depth[u] = d;
            up[0][u] = p;
            for (int i = 1; i < LOG; i++) {
                int mid = up[i - 1][u];
                up[i][u] = (mid == -1) ? -1 : up[i - 1][mid];
            }
            tin[u] = ++timer;
        };

        initNode(root, -1, 0);
        stck.push_back(root);

        while (!stck.empty()) {
            int u = stck.back();
            if (childIdx[u] < (int)adj[u].size()) {
                int v = adj[u][childIdx[u]++];
                if (visited[v]) continue; // parent or already-processed edge
                initNode(v, u, depth[u] + 1);
                stck.push_back(v);
            } else {
                tout[u] = timer;
                stck.pop_back();
                if (parent[u] != -1) sz[parent[u]] += sz[u];
            }
        }
    }

    // ---------- Basic queries ----------
    int kthAncestor(int u, int k) {
        if (k < 0) return -1;
        for (int i = 0; i < LOG && u != -1; i++) {
            if (k & (1 << i)) u = up[i][u];
        }
        return u;
    }

    int lca(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        u = kthAncestor(u, depth[u] - depth[v]);
        if (u == v) return u;

        for (int i = LOG - 1; i >= 0; i--) {
            if (up[i][u] != up[i][v]) {
                u = up[i][u];
                v = up[i][v];
            }
        }
        return up[0][u];
    }

    int dist(int u, int v) {
        int w = lca(u, v);
        return depth[u] + depth[v] - 2 * depth[w];
    }

    // ---------- Ancestor / subtree helpers (require Euler tour) ----------
    bool isAncestor(int u, int v) {
        return tin[u] <= tin[v] && tout[v] <= tout[u];
    }

    int subtreeSize(int u) {
        return sz[u];
    }
};
