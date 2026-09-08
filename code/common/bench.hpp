// INF-221 Tarea 1 2026-2 | Nombre: Tomás San Martín | Rol: 202473565-9
// Medición de tiempo (steady_clock) y de memoria: heap en uso (mallinfo2)
// muestreado cada 1 ms. En Windows se usa RSS del proceso (psapi) como fallback.
// Referencias:
//  [1] https://en.cppreference.com/w/cpp/chrono/steady_clock
//  [2] glibc: mallinfo2(3), https://man7.org/linux/man-pages/man3/mallinfo.3.html
//  [3] Windows: GetProcessMemoryInfo (psapi)
#ifndef BENCH_HPP
#define BENCH_HPP
#include <atomic>
#include <chrono>
#include <thread>
#ifdef _WIN32
  #include <windows.h>
  #include <psapi.h>
#else
  #include <malloc.h>
#endif

// Memoria de heap actualmente asignada por el proceso (KB).
inline long long current_mem_kb() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
        return static_cast<long long>(pmc.WorkingSetSize) / 1024;
    return 0;
#else
    struct mallinfo2 mi = mallinfo2();
    return static_cast<long long>(mi.uordblks) / 1024;
#endif
}

struct BenchResult { double time_ms; long long mem_extra_kb; };

template <class F>
BenchResult benchmark(F f) {
    const long long base = current_mem_kb();
    std::atomic<bool> stop{false};
    std::atomic<long long> peak{base};
    std::thread sampler([&] {
        while (!stop.load(std::memory_order_relaxed)) {
            long long r = current_mem_kb();
            long long p = peak.load(std::memory_order_relaxed);
            if (r > p) peak.store(r, std::memory_order_relaxed);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });
    auto t0 = std::chrono::steady_clock::now();
    f();
    auto t1 = std::chrono::steady_clock::now();
    stop.store(true);
    sampler.join();
    double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    long long extra = peak.load() - base;
    if (extra < 0) extra = 0;
    return {ms, extra};
}
#endif // BENCH_HPP