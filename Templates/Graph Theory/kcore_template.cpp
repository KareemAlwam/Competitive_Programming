// k-core peeling: repeatedly strip nodes with degree < K, decrementing
// neighbors and requeuing newly-eligible ones. Your tree-leaf-pruning
// problem is the special case K=2 restricted to a tree.
struct KCore {
    int n, K;
    vector<vector<int>> adj;
    vector<int> deg;
    vector<bool> removed;

    KCore(int n_, int K_) : n(n_), K(K_) {
        adj.assign(n, {});
        deg.assign(n, 0);
        removed.assign(n, false);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
        deg[u]++; deg[v]++;
    }

    // peels everything with degree < K until stable, OR stops early
    // after exactly `limit` removals if limit >= 0 (your use case).
    vector<int> peel(int limit = -1) {
        queue<int> q;
        for (int i = 0; i < n; i++) if (deg[i] < K) q.push(i);

        vector<int> order;
        while (!q.empty() && (limit < 0 || (int)order.size() < limit)) {
            int u = q.front(); q.pop();
            if (removed[u]) continue;
            removed[u] = true;
            order.push_back(u);
            for (int v : adj[u]) {
                if (removed[v]) continue;
                if (--deg[v] == K - 1) q.push(v);
            }
        }
        return order; // nodes in removal order
    }
};
