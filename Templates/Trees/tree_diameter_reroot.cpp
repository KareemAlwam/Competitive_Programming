// ================== TREE DIAMETER ==================
// Longest path between any two nodes in a tree (unweighted or weighted).
// Two-BFS/DFS method: from any node, find the farthest node u; from u,
// find the farthest node v. dist(u, v) is the diameter. Works because a
// tree's diameter always has one of its endpoints reachable this way.
//
// Iterative DFS (stack-safe for n up to ~2e5+), 0-indexed nodes.
// If edges are unweighted, treat every weight as 1.
// =====================================================

struct TreeDiameter {
    int n;
    vector<vector<pair<int,int>>> adj; // adj[u] = {v, weight}

    TreeDiameter(int n_) : n(n_) { adj.assign(n, {}); }

    void addEdge(int u, int v, int w = 1) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    // returns {farthestNode, distanceToIt} starting a walk from `src`
    pair<int, long long> farthest(int src) {
        vector<long long> dist(n, -1);
        vector<int> stck;
        dist[src] = 0;
        stck.push_back(src);
        int best = src;

        while (!stck.empty()) {
            int u = stck.back(); stck.pop_back();
            if (dist[u] > dist[best]) best = u;
            for (auto &[v, w] : adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + w;
                    stck.push_back(v);
                }
            }
        }
        return {best, dist[best]};
    }

    // returns {endpoint1, endpoint2, diameterLength}
    tuple<int, int, long long> diameter() {
        auto [u, d1] = farthest(0);       // any start node works, 0 is fine
        auto [v, d2] = farthest(u);       // u is guaranteed to be one diameter endpoint
        return {u, v, d2};
    }
};

// ================== REROOTING DP ==================
// Standard tree DP (post-order DFS) only gives you the answer "as seen
// from root". Rerooting gets the answer for EVERY node as if it were the
// root, in the same O(n) total time, via two passes:
//
//   Pass 1 (down, post-order): compute dp[u] = answer for u's subtree,
//     assuming u's parent doesn't exist (normal tree DP).
//   Pass 2 (up, pre-order): compute ans[u] = answer for the WHOLE tree
//     rooted at u, by combining dp[u] with a contribution passed down
//     from u's parent (everything "outside" u's subtree).
//
// The example below computes: for every node u, the maximum distance
// from u to any other node in the tree ("eccentricity"). Swap the
// combine/merge logic for whatever quantity your problem needs — the
// two-pass skeleton (down then up) stays the same.
// =====================================================

struct RerootDP {
    int n;
    vector<vector<pair<int,int>>> adj;
    vector<long long> down;   // down[u] = max dist from u into its own subtree
    vector<long long> ans;    // ans[u]  = max dist from u to ANY node in the tree
    vector<int> parent, order;

    RerootDP(int n_) : n(n_) {
        adj.assign(n, {});
        down.assign(n, 0);
        ans.assign(n, 0);
        parent.assign(n, -1);
    }

    void addEdge(int u, int v, int w = 1) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    void buildOrder(int root) {
        vector<char> visited(n, 0);
        vector<int> stck = {root};
        visited[root] = 1;
        order.clear();
        while (!stck.empty()) {
            int u = stck.back(); stck.pop_back();
            order.push_back(u);
            for (auto &[v, w] : adj[u]) {
                if (!visited[v]) {
                    visited[v] = 1;
                    parent[v] = u;
                    stck.push_back(v);
                }
            }
        }
    }

    // Pass 1: post-order (reverse of the order we discovered nodes in,
    // since order[] is a valid pre-order — children always come after
    // their parent, so processing it backwards processes children first).
    void computeDown() {
        for (int i = (int)order.size() - 1; i >= 0; i--) {
            int u = order[i];
            for (auto &[v, w] : adj[u]) {
                if (v == parent[u]) continue;
                down[u] = max(down[u], down[v] + w);
            }
        }
    }

    // Pass 2: pre-order (order[] as-is — parent must be finalized before child).
    void computeAns() {
        for (int u : order) {
            ans[u] = down[u]; // best already known from u's own subtree

            // combine with the best path going "through" the parent
            // (best of: parent's own outside-contribution, and parent's
            // OTHER children's subtrees) — this is the part that needs
            // the "best + second-best child" trick to exclude u's own
            // branch when computing what the parent passes back down.
        }

        // For the eccentricity example we need, per parent p, the best
        // and second-best `down[child]+w` among p's children, so that
        // when we push a value down to child c we can exclude c's own
        // contribution (use 2nd-best if c was the source of the best).
        vector<long long> best1(n, 0), best2(n, 0);
        vector<int> best1From(n, -1);

        for (int u : order) {
            for (auto &[v, w] : adj[u]) {
                if (v == parent[u]) continue;
                long long cand = down[v] + w;
                if (cand > best1[u]) {
                    best2[u] = best1[u];
                    best1[u] = cand;
                    best1From[u] = v;
                } else if (cand > best2[u]) {
                    best2[u] = cand;
                }
            }
        }

        vector<long long> fromParent(n, 0); // best path available via parent, excluding u's own branch
        for (int u : order) {
            int p = parent[u];
            if (p != -1) {
                int w = 0;
                for (auto &[v, ww] : adj[p]) if (v == u) { w = ww; break; }
                long long parentBest = (best1From[p] == u) ? best2[p] : best1[p];
                fromParent[u] = max(fromParent[p], parentBest) + w;
            }
            ans[u] = max(down[u], fromParent[u]);
        }
    }

    // returns ans[] : max distance from every node to the farthest node in the tree
    vector<long long> solve(int root = 0) {
        buildOrder(root);
        computeDown();
        computeAns();
        return ans;
    }
};
