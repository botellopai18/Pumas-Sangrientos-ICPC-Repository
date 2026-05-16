/**
 * Title: Lattice Points (Pick's Theorem)
 * Author: botellot / ICPC BRO
 * Description: Computes the number of interior (I) and boundary (B) lattice points 
 * of a simple polygon with integer coordinates using Pick's Theorem (A = I + B/2 - 1).
 * Complexity: O(N log(max_coord)) due to GCD for each edge.
 * Usage:
 * vector<P> poly = {{0,0}, {0,3}, {4,0}};
 * auto [interior, boundary] = get_lattice_points(poly); // Returns {0, 12}
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric> // Contiene std::gcd para C++17

using namespace std;
typedef long long ll;

struct P {
    ll x, y;
    void read() { cin >> x >> y; }
    P operator - (const P& b) const { return {x - b.x, y - b.y}; }
    // __int128 para evitar overflow en coordenadas grandes durante el producto cruz
    __int128 operator * (const P& b) const { return (__int128)x * b.y - (__int128)y * b.x; }
};

// Funcion iterativa de GCD para asegurar compatibilidad absoluta y velocidad
ll safe_gcd(ll a, ll b) {
    a = abs(a); b = abs(b);
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

// Retorna un pair: {puntos_interiores, puntos_en_el_borde}
pair<ll, ll> get_lattice_points(const vector<P>& poly) {
    int n = poly.size();
    __int128 double_area = 0;
    ll boundary = 0;

    for (int i = 0; i < n; i++) {
        int j = (i + 1 == n) ? 0 : i + 1;
        
        // 1. Acumular el doble del area (Shoelace)
        double_area += poly[i] * poly[j];
        
        // 2. Contar puntos enteros en el segmento actual usando GCD
        P diff = poly[j] - poly[i];
        boundary += safe_gcd(diff.x, diff.y);
    }

    // Asegurar que el area sea positiva
    if (double_area < 0) double_area = -double_area;

    // 3. Aplicar Teorema de Pick despejando los puntos interiores (I)
    // Formula original: A = I + B/2 - 1  ==>  2A = 2I + B - 2  ==>  2I = 2A - B + 2
    ll interior = (ll)(double_area - boundary + 2) / 2;

    return {interior, boundary};
}

void solve() {
    int n; 
    if (!(cin >> n)) return;
    
    vector<P> polygon(n);
    for (int i = 0; i < n; i++) {
        polygon[i].read();
    }
    
    pair<ll, ll> ans = get_lattice_points(polygon);
    cout << ans.first << " " << ans.second << "\n";
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    int t = 1;
    // cin >> t; // Descomentar si el problema tiene multiples casos
    while (t--) {
        solve();
    }
    return 0;
}