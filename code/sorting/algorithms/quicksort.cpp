// INF-221 Tarea 1 2026-2 | Nombre: Tomás San Martín | Rol: 202473565-9
// Algoritmo: Quick Sort con Mediana de Tres
// Referencias:
//  [1] CLRS, Cap. 7 (Quicksort).
//  [2] Sedgewick, R. "Implementing Quicksort Programs" (Comms. of the ACM, 1978).
// Implementación basada y adaptada con asistencia de IA (Qwen)

#include "sorting.hpp"
#include <algorithm>
#include <utility>

namespace {
    const size_t CUTOFF = 16; // Umbral para cambiar a Insertion Sort

    void insertion_sort(IntArray& v, size_t lo, size_t hi) {
        for (size_t i = lo + 1; i <= hi; ++i) {
            int key = v[i];
            size_t j = i;
            while (j > lo && v[j - 1] > key) {
                v[j] = v[j - 1];
                --j;
            }
            v[j] = key;
        }
    }

    void quick_sort_rec(IntArray& v, size_t lo, size_t hi) {
        if (hi - lo < CUTOFF) return; // Lo ordena insertion_sort al final

        // Mediana de Tres: Ordena lo, mid, hi para elegir un buen pivote
        size_t mid = lo + (hi - lo) / 2;
        if (v[mid] < v[lo]) std::swap(v[lo], v[mid]);
        if (v[hi] < v[lo])  std::swap(v[lo], v[hi]);
        if (v[hi] < v[mid]) std::swap(v[mid], v[hi]);

        // El pivote ahora está en 'mid'. Lo escondemos en hi-1
        std::swap(v[mid], v[hi - 1]);
        int pivot = v[hi - 1];

        // Partición de Hoare/Sedgewick
        size_t i = lo;
        size_t j = hi - 1;

        while (true) {
            while (v[++i] < pivot);
            while (v[--j] > pivot);
            if (i >= j) break;
            std::swap(v[i], v[j]);
        }
        // Restaurar pivote en su posición final
        std::swap(v[i], v[hi - 1]);

        quick_sort_rec(v, lo, i - 1);
        quick_sort_rec(v, i + 1, hi);
    }
}

void quick_sort(IntArray& v) {
    if (v.size() < 2) return;
    quick_sort_rec(v, 0, v.size() - 1);
    insertion_sort(v, 0, v.size() - 1); // Limpia los subarreglos pequeños
}