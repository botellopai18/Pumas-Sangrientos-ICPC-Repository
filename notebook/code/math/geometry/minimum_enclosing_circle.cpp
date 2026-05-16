/**
 * Title: Minimum Enclosing Circle (Welzl's Algorithm)
 * Author: CP-Algorithms / ICPC BRO
 * Description: Computes the Minimum Enclosing Circle (MEC) in expected O(N) time.
 * Uses std::variant and std::complex for zero-allocation performance and clean logic.
 * Necesita c++17 para variant, visit y chrono
 * Complexity: Expected O(N).
 * Limitations: Coordinates must fit within int64_t operations inside indicator().
 */

#include <iostream>
#include <vector>
#include <complex>
#include <variant>
#include <array>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

using ftype = int64_t;
using point = complex<ftype>;

// El circulo minimo siempre esta representado por 2 o 3 puntos en su circunferencia
using mec = variant<
    array<point, 2>,
    array<point, 3>
>;

/* I < 0 si z esta dentro de C,
   I > 0 si z esta fuera de C,
   I = 0 si z esta exactamente en la circunferencia de C */
ftype indicator(mec const& C, point z) {
    return visit([&](auto &&C) {
        point a = C[0], b = C[1];
        // La parte real de Multiplicar un numero complejo por el conjugado de otro (conj) 
        // es exactamente igual al producto punto de ambos vectores.
        point I0 = (b - z) * conj(a - z);
        if constexpr (C.size() == 2) {
            return real(I0);
        } else {
            point c = C[2];
            point I2 = (a - c) * conj(b - c);
            point I1 = I0 * I2;
            return imag(I2) < 0 ? -imag(I1) : imag(I1);
        }
    }, C);
}

bool inside(mec const& C, point p) {
    return indicator(C, p) <= 0;
}

// Inicializacion del generador con un seed basado en tiempo real para evitar hacks
mt19937_64 gen(chrono::steady_clock::now().time_since_epoch().count());

mec enclosing_circle(vector<point> &p) {
    int n = p.size();
    if (n == 0) return array<point, 2>{point{0, 0}, point{0, 0}};
    
    // El shuffle aleatorio es lo que garantiza el tiempo esperado O(N)
    shuffle(p.begin(), p.end(), gen);
    
    // Inicializar de forma segura con el primer punto duplicado (Soporta N=1 perfectamente)
    mec C = array<point, 2>{p[0], p[0]};
    
    for (int i = 0; i < n; i++) {
        if (!inside(C, p[i])) {
            C = array<point, 2>{p[i], p[0]};
            for (int j = 0; j < i; j++) {
                if (!inside(C, p[j])) {
                    C = array<point, 2>{p[i], p[j]};
                    for (int k = 0; k < j; k++) {
                        if (!inside(C, p[k])) {
                            C = array<point, 3>{p[i], p[j], p[k]};
                        }
                    }
                }
            }
        }
    }
    return C;
}

// --- BONUS: Como recuperar el centro y el radio en punto flotante si el problema lo pide ---
pair<complex<double>, double> get_circle_geometry(mec const& C) {
    return visit([&](auto &&C) -> pair<complex<double>, double> {
        if constexpr (C.size() == 2) {
            complex<double> a(real(C[0]), imag(C[0]));
            complex<double> b(real(C[1]), imag(C[1]));
            complex<double> center = (a + b) / 2.0;
            double radius = abs(a - b) / 2.0;
            return {center, radius};
        } else {
            complex<double> a(real(C[0]), imag(C[0]));
            complex<double> b(real(C[1]), imag(C[1]));
            complex<double> c(real(C[2]), imag(C[2]));
            // Formula del circuncentro usando numeros complejos
            complex<double> d = (a - b) * conj(c - b);
            complex<double> center = b + (a - b) * complex<double>(0, 1) * (c - b) * conj(c - a) / (2.0 * d.imag());
            double radius = abs(a - center);
            return {center, radius};
        }
    }, C);
}

void solve() {
    int n;
    if (!(cin >> n)) return;
    
    vector<point> orig_points(n);
    for (int i = 0; i < n; i++) {
        ftype x, y;
        cin >> x >> y;
        orig_points[i] = point{x, y};
    }
    
    // Sacamos una copia para que Welzl la barajee sin perder el orden original
    vector<point> working_points = orig_points;
    mec C = enclosing_circle(working_points);
    
}
