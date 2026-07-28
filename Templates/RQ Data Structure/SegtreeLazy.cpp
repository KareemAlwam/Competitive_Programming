// ================= LAZY SEGMENT TREE TEMPLATE =================
// Generic range-update / range-query segment tree.
// Only 3 things ever need editing for a new problem:
//   1) Node struct (what you store per segment)
//   2) merge(a, b)        -> how to combine two child nodes
//   3) applyLazy(node,...) -> how a pending update changes a node
// Everything else (build/update/query/push-down) stays the same.
// =================================================================

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// ---------- 1) NODE: what does each segment store? ----------
struct Node{
	ll sum = 0; // <-- EDIT: e.g. sum, min, max, gcd, count, etc.
							// you can store multiple fields, e.g. ll mn = INF, mx = -INF;
};

// identity element for merge (neutral value that doesn't affect result)
Node NEUTRAL = {0}; // <-- EDIT: 0 for sum, INF for min, -INF for max...

// ---------- 2) MERGE: combine two children into a parent ----------
Node merge(const Node &a, const Node &b){
	Node res;
	res.sum = a.sum + b.sum; // <-- EDIT
	// e.g. for min:  res.mn = min(a.mn, b.mn);
	// e.g. for max:  res.mx = max(a.mx, b.mx);
	return res;
}

struct LazySegTree{
	int n;
	vector<Node> tree;
	vector<ll> lazy;			// pending update value per node
	vector<bool> hasLazy; // whether a node has a pending update

	LazySegTree(int n){
		tree.assign(4 * n, Node());
		lazy.assign(4 * n, 0);
		hasLazy.assign(4 * n, false);
	}

	// ---------- 3) APPLY LAZY: effect of pending update on a node ----------
	// node covers a range of length `len`, and `val` is the pending update value
	void applyLazy(int node, int len, ll val){
		tree[node].sum += val * len; // <-- EDIT (range ADD example)
		// e.g. for range ASSIGN (set all to val): tree[node].sum = val * len;
		// e.g. for range MIN with add update:      tree[node].mn += val;

		// combine with any existing pending lazy on this node
		lazy[node] += val; // <-- EDIT if using ASSIGN instead of ADD:
											 //     lazy[node] = val;
		hasLazy[node] = true;
	}

	void pushDown(int node, int l, int r){
		if (!hasLazy[node])
			return;
		int mid = (l + r) / 2;
		applyLazy(2 * node, mid - l + 1, lazy[node]);
		applyLazy(2 * node + 1, r - mid, lazy[node]);
		lazy[node] = 0;
		hasLazy[node] = false;
	}

	void pull(int node){
		tree[node] = merge(tree[2 * node], tree[2 * node + 1]);
	}

	// ---------- build from an initial array (1-indexed input a[1..n]) ----------
	void build(vector<ll> &a, int node, int l, int r){
		if (l == r){
			tree[node].sum = a[l]; // <-- EDIT: set leaf from a[l] according to Node fields
			return;
		}
		int mid = (l + r) / 2;
		build(a, 2 * node, l, mid);
		build(a, 2 * node + 1, mid + 1, r);
		pull(node);
	}
	void build(vector<ll> &a) { build(a, 1, 1, n); }

	// ---------- range update: add `val` to every element in [ql, qr] ----------
	void update(int node, int l, int r, int ql, int qr, ll val){
		if (qr < l || r < ql)
			return;
		if (ql <= l && r <= qr){
			applyLazy(node, r - l + 1, val);
			return;
		}
		pushDown(node, l, r);
		int mid = (l + r) / 2;
		update(2 * node, l, mid, ql, qr, val);
		update(2 * node + 1, mid + 1, r, ql, qr, val);
		pull(node);
	}
	void update(int ql, int qr, ll val) { update(1, 1, n, ql, qr, val); }

	// ---------- range query on [ql, qr] ----------
	Node query(int node, int l, int r, int ql, int qr){
		if (qr < l || r < ql)
			return NEUTRAL;
		if (ql <= l && r <= qr)
			return tree[node];
		pushDown(node, l, r);
		int mid = (l + r) / 2;
		return merge(query(2 * node, l, mid, ql, qr),
								 query(2 * node + 1, mid + 1, r, ql, qr));
	}
	Node query(int ql, int qr) { return query(1, 1, n, ql, qr); }
};

// ============================= USAGE =============================
int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	int n, q;
	cin >> n >> q;

	vector<ll> a(n + 1); // 1-indexed
	for (int i = 1; i <= n; i++)
		cin >> a[i];

	LazySegTree st(n);
	st.build(a);

	while (q--){
		int type;
		cin >> type;
		if (type == 1){
			// range update: add val to [l, r]
			int l, r;
			ll val;
			cin >> l >> r >> val;
			st.update(l, r, val);
		}
		else{
			// range query on [l, r]
			int l, r;
			cin >> l >> r;
			cout << st.query(l, r).sum << "\n"; // <-- EDIT: .sum -> .mn / .mx / whatever
		}
	}
	return 0;
}
