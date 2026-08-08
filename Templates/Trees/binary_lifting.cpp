// ================== BINARY LIFTING (functional graph) ==================
// General-purpose "where do I end up after k steps" doubling technique.
// Works on ANY functional graph (each node has exactly one outgoing edge,
// e.g. nxt[i] = successor of i) -- not tied to trees. For tree parent-
// pointer binary lifting (LCA), see tree_core.cpp instead.
//
// BinaryLifting bl(nxt, maxK);  // nxt[i] = next node from i, or -1 if none
// bl.jump(u, k)                  -> node reached after exactly k steps, O(log k)
//
// IMPORTANT: maxK must be >= the largest k you will ever query. The table is
// only sized to cover k up to maxK -- if you query a bigger k than you told
// the constructor about, you'll silently get a WRONG answer, not a crash.
// Pass the problem's actual bound on k (e.g. 1e9, 1e18), not n, unless you
// know k is bounded by n in your specific problem.
// ==========================================================================

struct BinaryLifting {
    int n, LOG;
    vector<vector<int>> up;

    BinaryLifting(vector<int> &nxt, long long maxK) : n(nxt.size()) {
        LOG = 1;
        while ((1LL << LOG) < max(maxK, 1LL)) LOG++;
        LOG++;

        up.assign(LOG, vector<int>(n));
        up[0] = nxt;
        for (int k = 1; k < LOG; k++)
            for (int i = 0; i < n; i++)
                up[k][i] = (up[k - 1][i] == -1) ? -1 : up[k - 1][up[k - 1][i]];
    }

    int jump(int u, long long k) {
        for (int i = 0; i < LOG && u != -1; i++)
            if ((k >> i) & 1) u = up[i][u];
        return u;
    }
};

// ================== VARIANT: with path aggregate (e.g. min) ==================
// Same idea, but also tracks an aggregate (min shown here -- swap for max/sum
// as needed, sum just needs the merge to be `+` and NONE handling adjusted)
// over the k edges walked. Common pattern: "min value on the path of k jumps",
// "does a forbidden node/weight appear within k steps", etc.
//
// BinaryLiftingMin bl(nxt, w, maxK);   // w[i] = weight of the edge FROM node i
// auto [endNode, minW] = bl.jump(u, k);
// Same maxK caveat as BinaryLifting above.
// ================================================================================

struct BinaryLiftingMin {
    int n, LOG;
    vector<vector<int>> up;
    vector<vector<long long>> mn;
    static const long long INF = LLONG_MAX;

    BinaryLiftingMin(vector<int> &nxt, vector<long long> &w, long long maxK) : n(nxt.size()) {
        LOG = 1;
        while ((1LL << LOG) < max(maxK, 1LL)) LOG++;
        LOG++;

        up.assign(LOG, vector<int>(n));
        mn.assign(LOG, vector<long long>(n));
        up[0] = nxt;
        mn[0] = w;
        for (int k = 1; k < LOG; k++)
            for (int i = 0; i < n; i++) {
                int mid = up[k - 1][i];
                up[k][i] = (mid == -1) ? -1 : up[k - 1][mid];
                mn[k][i] = (mid == -1) ? mn[k - 1][i] : min(mn[k - 1][i], mn[k - 1][mid]);
            }
    }

    pair<int, long long> jump(int u, long long k) {
        long long best = INF;
        for (int i = 0; i < LOG && u != -1; i++)
            if ((k >> i) & 1) {
                best = min(best, mn[i][u]);
                u = up[i][u];
            }
        return {u, best};
    }
};
