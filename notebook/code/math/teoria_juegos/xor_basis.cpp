/**
 * Title: XOR Basis (Linear Basis)
 * Author: ICPC BRO / botellot
 * Description: Maintains a set of linearly independent vectors under XOR.
 * Useful for maximum/minimum XOR subset, counting distinct XOR sums, and membership queries.
 * Complexity: O(log(MAX_VAL)) per insertion. Max size is ~60 for long long.
 */

#include <iostream>
#include <vector>

using namespace std;

struct XorBasis {
    long long basis[62];
    int sz = 0; // Guarda cuantos elementos reales tiene la base

    XorBasis() {
        for (int i = 0; i < 62; ++i) basis[i] = 0;
    }

    // Inserta un numero a la base. Retorna true si aporto informacion nueva.
    bool insert(long long x) {
        for (int i = 61; i >= 0; --i) {
            if ((x >> i) & 1) { // Si el bit 'i' esta encendido
                if (!basis[i]) { // Si la ranura esta vacia, lo insertamos
                    basis[i] = x;
                    sz++;
                    return true;
                }
                x ^= basis[i]; // Si ya hay alguien, reducimos x y seguimos
            }
        }
        return false; // x se volvio 0, no aporto nada nuevo
    }

    // Retorna el MAXIMO XOR posible que se puede formar con cualquier subconjunto
    long long getMaxXor() {
        long long res = 0;
        for (int i = 61; i >= 0; --i) {
            // Greedy: si prender el bit 'i' aumenta nuestro resultado, lo tomamos
            if ((res ^ basis[i]) > res) {
                res ^= basis[i];
            }
        }
        return res;
    }

    // Verifica si un numero 'x' se puede formar con los elementos de la base
    bool canForm(long long x) {
        for (int i = 61; i >= 0; --i) {
            if ((x >> i) & 1) {
                if (!basis[i]) return false; // Falto un vector para apagar este bit
                x ^= basis[i];
            }
        }
        return x == 0;
    }
};

