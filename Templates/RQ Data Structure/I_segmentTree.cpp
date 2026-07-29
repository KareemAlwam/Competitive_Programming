struct Node {
  lol ans = 0;
};
struct SegTree {
  int n;
  vector<Node> seg;
  void init(int sz) { n = sz; seg.assign(2 * n, Node()); }
  inline Node merge(const Node &a, const Node &b) {
    Node res;
    res.ans = a.ans + b.ans;
    return res;
  }
  void build(vector<long long>& arr) {
    for (int i = 0; i < n; i++) seg[n + i].ans = arr[i];
    for (int i = n - 1; i; i--)
      seg[i] = merge(seg[i << 1], seg[i << 1 | 1]);
  }
  void upd(int p, const Node &v) {
    for (seg[p += n] = v; p >>= 1;)
      seg[p] = merge(seg[p << 1], seg[p << 1 | 1]);
  }
  Node qry(int l, int r) {      // [l,r)
    Node L, R;
    for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
      if (l & 1) L = merge(L, seg[l++]);
      if (r & 1) R = merge(seg[--r], R);
    }
    return merge(L, R);
  }
};
void solve() {
  int n, q; cin >> n >> q;
  vector<lol> arr(n); vscan(arr);
  SegTree st;
  st.init(n);
  st.build(arr);
  while (q--) {
    int l, r; cin >> l >> r;
    cout << st.qry(l - 1, r).ans << "\n";
  }
}
