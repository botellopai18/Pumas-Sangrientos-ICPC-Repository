/*
 * Description: Basic BigInt structure for positive integers using base 10^9.
 *  * * Complexity:
 * - Addition/Subtraction: O(N) where N is number of nodes (digits/9).
 * - Multi-Int Multiply/Divide/Mod: O(N).
 * - BigInt Multiply: O(N * M) quadratic. Fails if total digits > 10^5.
 * - String Conversion: O(L) where L is string length.
 * * Limitations:
 * - Only supports non-negative integers.
 * - less_lnum assumes a >= b (no negative result support).
 * - Division and Modulo are only by 'long long' scalars, not BigInt/BigInt.
 * - Multiplication is not FFT-optimized.
 * Provides fast I/O and linear operations for small/medium inputs.
 * Author: botellot / ICPC BRO
 * Title: BigInt Operations (Unsigned)
 * * Usage:
 * BigInt a("123456789123456789"), b(100);
 * BigInt c = prod_lnum_int(a, 5);
 * ll remainder = mod_lnum_int(a, 1e9+7);
 */
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

typedef long long ll;

struct BigInt {
    static const int BASE = 1e9;
    vector<int> nodes;

    BigInt() {}
    BigInt(long long v) {
        if (v == 0) nodes.push_back(0);
        while (v > 0) {
            nodes.push_back(v % BASE);
            v /= BASE;
        }
    }

    BigInt(string s) {
        for (int i = (int)s.length(); i > 0; i -= 9) {
            if (i < 9)
                nodes.push_back(stoi(s.substr(0, i)));
            else
                nodes.push_back(stoi(s.substr(i - 9, 9)));
        }
        trim();
    }

    void trim() {
        while (nodes.size() > 1 && nodes.back() == 0)
            nodes.pop_back();
        if (nodes.empty()) nodes.push_back(0);
    }
};

// --- OPERACIONES EXTERNAS ---

// O(N) - Suma estándar con carry
BigInt sum_lnum(const BigInt& a, const BigInt& b) {
    BigInt res = a;
    int carry = 0;
    for (size_t i = 0; i < max(res.nodes.size(), b.nodes.size()) || carry; ++i) {
        if (i == res.nodes.size()) res.nodes.push_back(0);
        long long cur = (long long)res.nodes[i] + carry + (i < b.nodes.size() ? b.nodes[i] : 0);
        res.nodes[i] = cur % BigInt::BASE;
        carry = cur / BigInt::BASE;
    }
    return res;
}

// O(N) - Resta estándar (Asume a >= b)
BigInt less_lnum(const BigInt& a, const BigInt& b) {
    BigInt res = a;
    int carry = 0;
    for (size_t i = 0; i < b.nodes.size() || carry; ++i) {
        long long cur = res.nodes[i] - carry - (i < b.nodes.size() ? b.nodes[i] : 0);
        carry = cur < 0;
        if (carry) cur += BigInt::BASE;
        res.nodes[i] = (int)cur;
    }
    res.trim();
    return res;
}

// O(N) - Multiplicación por escalar
BigInt prod_lnum_int(const BigInt& a, ll v) {
    if (v == 0) return BigInt(0);
    BigInt res = a;
    ll carry = 0;
    for (size_t i = 0; i < res.nodes.size() || carry; ++i) {
        if (i == res.nodes.size()) res.nodes.push_back(0);
        long long cur = res.nodes[i] * 1LL * v + carry;
        res.nodes[i] = cur % BigInt::BASE;
        carry = cur / BigInt::BASE;
    }
    res.trim();
    return res;
}

// O(N*M) - Multiplicación BigInt * BigInt
BigInt prod_lnum(const BigInt& a, const BigInt& b) {
    BigInt res;
    res.nodes.resize(a.nodes.size() + b.nodes.size(), 0);
    for (size_t i = 0; i < a.nodes.size(); ++i) {
        ll carry = 0;
        for (size_t j = 0; j < b.nodes.size() || carry; ++j) {
            long long cur = res.nodes[i + j] +
                            a.nodes[i] * 1LL * (j < b.nodes.size() ? b.nodes[j] : 0) + carry;
            res.nodes[i + j] = cur % BigInt::BASE;
            carry = cur / BigInt::BASE;
        }
    }
    res.trim();
    return res;
}

// O(N) - División por escalar
BigInt div_lnum_int(const BigInt& a, ll v) {
    BigInt res = a;
    ll carry = 0;
    for (int i = (int)res.nodes.size() - 1; i >= 0; --i) {
        long long cur = res.nodes[i] + carry * 1LL * BigInt::BASE;
        res.nodes[i] = (int)(cur / v);
        carry = cur % v;
    }
    res.trim();
    return res;
}

// O(N) - Módulo por escalar
ll mod_lnum_int(const BigInt& a, ll v) {
    ll m = 0;
    for (int i = (int)a.nodes.size() - 1; i >= 0; --i)
        m = (a.nodes[i] + m * 1LL * BigInt::BASE) % v;
    return m;
}

// O(N) - Print formateado con ceros a la izquierda para la base
void print(const BigInt& a) {
    if (a.nodes.empty()) { printf("0\n"); return; }
    printf("%d", a.nodes.back());
    for (int i = (int)a.nodes.size() - 2; i >= 0; --i)
        printf("%09d", a.nodes[i]);
    printf("\n");
}