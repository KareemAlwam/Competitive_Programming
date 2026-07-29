struct SegTree {// zero based
    int n;
    vector<long long> tree;

    void init(int size) {
        n = size;
        tree.assign(2 * n, 0);
    }

    void build(vector<long long>& arr) {
        for (int i = 0; i < n; i++)
            tree[n + i] = arr[i];
        for (int i = n - 1; i > 0; i--)
            tree[i] = tree[2*i] + tree[2*i+1];
    }

    void update(int pos, long long val) {
        pos += n;
        tree[pos] = val;
        for (pos /= 2; pos >= 1; pos /= 2)
            tree[pos] = tree[2*pos] + tree[2*pos+1];
    }

    long long query(int l, int r) { // sum in [l, r), half-open
        long long res = 0;
        for (l += n, r += n; l < r; l /= 2, r /= 2) {
            if (l & 1) res += tree[l++];
            if (r & 1) res += tree[--r];
        }
        return res;
    }
};
int main(){
  SegTree st;
  st.init(n);
  st.build(arr);
  st.update(3, 10);        // set index 3 to value 10
  long long ans = st.query(2, 7); // sum of arr[2..6]
return 0;}
