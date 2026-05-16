/**
 * Title: Polygon Area (Shoelace Formula)
 * Author: botellot / ICPC BRO
 * Description: Computes twice the area of a simple polygon (convex or concave).
 * Returns 2 * Area as long long to maintain absolute integer precision.
 * Vertices must be given in clockwise or counter-clockwise order.
 * Complexity: O(N) where N is the number of vertices.
 * Usage:
 * vector<P> poly = {{0,0}, {4,0}, {4,3}};
 * ll double_area = polygon_double_area(poly); // Returns 12
 * double real_area = double_area / 2.0;       // 6.0
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;
typedef long long ll;

struct P {
    ll x, y;
    void read() {cin >> x >> y;}
    ll operator * (const P& b) const {return x * b.y - y * b.x;}
};

// Retorna 2 * Area del poligono usando el producto cruz desde el origen
ll polygon_double_area(const vector<P>& poly) {
    int n = (int)poly.size();
    ll area = 0LL;
    for (int i = 0; i < n; i++) {
        int next_idx = (i + 1 == n) ? 0 : i + 1;
        area += poly[i] * poly[next_idx];
    }
    return abs(area);
}

void solve() {
    int n; cin >> n;
    vector<P> polygon(n);
    for(P& p : polygon) {
        p.read();
    }
    ll area = polygon_double_area(polygon);
}
