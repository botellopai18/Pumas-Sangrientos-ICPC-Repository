/**
 * Title: Hill Climbing Optimization 2D
 * Author: KACTL / botellot / ICPC BRO
 * Description: Heuristic search to minimize a 2D continuous function $f(x, y)$. 
 * It explores a 3x3 local grid around the current state with an exponentially decaying jump size.
 * Complexity: O(\log(\text{scale}) \cdot 100 \cdot 9) \approx O(1) with a small constant.
 * Limitations:
 * - Strictly requires the function to be unimodal (or locally well-behaved near `start`).
 * - Can get trapped in local minima if the surface has multiple valleys.
 * Usage:
 * auto [val, pos] = hillClimb({0, 0}, [](P p) { return p[0]*p[0] + p[1]*p[1]; });
 */

#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <cmath>

using namespace std;

// Macro utilitaria de KACTL
#define rep(i, a, b) for(int i=a; i < (b); ++i)

typedef array<double, 2> P;

template<class F> 
pair<double, P> hillClimb(P start, F f) {
    pair<double, P> cur(f(start), start);
    for (double jmp = 1e9; jmp > 1e-20; jmp /= 2) {
        rep(j, 0, 100) rep(dx, -1, 2) rep(dy, -1, 2) {
            P p = cur.second;
            p[0] += dx * jmp;
            p[1] += dy * jmp;
            cur = min(cur, make_pair(f(p), p));
        }
    }
    return cur;
}

int main() {
    cin.tie(0)->sync_with_stdio(0);

    // Queremos minimizar un paraboloide elíptico en 2D:
    // f(x, y) = (x - 5.5)^2 + (y + 3.2)^2 + 10.0
    // El mínimo global real está en: x = 5.5, y = -3.2 con un valor de 10.0
    auto f = [](P p) {
        double x = p[0], y = p[1];
        return (x - 5.5) * (x - 5.5) + (y + 3.2) * (y + 3.2) + 10.0;
    };

    cout << "Ejecutando Hill Climbing desde el origen {0.0, 0.0}..." << "\n";
    
    // Le pasamos un punto inicial cualquiera, por ejemplo {0, 0}
    auto [ans_val, ans_pos] = hillClimb({0.0, 0.0}, f);

    cout << "--- Resultados Optimizados ---" << "\n";
    cout << "Valor minimo encontrado: " << ans_val << " (Esperado: 10.0)" << "\n";
    cout << "Coordenada X optima: " << ans_pos[0] << " (Esperado: 5.5)" << "\n";
    cout << "Coordenada Y optima: " << ans_pos[1] << " (Esperado: -3.2)" << "\n";

    return 0;
}