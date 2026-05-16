/**
 * Title: Polinomio
 * Author: KACTL / botellot / ICPC BRO
 * Description: Structure to store and manipulate real-coefficient polynomials. 
 * Provides linear-time evaluation using Horner's Method, analytical differentiation, 
 * and in-place synthetic division (Ruffini's Rule) by a known root.
 * Complexity: 
 * - Evaluation: O(N)
 * - Differentiation: O(N)
 * - Root Division: O(N)
 * Limitations:
 * - Uses `double`, so it is susceptible to floating-point precision loss and catastrophic cancellation in large degrees.
 * - `divroot` assumes x0 is an exact or highly precise root; otherwise, numerical errors accumulate in the lower coefficients.
 * Usage:
 * Poly p{{2, -3, 1}}; // Represents x^2 - 3x + 2
 * double val = p(2);  // Evaluates P(2) -> 0
 * p.diff();           // P(x) becomes 2x - 3
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

// Macro utilitaria para emular el estilo scannable de KACTL
#define sz(x) (int)(x).size()

struct Poly {
    vector<double> a;

    // Evalúa el polinomio en el punto x usando el Método de Horner 
    // Evita calcular potencias explícitas x^n, reduciendo el error numérico.
    double operator() (double x) const {
        double val = 0; 
        for (int i = sz(a); i--;) {
            val = val * x + a[i]; 
        }
        return val; 
    }

    // Calcula la derivada analítica del polinomio in-place
    // Regla de la potencia: d/dx(c * x^i) = i * c * x^(i-1)
    void diff() {
        for (int i = 1; i < sz(a); ++i) {
            a[i - 1] = i * a[i];
        }
        if (!a.empty()) {
            a.pop_back(); // El grado máximo disminuye en 1 
        }
    }

    // Divide el polinomio entre (x - x0) in-place usando la Regla de Ruffini 
    // Útil para "eliminar" una raíz ya encontrada y buscar las demás en un grado menor.
    void divroot(double x0) {
        double b = 0;
        for (int i = sz(a); i--;) { 
            double c = a[i]; 
            a[i] = b;       // Guarda el coeficiente del nuevo cociente
            b = c + b * x0; // Calcula el residuo acumulado
        }
        if (!a.empty()) {
            a.pop_back(); // Elimina el último elemento sobrante (grado se reduce en 1)
        }
    }
};
