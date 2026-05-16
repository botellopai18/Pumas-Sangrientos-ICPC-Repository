/**
 * Title: Segment Intersection (Anti-Overflow Version)
 * Author: botellot / ICPC BRO
 * Description: Robustly checks if line segments P1P2 and P3P4 intersect.
 * Uses __int128 in cross product to strictly avoid overflow with coordinates up to 10^9.
 * Handles general intersections, T-junctions, and collinear/overlapping segments.
 * Complexity: O(1).
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;

struct P {
    ll x, y;
    
    void read() {
        cin >> x >> y;
    }
    
    P operator - (const P& b) const { 
        return {x - b.x, y - b.y}; 
    }
    
    // Retorna __int128 para blindar multiplicaciones de coordenadas de hasta 10^9
    __int128 operator * (const P& b) const { 
        return (__int128)x * b.y - (__int128)y * b.x; 
    }
};

bool intersect(P p1, P p2, P p3, P p4) {
    // Caso 1: Los segmentos son paralelos o colineales
    if ((p4 - p3) * (p2 - p1) == 0) {
        // Si no son colineales (paralelos en distintas lineas), no hay interseccion
        if ((p2 - p1) * (p3 - p1) != 0) return false;
        
        // Si son colineales, revisamos el traslape de sus cajas contenedoreras
        for (int rep = 0; rep < 2; rep++) {
            if (max(p1.x, p2.x) < min(p3.x, p4.x) || max(p1.y, p2.y) < min(p3.y, p4.y)) {
                return false;
            }
            swap(p1, p3); swap(p2, p4);
        }
        return true; 
    }

    // Caso 2: No paralelos (Straddle Test con loop y swap)
    for (int rep = 0; rep < 2; rep++) {
        // s1 y s2 guardan los signos de orientacion usando __int128
        __int128 s1 = (p2 - p1) * (p3 - p1);
        __int128 s2 = (p2 - p1) * (p4 - p1);
        
        // Se descarta si ambos puntos estan estrictamente del mismo lado
        if ((s1 < 0 && s2 < 0) || (s1 > 0 && s2 > 0)) return false;
        
        swap(p1, p3); swap(p2, p4);
    }

    return true;
}

void solve() {
    P p1, p2, p3, p4;
    p1.read(); p2.read(); p3.read(); p4.read();
    
    if (intersect(p1, p2, p3, p4)) {
        cout << "YES\n";
    } else {
        cout << "NO\n";
    }
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    int t;
    if (cin >> t) {
        while (t--) solve();
    }
    return 0;
}