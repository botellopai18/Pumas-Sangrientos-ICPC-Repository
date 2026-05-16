/**
 * Title: Numerical Integration (Simpson's 1/3 Rule)
 * Author: KACTL / botellot / ICPC BRO
 * Description: Approximates the definite integral $\int_{a}^{b} f(x) dx$ (area under the curve) 
 * using Simpson's 1/3 rule by fitting parabolas over $2n$ subintervals.
 * Complexity: O(N) where N is the number of intervals.
 * Limitations:
 * - The function 'f' must be continuous and smooth within $[a, b]$. 
 * - If the function has sharp spikes, discontinuities, or vertical asymptotes, 
 * standard Simpson's rule will lose stability. In those cases, use Adaptive Integration instead.
 * Usage:
 * double area = quad(0, M_PI, [](double x) { return sin(x); }); // Returns 2.0
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

#define rep(i, a, b) for(int i=a; i < (b); ++i)

// Algoritmo de Integracion de KACTL
template<class F>
double quad(double a, double b, F f, const int n = 1000) { //
    double h = (b - a) / 2 / n, v = f(a) + f(b); //
    rep(i, 1, n * 2) //
        v += f(a + i * h) * (i & 1 ? 4 : 2); //
    return v * h / 3; //
}

// --- CÓDIGO DE PRUEBA PARA TU TERMINAL EN MAC M2 ---
int main() {
    cin.tie(0)->sync_with_stdio(0);

    // Prueba 1: Una funcion cuadratura simple donde sabemos el area exacta
    // Integral de f(x) = 3x^2 desde x=0 hasta x=2
    // El area exacta por calculo es: [x^3] de 0 a 2 = 2^3 - 0^3 = 8.0
    auto f1 = [](double x) { return 3 * x * x; };
    
    cout << "--- Test 1: Funcion Polinomial ---" << "\n";
    cout << "Area de 3x^2 de 0 a 2: " << quad(0, 2, f1) << " (Esperado: 8.0)" << "\n\n";

    // Prueba 2: Una curva trigonometrica clasica
    // Integral de f(x) = sin(x) desde x=0 hasta x=PI
    // El area exacta bajo una montaña de seno es exactamente 2.0
    auto f2 = [](double x) { return sin(x); };

    cout << "--- Test 2: Funcion Trigonometrica ---" << "\n";
    cout << "Area de sin(x) de 0 a PI: " << quad(0, M_PI, f2) << " (Esperado: 2.0)" << "\n";

    return 0;
}