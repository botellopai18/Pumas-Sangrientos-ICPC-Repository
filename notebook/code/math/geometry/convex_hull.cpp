/**
 * Title: Convex Hull (Monotone Chain - Compact)
 * Author: botellot / ICPC BRO
 * Description: Computes the Convex Hull of a set of points using Andrew's Monotone Chain.
 * Extremely compact version using a double-pass loop and coordinate reversing.
 * Complexity: O(N log N) due to sorting.
 * Usage: 
 * vector<P> pts = {{0,0}, {3,0}, {3,3}, {0,3}, {1,1}};
 * vector<P> hull = get_convex_hull(pts);
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;

struct P {
    ll x, y;
    void read() { 
    cin >> x >> y; }
    P operator - (const P& b) const { return {x - b.x, y - b.y}; }
    ll operator * (const P& b) const { return x * b.y - y * b.x; }
    __int128 triangle(const P& b, const P& c) const {return (__int128)((b - *this) * (c - *this));}
    bool operator < (const P& b) const {return make_pair(x,y) < make_pair(b.x, b.y);}
};

// Funcion que recibe el set de puntos y retorna las esquinas del Convex Hull
vector<P> get_convex_hull(vector<P> points) {
    int n = points.size();
    if (n <= 1) return points;

    // 1. Ordenar de izquierda a derecha
    sort(points.begin(), points.end()); 
    vector<P> hull;
    
    // 2. Doble pasada (rep=0 para el cascaron superior, rep=1 para el inferior)
    for (int rep = 0; rep < 2; ++rep) {
        const int S = hull.size(); // S guarda el tamaño inicial de esta pasada
        
        for (P C : points) {
            // hull.size() - S >= 2 asegura no borrar puntos de la pasada anterior
            while ((int)hull.size() - S >= 2) {
                P A = hull[hull.size() - 2]; // Equivalente a hull.end()[-2]
                P B = hull[hull.size() - 1]; // Equivalente a hull.end()[-1]
                
                // Si el giro es a la derecha (o colineal), el punto B es valido
                if (A.triangle(B, C) <= 0) { 
                    break;
                }
                // Si el giro es a la izquierda, B hace una "panza" invalida y se elimina
                hull.pop_back(); 
            }
            hull.push_back(C);
        }
        hull.pop_back(); // Elimina el punto extremo duplicado antes de dar la vuelta
        reverse(points.begin(), points.end()); // Voltear los puntos para el viaje de regreso
    }
    
    return hull;
}
