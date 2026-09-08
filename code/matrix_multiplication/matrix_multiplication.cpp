// INF-221 Tarea 1 2026-2 | Nombre: Tomás San Martín | Rol: 202473565-9
// Programa principal matrices: mide tiempo y memoria (heap) de naive y strassen.
// Lee de data/matrix_input, escribe en data/matrix_output y data/measurements.
#include "algorithms/matrix_algorithms.hpp"
#include "../common/bench.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <regex>
#include <algorithm>

namespace fs = std::filesystem;
using AlgoFn = Matrix (*)(const Matrix&, const Matrix&);

struct Case { long long n; std::string t, d, m, base; };

static bool read_matrix(const fs::path& p, size_t n, Matrix& M) {
    std::ifstream in(p);
    if (!in) return false;
    std::vector<long long> vals((std::istream_iterator<long long>(in)), {});
    if (vals.size() == n * n + 1 && vals[0] == (long long)n) vals.erase(vals.begin());
    if (vals.size() != n * n) return false;
    M.assign(n, std::vector<long long>(n));
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < n; ++j) M[i][j] = vals[i * n + j];
    return true;
}

int main(int argc, char** argv) {
    const fs::path input_dir = (argc > 1) ? argv[1] : fs::path("data/matrix_input");
    const fs::path out_dir   = (argc > 2) ? argv[2] : fs::path("data/matrix_output");
    const fs::path meas_dir  = (argc > 3) ? argv[3] : fs::path("data/measurements");
    fs::create_directories(out_dir);
    fs::create_directories(meas_dir);

    const std::vector<std::pair<std::string, AlgoFn>> algos = {
        {"naive", naive_multiply}, {"strassen", strassen_multiply}
    };
    std::map<std::string, std::ofstream> csvs;
    for (auto& [name, fn] : algos) {
        csvs[name].open(meas_dir / (name + ".csv"));
        csvs[name] << "algorithm,n,type,domain,sample,time_ms,mem_kb\n";
    }

    const std::regex re(R"((\d+)_(dispersa|diagonal|densa)_(D\d+)_(a|b|c)_1\.txt)");
    std::vector<Case> cases;
    for (auto& e : fs::directory_iterator(input_dir)) {
        std::smatch sm;
        const std::string fn = e.path().filename().string();
        if (e.is_regular_file() && std::regex_match(fn, sm, re))
            cases.push_back({std::stoll(sm[1]), sm[2], sm[3], sm[4], fn.substr(0, fn.size() - 6)});
    }
    std::sort(cases.begin(), cases.end(), [](const Case& a, const Case& b) {
        return std::tie(a.n, a.t, a.d, a.m) < std::tie(b.n, b.t, b.d, b.m);
    });

    for (const Case& c : cases) {
        Matrix A, B;
        const size_t n = (size_t)c.n;
        if (!read_matrix(input_dir / (c.base + "_1.txt"), n, A) ||
            !read_matrix(input_dir / (c.base + "_2.txt"), n, B)) {
            std::cerr << "No se pudo leer " << c.base << "\n"; continue;
        }
        const int reps = (n >= 512) ? 1 : 5;
        bool out_written = false;
        for (auto& [name, fn] : algos) {
            double t_sum = 0; long long mem = 0;
            Matrix C;
            for (int r = 0; r < reps; ++r) {
                auto res = benchmark([&] { C = fn(A, B); });
                t_sum += res.time_ms; mem = res.mem_extra_kb;
            }
            if (!out_written) {
                std::ofstream out(out_dir / (c.base + "_out.txt"));
                for (size_t i = 0; i < n; ++i)
                    for (size_t j = 0; j < n; ++j) out << C[i][j] << " \n"[j == n - 1];
                out_written = true;
            } else if (name == "strassen" && C != Matrix()) {
                // Verificación simple contra naive (ya calculado y guardado)
                Matrix ref; std::ifstream in(out_dir / (c.base + "_out.txt"));
                ref.assign(n, std::vector<long long>(n));
                for (size_t i = 0; i < n; ++i) for (size_t j = 0; j < n; ++j) in >> ref[i][j];
                if (ref != C) std::cerr << "ERROR: strassen != naive en " << c.base << "\n";
            }
            csvs[name] << name << ',' << c.n << ',' << c.t << ',' << c.d << ',' << c.m
                       << ',' << t_sum / reps << ',' << mem << '\n';
        }
        std::cerr << "OK " << c.base << "\n";
    }
    return 0;
}