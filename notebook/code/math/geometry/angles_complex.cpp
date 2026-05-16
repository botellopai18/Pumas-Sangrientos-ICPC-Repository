/**
 * Title: Angle Between Segments (Complex Coordinates)
 * Author: CP-Algorithms / ICPC BRO
 * Description: Computes the directed angle from segment AB to segment CD.
 * Returns the result in both radians and degrees.
 * Complexity: O(1).
 */
#include <iostream>
#include <complex>
#include <cmath>
using namespace std;

using ftype = double; 
using point = complex<ftype>;

const double PI = acos(-1.0);

// Retorna el ángulo dirigido desde v1 hacia v2 en radianes (rango de -PI a PI)
// Un valor positivo significa giro antihorario, negativo significa giro horario
double angle_between_vectors(point v1, point v2) {
    // La magia de los complejos: arg(conj(v1) * v2) 
    // es equivalente a hacer atan2(cross(v1,v2), dot(v1,v2))
    return arg(conj(v1) * v2);
}

// Retorna el ángulo dirigido del segmento AB al segmento CD
double angle_between_segments(point a, point b, point c, point d) {
    point v1 = b - a; // Vector del primer segmento
    point v2 = d - c; // Vector del segundo segmento
    return angle_between_vectors(v1, v2);
}

void solve() {
    // Segmento 1: De (0,0) a (2,0) -> Vector horizontal a la derecha (0 grados)
    point a = {0.0, 0.0}, b = {2.0, 0.0};
    
    // Segmento 2: De (0,0) a (0,2) -> Vector vertical hacia arriba (90 grados)
    point c = {0.0, 0.0}, d = {0.0, 2.0};
    
    double rad = angle_between_segments(a, b, c, d);
    double deg = rad * 180.0 / PI;
    
    cout << "Angulo en Radianes: " << rad << "\n"; // Debería dar PI / 2 (~1.5708)
    cout << "Angulo en Grados: " << deg << "\n";   // Debería dar 90.0
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    solve();
    return 0;
}