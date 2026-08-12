// ================== TOPOLOGICAL SORT (Kahn's algorithm) ==================
// General-purpose ordering of a DAG such that every edge u->v has u before v.
// Not tied to any specific DP — use the order for whatever the problem needs:
// DAG DP (longest/shortest/count paths), dependency resolution, course
// scheduling, build-order problems, etc.
//
// 0-indexed nodes, 0..n-1.
//   TopoSort ts(n);
//   ts.addEdge(u, v);       // for every directed edge u -> v
//   auto order = ts.sort(); // topo order, or {} if a cycle exists
//   ts.hasCycle()           // true/false, valid after sort() is called
// ============================================================================

struct TopoSort {
    int n;
    vector<vector<int>> adj;
    vector<int> indeg;
    bool cycle = false;

    TopoSort(int n_) : n(n_) {
        adj.assign(n, {});
        indeg.assign(n, 0);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        indeg[v]++;
    }

    vector<int> sort() {
        vector<int> order;
        queue<int> q; // swap for a priority_queue if you need lexicographically
                      // smallest order (e.g. "print any valid order, smallest first")
        vector<int> deg = indeg; // keep original indeg[] intact, work on a copy
        order.reserve(n);

        for (int i = 0; i < n; i++)
            if (deg[i] == 0) q.push(i);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            order.push_back(u);
            for (int v : adj[u])
                if (--deg[v] == 0) q.push(v);
        }

        cycle = ((int)order.size() != n);
        return cycle ? vector<int>{} : order;
    }

    bool hasCycle() const { return cycle; }
};

// ================== USAGE PATTERN: DP over a DAG ==================
// Topo sort just gives you a safe processing order. Whatever you do with
// each node as you visit it in that order is a separate concern:
//
//   auto order = ts.sort();
//   // forward pass (propagate from sources):
//   for (int u : order) {
//       for (auto &[v, w] : weightedAdj[u]) {
//           dp[v] = max(dp[v], dp[u] + w);   // or min, or +=, or |=, etc.
//       }
//   }
//   // backward pass (propagate toward sinks): same order, reversed:
//   for (int i = (int)order.size() - 1; i >= 0; i--) {
//       int u = order[i];
//       for (auto &[v, w] : weightedAdj[u]) {
//           dp2[u] = max(dp2[u], dp2[v] + w);
//       }
//   }
//
// The "topo order" and the "DP recurrence" are two independent ideas —
// counting paths, longest path, reachability, and "number of ways" all
// reuse the exact same order, just with a different combine/merge step.
// =====================================================================
