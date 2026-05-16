/**
 * Title: Integer Point
 * Author: botellot / ICPC BRO
 * Description: Robust 2D Point/Vector structure using long long to avoid precision issues.
 * Includes dot product, cross product, sorting comparators.
 * Complexity: O(1) for all operations.
 * Limitations: 
 * - Does not support non-integer operations (like division or precise rotation).
 * - Watch out for overflow in cross/dot products if coordinates are > 10^9 (results fit in __int128 if needed).
 * Usage:
 * P a{1, 2}, b{3, 4}, c{5, 6};
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

    // Operaciones básicas de vectores
    P operator + (const P& b) const { return P{x + b.x, y + b.y}; }
    P operator - (const P& b) const { return P{x - b.x, y - b.y}; }
    void operator += (const P& b) { x += b.x; y += b.y; }
    void operator -= (const P& b) { x -= b.x; y -= b.y; }

    // Multiplicación por un escalar
    P operator * (ll k) const { return P{x * k, y * k}; }

    // Producto Punto (Dot Product) -> a . b = |a||b|cos(theta)
    // Utilidad: Proyecciones, ver si un ángulo es agudo (>0), recto (=0) u obtuso (<0)
    ll dot(const P& b) const { return x * b.x + y * b.y; }

    // Producto Cruz (Cross Product) -> a x b = |a||b|sin(theta)
    // Utilidad: Áreas, ver orientaciones y giros
    ll operator * (const P& b) const { return x * b.y - y * b.x; }

    // Magnitud al cuadrado (evita usar sqrt y floats)
    ll norm() const { return x * x + y * y; }

    // Comparadores (Ordena puntos y puede usar std::sort)
    // Ordena de izquierda a derecha, y abajo hacia arriba
    bool operator < (const P& b) const {
        return make_pair(x,y) < make_pair(b.x, b.y);
    }

    bool operator == (const P& b) const {
        return x == b.x && y == b.y;
    }
};

