#include <bits/stdc++.h>
using namespace std;

struct MergeSortTree {
    int n;               // size of original array (not power of two)
    int size;            // power of two >= n
    vector<vector<int>> tree;

    MergeSortTree(const vector<int>& a) {
        n = (int)a.size();
        size = 1;
        while (size < n) size <<= 1;
        tree.assign(2 * size, {});

        // place leaves at positions [size, size+n)
        for (int i = 0; i < n; ++i)
            tree[size + i] = {a[i]};

        // build internal nodes iteratively
        for (int i = size - 1; i >= 1; --i) {
            const auto& L = tree[i << 1];
            const auto& R = tree[i << 1 | 1];
            tree[i].resize(L.size() + R.size());
            merge(L.begin(), L.end(), R.begin(), R.end(), tree[i].begin());
        }
    }

    // count numbers in [c, d] among a[l..r] inclusive
    int query(int l, int r, int c, int d) const {
        if (l > r) return 0;
        int res = 0;
        l += size;
        r += size + 1;    // convert to half‑open [l, r+1)

        while (l < r) {
            if (l & 1) {
                const auto& vec = tree[l++];
                res += upper_bound(vec.begin(), vec.end(), d) -
                       lower_bound(vec.begin(), vec.end(), c);
            }
            if (r & 1) {
                const auto& vec = tree[--r];
                res += upper_bound(vec.begin(), vec.end(), d) -
                       lower_bound(vec.begin(), vec.end(), c);
            }
            l >>= 1;
            r >>= 1;
        }
        return res;
    }
};

void solve() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> x(n);
    for (int& v : x) cin >> v;

    MergeSortTree mst(x);

    while (q--) {
        int a, b, c, d;
        cin >> a >> b >> c >> d;
        --a; --b;   // convert to 0‑based inclusive
        cout << mst.query(a, b, c, d) << '\n';
    }
}
