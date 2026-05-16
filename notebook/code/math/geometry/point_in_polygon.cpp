/**
 * Title: Point in Polygon (Ray Casting)
 * Author: botellot / ICPC BRO
 * Description: Checks if a point P is INSIDE (1), OUTSIDE (0), or on the BOUNDARY (2) of a simple polygon.
 * Uses a ray to infinity shifted by X+1 to completely avoid vertex-intersection edge cases.
 * Needs vertices of polygon ordered clockwise or anticlockwise.
 * Complexity: O(N) per query, where N is the number of polygon vertices.
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;

struct P {
    ll x, y;
    void read() { cin >> x >> y; }
    P operator - (const P& b) const { return {x - b.x, y - b.y}; }
    __int128 operator * (const P& b) const { return (__int128)x * b.y - (__int128)y * b.x; }
};

// Determina si el punto c esta sobre el segmento ab
bool in_segment(P a, P b, P c) {
    if ((a - c) * (b - c) != 0) return false; // No son colineales
    return min(a.x, b.x) <= c.x && c.x <= max(a.x, b.x) &&
           min(a.y, b.y) <= c.y && c.y <= max(a.y, b.y);
}

// Interseccion de segmentos
bool intersect(P p1, P p2, P p3, P p4) {
    if ((p4 - p3) * (p2 - p1) == 0) {
        if ((p2 - p1) * (p3 - p1) != 0) return false;
        for (int rep = 0; rep < 2; rep++) {
            if (max(p1.x, p2.x) < min(p3.x, p4.x) || max(p1.y, p2.y) < min(p3.y, p4.y)) return false;
            swap(p1, p3); swap(p2, p4);
        }
        return true;
    }
    for (int rep = 0; rep < 2; rep++) {
        __int128 s1 = (p2 - p1) * (p3 - p1);
        __int128 s2 = (p2 - p1) * (p4 - p1);
        if ((s1 < 0 && s2 < 0) || (s1 > 0 && s2 > 0)) return false;
        swap(p1, p3); swap(p2, p4);
    }
    return true;
}

// Retorna: 0 = OUTSIDE, 1 = INSIDE, 2 = BOUNDARY
int point_in_polygon(const vector<P>& poly, P p) {
    int n = poly.size();
    int cnt = 0;
    
    // Rayo al "infinito". El truco de p.x + 1 evita que el rayo cruce exactamente
    // por un vertice entero del poligono, eliminando bugs de doble conteo.
    P out = {p.x + 1, 2000000007LL}; 
    
    for (int i = 0; i < n; i++) {
        int j = (i + 1 == n) ? 0 : i + 1;
        
        // 1. Si el punto esta sobre el borde de este segmento, terminar de inmediato
        if (in_segment(poly[i], poly[j], p)) {
            return 2; 
        }
        
        // 2. Contar intersecciones con el rayo
        if (intersect(p, out, poly[i], poly[j])) {
            cnt++;
        }
    }
    
    // Impar = INSIDE, Par = OUTSIDE
    return (cnt % 2 == 1) ? 1 : 0;
}

void solve() {
    int n, m; 
    if (!(cin >> n >> m)) return;
    
    vector<P> polygon(n);
    for (int i = 0; i < n; i++) polygon[i].read();
    
    while (m--) {
        P p; p.read();
        int res = point_in_polygon(polygon, p);
        if (res == 2) cout << "BOUNDARY\n";
        else if (res == 1) cout << "INSIDE\n";
        else cout << "OUTSIDE\n";
    }
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0);
    solve();
    return 0;
}