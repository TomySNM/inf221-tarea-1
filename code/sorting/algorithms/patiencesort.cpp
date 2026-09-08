// INF-221 Tarea 1 2026-2 | Nombre: Tomás San Martín | Rol: 202473565-9
// Algoritmo: Patience Sort
// Referencias:
//  [1] Aldous & Diaconis, "Longest strictly increasing subsequences and Plancherel measure".
//  [2] https://en.wikipedia.org/wiki/Patience_sorting
// Implementación basada y adaptada con asistencia de IA (Qwen)

#include "sorting.hpp"
#include <vector>
#include <queue>
#include <algorithm>

void patience_sort(IntArray& arr) {
    if (arr.empty()) return;

    // FASE 1: Creación de pilas (O(N log K))
    std::vector<std::vector<int>> piles;
    std::vector<int> pile_tops; 

    for (int x : arr) {
        // lower_bound encuentra la primera pila cuyo tope sea >= x
        auto it = std::lower_bound(pile_tops.begin(), pile_tops.end(), x);
        
        if (it == pile_tops.end()) {
            piles.push_back({x});
            pile_tops.push_back(x);
        } else {
            size_t idx = std::distance(pile_tops.begin(), it);
            piles[idx].push_back(x);
            pile_tops[idx] = x; 
        }
    }

    // FASE 2: Fusión usando Min-Heap (O(N log K))
    using Node = std::pair<int, size_t>; // {valor, indice_pila}
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> min_heap;

    for (size_t i = 0; i < piles.size(); ++i) {
        min_heap.push({piles[i].back(), i});
    }

    size_t idx = 0;
    while (!min_heap.empty()) {
        auto [val, pile_idx] = min_heap.top();
        min_heap.pop();

        arr[idx++] = val; 
        piles[pile_idx].pop_back(); 

        if (!piles[pile_idx].empty()) {
            min_heap.push({piles[pile_idx].back(), pile_idx});
        }
    }
}
