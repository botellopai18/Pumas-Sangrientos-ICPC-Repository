/**
 * Title: Geometry Primitives (Complex Coordinates)
 * Author: CP-Algorithms / ICPC BRO
 * Description: 2D Geometry primitives using std::complex.
 * Maps (X, Y) to the complex number X + iY. 
 * Provides native vector operations, dot product, cross product, and rotations.
 * Complexity: O(1) for all primitive operations.
 */

#include <iostream>
#include <complex>
#include <vector>
#include <cmath>

using namespace std;

// Podemos usar double o int64_t dependiendo de si el problema requiere precision exacta
using ftype = double; 
using point = complex<ftype>;

// --- OPERACIONES BASICAS NATIVAS ---
// punto p = {x, y};
// p.real() -> Accede a la coordenada X
// p.imag() -> Accede a la coordenada Y
// p1 + p2  -> Suma de vectores (Traslacion)
// p1 - p2  -> Resta de vectores (Vector de p2 apuntando a p1)
// p * k    -> Multiplicacion por un escalar k (Escalamiento)

// Producto Punto (Dot Product): a . b = |a||b|cos(theta)
// Utilidad: Proyecciones, detectar angulos agudos/rectos/obtusos
ftype dot(point a, point b) {
    return (conj(a) * b).real();
}

// Producto Cruz (Cross Product): a x b = |a||b|sin(theta)
// Utilidad: Areas, orientacion (CCW), giros izquierda/derecha
ftype cross(point a, point b) {
    return (conj(a) * b).imag();
}

// Magnitud al cuadrado (Squared Magnitude)
// Si ftype es entero, esto se mantiene 100% exacto sin floats
ftype norm_sq(point a) {
    return norm(a); // std::norm regresa x^2 + y^2
}

// Magnitud real (Distancia al origen)
double magnitude(point a) {
    return abs(a); // std::abs regresa sqrt(x^2 + y^2)
}

// Distancia entre dos puntos
double dist(point a, point b) {
    return abs(a - b);
}

// --- ORIENTACION Y GIROS ---

// Orientacion de 3 puntos (CCW)
// Retorna: > 0 si c gira a la izquierda de ab, < 0 a la derecha, 0 si son colineales
ftype ccw(point a, point b, point c) {
    return cross(b - a, c - a);
}

// --- ROTACIONES (Solo si ftype es punto flotante) ---

// Angulo polar del vector en radianes (rango de -pi a pi)
double angle(point a) {
    return arg(a);
}

// Rotar un punto 'a' un angulo 'theta' (en radianes) respecto al origen
point rotate_origin(point a, double theta) {
    return a * polar(1.0, theta);
}

// Rotar un punto 'p' respecto a un centro 'pivot' un angulo 'theta'
point rotate_pivot(point p, point pivot, double theta) {
    return pivot + (p - pivot) * polar(1.0, theta);
}

void solve() {
    point a = {3.0, 4.0};
    point b = {1.0, 2.0};
    
    cout << "X: " << a.real() << ", Y: " << a.imag() << "\n";
    cout << "Suma: " << (a + b).real() << " " << (a + b).imag() << "\n";
    cout << "Producto Punto: " << dot(a, b) << "\n";
    cout << "Producto Cruz: " << cross(a, b) << "\n";
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    solve();
    return 0;
}