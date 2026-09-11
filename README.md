# Tarea 1 INF-221 2026-2 — Algoritmos y Complejidad
**Tomás San Martín | Rol 202473565-9**

Análisis experimental de cuatro algoritmos de ordenamiento (merge sort, quick sort,
patience sort y `std::sort`) y dos de multiplicación de matrices cuadradas (naive y
Strassen): medición de tiempo y memoria sobre 72 casos por problema, y contraste de lo
observado con las cotas teóricas O(n log n), O(n³) y O(n^log₂ 7), incluyendo una
verificación con el teorema maestro.

El informe final (figuras, tablas y análisis) está en [`report/report.pdf`](report/report.pdf).
Este repositorio es el respaldo del código; la entrega oficial es el `.zip` subido a aula.usm.cl.

## Estructura del repositorio

```
├── assignment_statement/       Enunciado de la tarea (PDF y fuentes LaTeX).
├── code/
│   ├── common/                 bench.hpp: medición de tiempo y memoria (heap).
│   ├── sorting/                Los 4 ordenamientos + main, generador y graficador.
│   └── matrix_multiplication/  naive y Strassen + main, generador y graficador.
├── report/                     Informe LaTeX y su PDF compilado.
└── README.md
```

## Cómo correr todo desde cero

Los datos crudos de entrada/salida no están en el repo (~2 GB); se regeneran con los
scripts incluidos en cada carpeta:

```bash
cd code/sorting            # o code/matrix_multiplication
make                       # compila
python3 scripts/array_generator.py     # genera los casos (en matrices: matrix_generator.py)
make run                   # mide tiempo y memoria -> data/measurements/*.csv
python3 scripts/plot_generator.py      # grafica los CSV -> data/plots/*.png
```

Requisitos: g++ con C++17; Python 3 con numpy, pandas y matplotlib.

## Detalles que vale la pena conocer

- **Patience sort:** la fusión de pilas fue reescrita con un min-heap (la base de la fase
  de pilas viene de GeeksforGeeks/Wikipedia, citadas al inicio del `.cpp`).
- **Quicksort:** mediana de tres + cutoff al estilo Sedgewick (citado en el código).
- **Strassen:** umbral de corte que cambia a naive en submatrices pequeñas.
- Todas las mediciones usan las mismas flags (`-O2 -std=c++17`): las diferencias entre
  curvas las marcan los algoritmos, no el compilador.

## Referencias y declaración

Las referencias de cada implementación están al inicio de cada `.cpp` (CLRS, Sedgewick,
Strassen, cppreference, Wikipedia, GeeksforGeeks). Se usó asistencia de IA (Qwen) para 
la implementación de los algoritmos.