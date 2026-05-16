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

