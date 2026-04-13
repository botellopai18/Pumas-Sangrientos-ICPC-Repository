/**
 * Title: Standard Competitive Programming Template
 * Description: Plantilla base con optimización de I/O, macros comunes y typedefs esenciales para reducir tiempo de codificación.
 * Author: Pumas Sangrientos
 */

#include <bits/stdc++.h>

using namespace std;

// === Typedefs ===
typedef long long ll;
typedef long double ld;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef vector<int> vi;
typedef vector<ll> vll;

// === Macros ===
#define endl '\n'
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define FOR(i, a, b) for(int i = (a); i < (b); ++i)
#define ROF(i, a, b) for(int i = (a); i >= (b); --i)

/**
 * FAST I/O:
 * sync_with_stdio(0) desactiva la sincronización con los buffers de C (stdio).
 * cin.tie(0) desacopla cin de cout, evitando flushes innecesarios.
 * IMPORTANTE: No usar 'endl' si se requiere máxima velocidad, usar '\n'.
 */
void fast_io() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
}

void solve() {
    // Escribe tu lógica aquí
}

int main() {
    fast_io();

    int t = 1;
    // cin >> t; // Descomentar si el problema tiene múltiples casos de prueba
    while(t--) {
        solve();
    }

    return 0;
}