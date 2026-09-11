// INF-221 Tarea 1 2026-2 | Nombre: Tomás San Martín | Rol: 202473565-9
// Declaraciones para los algoritmos de multiplicación de matrices.
#ifndef MATRIX_ALGORITHMS_HPP
#define MATRIX_ALGORITHMS_HPP
#include <vector>

using Matrix = std::vector<std::vector<long long>>;

Matrix naive_multiply(const Matrix& A, const Matrix& B);
Matrix strassen_multiply(const Matrix& A, const Matrix& B);

#endif // MATRIX_ALGORITHMS_HPP
