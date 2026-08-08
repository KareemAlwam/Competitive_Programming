// ================== ORDERED MULTISET (PBDS) TEMPLATE ==================
// GNU PBDS order-statistics tree, multiset version (duplicates allowed).
// GCC/GNU-only (works on Codeforces, most ICPC-style judges). Do not
// port to non-GCC compilers.
//
// Core operations, all O(log n):
//   insert(x)              - add x
//   order_of_key(x)        - # elements strictly less than x
//   find_by_order(k)       - iterator to the (k+1)-th smallest element (0-indexed)
//   eraseOne(t, x)         - safely erase ONE occurrence of x (checked)
//   kthSmallest(t, k)      - k-th smallest, 0-indexed, or -1 if out of range
// =========================================================================

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

typedef tree<int, null_type, std::less_equal<int>, rb_tree_tag,
             tree_order_statistics_node_update> ordered_multiset;

// Safely erase exactly ONE occurrence of v from t.
// No-op (returns false) if v is not present
bool eraseOne(ordered_multiset &t, int v) {
    int rankOfV = t.order_of_key(v);            // # elements < v
    auto it = t.find_by_order(rankOfV);          // candidate: first element >= v
    if (it == t.end() || *it != v) return false;  // v not actually in t
    t.erase(it);
    return true;
}

int countLess(ordered_multiset &t, int x) {
    return t.order_of_key(x);
}
int countLessEqual(ordered_multiset &t, int x) {
    return t.order_of_key(x + 1);
}
int countEqual(ordered_multiset &t, int x) {
    return t.order_of_key(x + 1) - t.order_of_key(x);
}
// k-th smallest element, 0-indexed. Returns -1 if k is out of range.
int kthSmallest(ordered_multiset &t, int k) {
    if (k < 0 || k >= (int)t.size()) return -1;
    return *t.find_by_order(k);
}

// ================== COMMON USE CASES ==================
//
// 1) INVERSION COUNT (pairs i<j with a[i] > a[j]), processing left to right:
//      ordered_multiset t;
//      long long inversions = 0;
//      for (int i = 0; i < n; i++) {
//          inversions += (long long)(i - countLessEqual(t, a[i])); // # earlier elems > a[i]
//          t.insert(a[i]);
//      }
//
//    NOTE: insert-then-query gives a DIFFERENT count (elements before i
//    that are < a[i], not inversions). Query BEFORE inserting for
//    inversions, as shown above. This is the single easiest mistake to
//    make with this template under time pressure -- decide which count
//    you need and match the insert/query order accordingly.
//
// 2) COUNT SMALLER-THAN-CURRENT SO FAR (e.g. some counting DP variants):
//      ordered_multiset t;
//      for (int i = 0; i < n; i++) {
//          long long smallerBefore = countLess(t, a[i]);
//          t.insert(a[i]);
//      }
//
// 3) K-TH SMALLEST UNDER INSERT/DELETE (order-statistics with updates):
//      ordered_multiset t;
//      t.insert(x);
//      eraseOne(t, x);
//      int median = kthSmallest(t, t.size() / 2);
//
// =========================================================================
