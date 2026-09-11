// INF-221 Tarea 1 2026-2 | Nombre: Tomás San Martín | Rol: 202473565-9
// Algoritmo: Strassen recursivo con cutoff a naive. Complejidad: O(n^2.807).
// Referencias:
//  [1] Strassen, V. (1969). "Gaussian elimination is not optimal". Numer. Math. 13.
//  [2] https://en.wikipedia.org/wiki/Strassen_algorithm
#include "matrix_algorithms.hpp"

namespace {
constexpr size_t CUTOFF = 128;

Matrix add(const Matrix& A, const Matrix& B) {
    const size_t n = A.size();
    Matrix C(n, std::vector<long long>(n));
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < n; ++j) C[i][j] = A[i][j] + B[i][j];
    return C;
}
Matrix sub(const Matrix& A, const Matrix& B) {
    const size_t n = A.size();
    Matrix C(n, std::vector<long long>(n));
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < n; ++j) C[i][j] = A[i][j] - B[i][j];
    return C;
}
Matrix block(const Matrix& M, size_t r, size_t c, size_t h) {
    Matrix B(h, std::vector<long long>(h));
    for (size_t i = 0; i < h; ++i)
        for (size_t j = 0; j < h; ++j) B[i][j] = M[r + i][c + j];
    return B;
}
void set_block(Matrix& M, const Matrix& B, size_t r, size_t c) {
    const size_t h = B.size();
    for (size_t i = 0; i < h; ++i)
        for (size_t j = 0; j < h; ++j) M[r + i][c + j] = B[i][j];
}
Matrix strassen_rec(const Matrix& A, const Matrix& B) {
    const size_t n = A.size();
    if (n <= CUTOFF) return naive_multiply(A, B);
    const size_t h = n / 2;
    Matrix A11 = block(A, 0, 0, h), A12 = block(A, 0, h, h),
           A21 = block(A, h, 0, h), A22 = block(A, h, h, h),
           B11 = block(B, 0, 0, h), B12 = block(B, 0, h, h),
           B21 = block(B, h, 0, h), B22 = block(B, h, h, h);
    Matrix M1 = strassen_rec(add(A11, A22), add(B11, B22));
    Matrix M2 = strassen_rec(add(A21, A22), B11);
    Matrix M3 = strassen_rec(A11, sub(B12, B22));
    Matrix M4 = strassen_rec(A22, sub(B21, B11));
    Matrix M5 = strassen_rec(add(A11, A12), B22);
    Matrix M6 = strassen_rec(sub(A21, A11), add(B11, B12));
    Matrix M7 = strassen_rec(sub(A12, A22), add(B21, B22));
    Matrix C(n, std::vector<long long>(n));
    set_block(C, add(add(M1, M4), sub(M7, M5)), 0, 0);
    set_block(C, add(M3, M5), 0, h);
    set_block(C, add(M2, M4), h, 0);
    set_block(C, add(add(M1, M3), sub(M6, M2)), h, h);
    return C;
}
}

Matrix strassen_multiply(const Matrix& A, const Matrix& B) {
    const size_t n = A.size();
    size_t p = 1; while (p < n) p <<= 1;
    if (p == n) return strassen_rec(A, B);
    Matrix PA(p, std::vector<long long>(p, 0)), PB(p, std::vector<long long>(p, 0));
    set_block(PA, A, 0, 0); set_block(PB, B, 0, 0);
    Matrix C = strassen_rec(PA, PB);
    Matrix R(n, std::vector<long long>(n));
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < n; ++j) R[i][j] = C[i][j];
    return R;
}