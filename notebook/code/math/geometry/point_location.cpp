/**
 * Title: Point Location vs Line
 * Author: botellot / ICPC BRO
 * Description: Returns orientation of P3 relative to vector P1->P2.
 * - LEFT (1): Giro Antihorario (Counter-Clockwise). Producto cruz > 0.
 * - RIGHT (-1): Giro Horario (Clockwise). Producto cruz < 0.
 * - TOUCH (0): Colineales / Sobre la misma linea. Producto cruz = 0.
 * Complexity: O(1).
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;

struct P {
    ll x, y;
    P operator - (const P& b) const { return {x - b.x, y - b.y}; }
    ll operator * (const P& b) const { return x * b.y - y * b.x; }
};

int point_location(P p1, P p2, P p3) {
    ll cp = (p2 - p1) * (p3 - p1);
    if (cp < 0) return -1; // Horario / Derecha
    if (cp > 0) return 1;  // Antihorario / Izquierda
    return 0;              // Colineal
}