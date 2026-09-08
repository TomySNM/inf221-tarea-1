#ifndef SORTING_HPP
#define SORTING_HPP

#include <vector>

// Definimos un alias para no escribir "std::vector<int>" en todos lados.
using IntArray = std::vector<int>;

// Declaramos las funciones.
// implementadas en los otros archivos .cpp (mergesort.cpp, quicksort.cpp, etc.).
void merge_sort(IntArray& v);
void quick_sort(IntArray& v);
void patience_sort(IntArray& v);
void std_sort(IntArray& v);
std::vector<int> sortArray(std::vector<int>& arr);

#endif