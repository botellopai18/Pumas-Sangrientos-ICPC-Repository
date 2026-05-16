/**
 * Description: Calcula coeficientes binomiales (nCr) usando factoriales precomputados e inversos modulares. Requiere que m sea primo.
 * Author: botellot
 * Title: Binomial Coefficients Modulo Prime
 */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int MAXN = 1000005; // Ajustar según el problema
const ll m = 1e9 + 7;    // O 998244353
ll factorial[MAXN];
ll inv_factorial[MAXN];
// Exponenciación binaria para el inverso modular O(log m)
ll power(ll base, ll exp) {
    ll res = 1;
    base %= m;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % m;
        base = (base * base) % m;
        exp /= 2;
    }
    return res;
}
ll modInverse(ll n) {
    return power(n, m - 2);
}
// Precomputo en O(MAXN)
void precompute() {
    factorial[0] = 1;
    for (int i = 1; i < MAXN; i++)
        factorial[i] = (factorial[i - 1] * i) % m;
    // Inverso del factorial final usando Fermat
    inv_factorial[MAXN - 1] = modInverse(factorial[MAXN - 1]);
    // Inversos de los demás factoriales en O(MAXN)
    for (int i = MAXN - 2; i >= 0; i--)
        inv_factorial[i] = (inv_factorial[i + 1] * (i + 1)) % m;
}
// Cálculo en O(1)
ll nCr(int n, int k) {
    if (k < 0 || k > n) return 0;
    return factorial[n] * inv_factorial[k] % m * inv_factorial[n - k] % m;
}