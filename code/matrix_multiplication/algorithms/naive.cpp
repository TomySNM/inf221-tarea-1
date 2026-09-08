// INF-221 Tarea 1 2026-2 | Nombre: Tomás San Martín | Rol: 202473565-9
// Algoritmo: Multiplicación Naive (Triple loop ijk). Complejidad: O(n^3).
// Referencias:
//  [1] Cormen et al., Introduction to Algorithms, Sec. 4.2.
//  [2] https://en.wikipedia.org/wiki/Matrix_multiplication_algorithm
#include "matrix_algorithms.hpp"

Matrix naive_multiply(const Matrix& A, const Matrix& B) {
    const size_t n = A.size();
    Matrix C(n, std::vector<long long>(n, 0));
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < n; ++j) {
            long long s = 0;
            for (size_t k = 0; k < n; ++k) s += A[i][k] * B[k][j];
            C[i][j] = s;
        }
    return C;
}