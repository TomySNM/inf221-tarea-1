// INF-221 Tarea 1 2026-2 | Nombre: Tomás San Martín | Rol: 202473565-9
// Algoritmo: Merge Sort
// Referencias:
//  [1] Cormen, Leiserson, Rivest, Stein. "Introduction to Algorithms" (CLRS), Sec 2.3.1.
//  [2] https://en.wikipedia.org/wiki/Merge_sort
// Implementación basada y adaptada con asistencia de IA (Qwen)

#include "sorting.hpp"

namespace {
    // Fusión de dos mitades ya ordenadas usando un buffer auxiliar
    void merge(IntArray& v, IntArray& tmp, size_t lo, size_t mid, size_t hi) {
        for (size_t k = lo; k < hi; ++k) tmp[k] = v[k];
        
        size_t i = lo, j = mid;
        for (size_t k = lo; k < hi; ++k) {
            if (i < mid && (j >= hi || tmp[i] <= tmp[j])) {
                v[k] = tmp[i++];
            } else {
                v[k] = tmp[j++];
            }
        }
    }

    void rec(IntArray& v, IntArray& tmp, size_t lo, size_t hi) {
        if (hi - lo < 2) return; // Caso base: 1 o 0 elementos
        
        size_t mid = lo + (hi - lo) / 2;
        rec(v, tmp, lo, mid);
        rec(v, tmp, mid, hi);
        
        // Optimización: si el mayor de la izquierda es <= al menor de la derecha, ya está ordenado
        if (v[mid - 1] <= v[mid]) return; 
        
        merge(v, tmp, lo, mid, hi);
    }
}

void merge_sort(IntArray& v) {
    if (v.size() < 2) return;
    IntArray tmp(v.size()); // Buffer auxiliar reutilizable (O(N) espacio)
    rec(v, tmp, 0, v.size());
}