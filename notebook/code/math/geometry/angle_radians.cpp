/*
Title: Angle <-> radians
*/
#include <cmath>

const double PI = acos(-1.0);

// Convierte radianes a grados
double rad_to_deg(double rad) {
    return rad * 180.0 / PI;
}

// Convierte grados a radianes (por si la necesitas al revés)
double deg_to_rad(double deg) {
    return deg * PI / 180.0;
}