// INF-221 Tarea 1 2026-2 | Nombre: Tomás San Martín | Rol: 202473565-9
// Programa principal: mide tiempo y memoria de los 4 algoritmos de ordenamiento.
// Lee casos de data/array_input y escribe:
//   - data/array_output/{n}_{t}_{d}_{m}_out.txt
//   - data/measurements/<algoritmo>.csv
// Referencias:
//  [1] https://en.cppreference.com/w/cpp/filesystem
//  [2] https://en.cppreference.com/w/cpp/regex

#include "algorithms/sorting.hpp"
#include "../common/bench.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <regex>
#include <string>
#include <vector>

namespace fs = std::filesystem;
using AlgoFn = void (*)(IntArray&);

struct Case { long long n; std::string t, d, m; fs::path file; };

static bool read_ints(const fs::path& p, std::vector<int>& out) {
    std::ifstream in(p);
    if (!in) return false;
    out.assign(std::istream_iterator<int>(in), {});
    return true;
}

int main(int argc, char** argv) {
    const fs::path input_dir = (argc > 1) ? argv[1] : fs::path("data/array_input");
    const fs::path out_dir   = (argc > 2) ? argv[2] : fs::path("data/array_output");
    const fs::path meas_dir  = (argc > 3) ? argv[3] : fs::path("data/measurements");
    fs::create_directories(out_dir);
    fs::create_directories(meas_dir);

    // Funciones con firma void(IntArray&)
    const std::vector<std::pair<std::string, AlgoFn>> algos_void = {
        {"mergesort", merge_sort},
        {"quicksort", quick_sort},
        {"patiencesort", patience_sort},
    };

    // Función SORT con firma diferente (devuelve vector)
    auto sortArray_wrapper = [](IntArray& v) { sortArray(v); };

    // Abrir todos los CSV
    std::map<std::string, std::ofstream> csvs;
    for (auto& [name, fn] : algos_void) {
        csvs[name].open(meas_dir / (name + ".csv"));
        csvs[name] << "algorithm,n,type,domain,sample,time_ms,mem_kb\n";
    }
    csvs["sort"].open(meas_dir / "sort.csv");
    csvs["sort"] << "algorithm,n,type,domain,sample,time_ms,mem_kb\n";

    const std::regex re(R"((\d+)_(ascendente|descendente|aleatorio)_(D\d+)_(a|b|c)\.txt)");
    std::vector<Case> cases;
    for (auto& e : fs::directory_iterator(input_dir)) {
        std::smatch sm;
        const std::string fn = e.path().filename().string();
        if (e.is_regular_file() && std::regex_match(fn, sm, re))
            cases.push_back({std::stoll(sm[1]), sm[2], sm[3], sm[4], e.path()});
    }
    std::sort(cases.begin(), cases.end(), [](const Case& a, const Case& b) {
        return std::tie(a.n, a.t, a.d, a.m) < std::tie(b.n, b.t, b.d, b.m);
    });

    for (const Case& c : cases) {
        std::vector<int> original;
        if (!read_ints(c.file, original)) {
            std::cerr << "No se pudo leer " << c.file << "\n";
            continue;
        }
        const int reps = (c.n > 1000000) ? 1 : 5;
        bool out_written = false;

        // Primer bucle: algoritmos void (merge, quick, patience)
        for (auto& [name, fn] : algos_void) {
            double t_sum = 0; long long mem = 0;
            IntArray work;
            for (int r = 0; r < reps; ++r) {
                work = original;                 // copia FUERA de la medicion
                auto res = benchmark([&] { fn(work); });
                t_sum += res.time_ms;
                mem = res.mem_extra_kb;
            }
            if (!std::is_sorted(work.begin(), work.end()))
                std::cerr << "ERROR: " << name << " no ordeno " << c.file << "\n";
            if (!out_written) {
                std::ofstream out(out_dir / (c.file.stem().string() + "_out.txt"));
                for (size_t i = 0; i < work.size(); ++i)
                    out << work[i] << " \n"[i == work.size() - 1];
                out_written = true;
            }
            csvs[name] << name << ',' << c.n << ',' << c.t << ',' << c.d << ',' << c.m
                       << ',' << t_sum / reps << ',' << mem << '\n';
        }

        // Segundo bucle: SORT (firma diferente)
        {
            double t_sum = 0; long long mem = 0;
            IntArray work;
            for (int r = 0; r < reps; ++r) {
                work = original;
                auto res = benchmark([&] { sortArray_wrapper(work); });
                t_sum += res.time_ms;
                mem = res.mem_extra_kb;
            }
            if (!std::is_sorted(work.begin(), work.end()))
                std::cerr << "ERROR: sort no ordeno " << c.file << "\n";
            csvs["sort"] << "sort" << ',' << c.n << ',' << c.t << ',' << c.d << ',' << c.m
                         << ',' << t_sum / reps << ',' << mem << '\n';
        }

        std::cerr << "OK " << c.file.filename() << "\n";
    }
    return 0;
}