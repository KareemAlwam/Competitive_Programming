// ================== TREE PATH UTILITIES (optional) ==================
// Paste this ONLY when a problem specifically asks for "kth node on
// path" / "node at distance d towards v" / walking along a u-v path.
// Requires tree_core.cpp's Tree struct (LCA + kthAncestor) already built.
//
// These are free functions, not struct methods, so you don't have to
// touch or bloat the core template for problems that don't need them.
// ======================================================================

// Returns the k-th node (0-indexed) on the path from u to v.
// k = 0 -> u, k = dist(u,v) -> v. Returns -1 if k is out of range.
int kthNodeOnPath(Tree &T, int u, int v, int k) {
    int w = T.lca(u, v);
    int distUV = T.depth[u] + T.depth[v] - 2 * T.depth[w];

    if (k < 0 || k > distUV) return -1;

    int upLegLen = T.depth[u] - T.depth[w]; // length of u -> w segment
    if (k <= upLegLen) {
        return T.kthAncestor(u, k);
    } else {
        int remaining = distUV - k; // steps upward from v
        return T.kthAncestor(v, remaining);
    }
}

// Node at distance d from u, walking towards v.
// d = 0 -> u, d = dist(u,v) -> v.
int nodeAtDistance(Tree &T, int u, int v, int d) {
    return kthNodeOnPath(T, u, v, d);
}

// The child of u that lies on the path towards v.
// Precondition: u must be an ancestor of v (including u == v is invalid -> returns -1).
int getChildOnPath(Tree &T, int u, int v) {
    if (u == v || !T.isAncestor(u, v)) return -1;
    int steps = T.depth[v] - T.depth[u] - 1; // guaranteed >= 0 here
    return T.kthAncestor(v, steps);
}

// True if u and v lie on the same root-to-leaf branch (one is an ancestor of the other).
bool isOnSamePath(Tree &T, int u, int v) {
    return T.isAncestor(u, v) || T.isAncestor(v, u);
}
