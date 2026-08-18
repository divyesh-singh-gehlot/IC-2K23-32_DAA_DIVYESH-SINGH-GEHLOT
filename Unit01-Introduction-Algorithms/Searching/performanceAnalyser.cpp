// ===========================================================
// Generic Algorithm Performance Analyzer (C++ template)
//
// Reusable harness: works for ANY algorithm, not just binary
// search. You only edit the sections marked EDIT HERE.
//
// How it works:
//   - benchmark<T>() takes any callable + a "setup" function
//     that regenerates input for each size n.
//   - Add one entry per algorithm you want to compare in
//     run_experiment().
//   - CSV + gnuplot output stay generic; columns are named
//     automatically from the labels you give each algorithm.
// ===========================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include <numeric>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <functional>
#include <memory>

// -----------------------------------------------------------
// volatile sink: stops the optimizer from proving a benchmarked
// call's result is unused and deleting the call entirely.
// Every algorithm you benchmark must funnel its result through
// this (see examples below). Without it, -O2 can silently
// optimize your whole timing loop into a no-op.
// -----------------------------------------------------------
volatile long long sink = 0;

// -----------------------------------------------------------
// Generic timing helper.
//   fn       -> the code to time, wrapped as a lambda with no args.
//               Must write its result into `sink` (or a captured
//               reference) so it can't be optimized away.
//   trials   -> number of repetitions to average over.
// Returns average seconds per call.
// -----------------------------------------------------------
double measure_time(const std::function<void()>& fn, int trials = 200) {
    fn();  // warm-up (page faults, cache, branch predictor)

    // -------------------------------------------------------
    // Auto-calibrate a batch size: a single call to a fast
    // algorithm (e.g. binary search on a small array) can
    // finish faster than the clock's resolution (~100ns-1us
    // on Windows), which makes every sample read as 0.
    // Fix: run the call `batch` times back-to-back inside one
    // timed block, growing `batch` until that block clears a
    // minimum duration, then report time-per-call = block/batch.
    // -------------------------------------------------------
    constexpr double MIN_BLOCK_SECONDS = 0.001;  // 1 ms floor
    long long batch = 1;

    while (true) {
        auto start = std::chrono::steady_clock::now();
        for (long long i = 0; i < batch; ++i) fn();
        auto end = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double>(end - start).count();

        if (elapsed >= MIN_BLOCK_SECONDS || batch > (1LL << 30)) break;
        batch *= 2;
    }

    // Now take `trials` timed blocks of size `batch` and average
    // the per-call time across them.
    std::vector<double> times;
    times.reserve(trials);

    for (int t = 0; t < trials; ++t) {
        auto start = std::chrono::steady_clock::now();
        for (long long i = 0; i < batch; ++i) fn();
        auto end = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double>(end - start).count();
        times.push_back(elapsed / static_cast<double>(batch));
    }

    return std::accumulate(times.begin(), times.end(), 0.0) / times.size();
}

// -----------------------------------------------------------
// Describes one algorithm entry in the comparison.
//   name       -> shows up in console output, CSV header, and plot legend
//   build_call -> given input size n, returns a zero-arg lambda that
//                 runs the algorithm once on freshly-generated input
//                 for that n. Regenerating input per size (not per
//                 trial) keeps timing focused on the algorithm, not
//                 on data generation.
// -----------------------------------------------------------
struct AlgoEntry {
    std::string name;
    std::function<std::function<void()>(long long n)> build_call;
};

// ===========================================================
// EDIT HERE (1/3): your input sizes
// ===========================================================
std::vector<long long> input_sizes = {
    100, 500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000
};

// ===========================================================
// EDIT HERE (2/3): your algorithms
// Add/remove entries here. Everything below is generic.
// ===========================================================

// ---- pasted verbatim from iterativeBinarySearch.cpp ----
// (only the function — main()/cin left behind in the original file)
int iterativeBinarySearch(int arr[], int n, int key) {
    int low = 0;
    int high = n - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (arr[mid] == key)
            return mid;
        if (arr[mid] < key)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}

int recursiveBinarySearch(int arr[], int low, int high, int key) {
    if (low > high)
        return -1;

    int mid = (high + low) / 2;

    if (arr[mid] == key)
        return mid;

    if (arr[mid] < key)
        return recursiveBinarySearch(arr, mid + 1, high, key);
    else
        return recursiveBinarySearch(arr, low, mid - 1, key);
}

int linearSearch(int arr[], int n, int key) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == key) {
            return i;
        }
    }

    return -1;
}

std::vector<AlgoEntry> get_algorithms() {
    return {
        // ---- iterativeBinarySearch: doesn't mutate the array ->
        //      build sorted data ONCE, outside the returned lambda,
        //      and reuse it every trial ----
        {
            "iterativeBinarySearch",
            [](long long n) -> std::function<void()> {
                auto arr = std::make_shared<std::vector<int>>(n);
                std::iota(arr->begin(), arr->end(), 0);          // sorted 0..n-1, required by binary search
                std::mt19937 rng(std::random_device{}());
                int key = (*arr)[std::uniform_int_distribution<long long>(0, n - 1)(rng)];

                return [arr, key]() {
                    sink = iterativeBinarySearch(arr->data(), static_cast<int>(arr->size()), key);
                };
            }
        },
        // ---- recursiveBinarySearch: same sorted-array setup as the
        //      iterative version, just a different call signature
        //      (takes low/high bounds instead of a count) ----
        {
            "recursiveBinarySearch",
            [](long long n) -> std::function<void()> {
                auto arr = std::make_shared<std::vector<int>>(n);
                std::iota(arr->begin(), arr->end(), 0);
                std::mt19937 rng(std::random_device{}());
                int key = (*arr)[std::uniform_int_distribution<long long>(0, n - 1)(rng)];

                return [arr, key]() {
                    sink = recursiveBinarySearch(arr->data(), 0, static_cast<int>(arr->size()) - 1, key);
                };
            }
        },
        // ---- linearSearch: doesn't require sorted data and doesn't
        //      mutate the array -> build ONCE and reuse every trial.
        //      Worst-case key (not present) shown here so the search
        //      always scans the full array; swap to a present key if
        //      you want average-case timing instead. ----
        {
            "linearSearch",
            [](long long n) -> std::function<void()> {
                auto arr = std::make_shared<std::vector<int>>(n);
                std::mt19937 rng(std::random_device{}());
                std::uniform_int_distribution<int> dist(0, static_cast<int>(n) * 2);
                for (auto& x : *arr) x = dist(rng);
                int key = -1;  // guaranteed not present -> worst case scan

                return [arr, key]() {
                    sink = linearSearch(arr->data(), static_cast<int>(arr->size()), key);
                };
            }
        },
    };
}

// ===========================================================
// EDIT HERE (3/3): trial count per size (optional tuning)
// ===========================================================
constexpr int TRIALS_PER_SIZE = 200;

// -----------------------------------------------------------
// Everything below is generic — no need to touch it.
// -----------------------------------------------------------

struct Result {
    long long n;
    std::vector<double> times;  // one per algorithm, same order as get_algorithms()
};

std::vector<Result> run_experiment(const std::vector<AlgoEntry>& algos) {
    std::vector<Result> results;

    for (long long n : input_sizes) {
        Result row{n, {}};
        std::cout << "n = " << n;

        for (const auto& algo : algos) {
            auto call = algo.build_call(n);
            double avg_time = measure_time([&call]() { call(); }, TRIALS_PER_SIZE);
            row.times.push_back(avg_time);
            std::cout << "  |  " << algo.name << " = " << (avg_time * 1e6) << " us";
        }
        std::cout << "\n";
        results.push_back(std::move(row));
    }
    return results;
}

void save_results_csv(const std::vector<Result>& results, const std::vector<AlgoEntry>& algos,
                       const std::string& filename = "results.csv") {
    std::ofstream f(filename);
    f << "input_size";
    for (const auto& algo : algos) f << "," << algo.name << "_time_sec";
    f << "\n";

    for (const auto& r : results) {
        f << r.n;
        for (double t : r.times) f << "," << t;
        f << "\n";
    }
    std::cout << "\nResults saved to " << filename << "\n";
}

void plot_results(const std::vector<Result>& results, const std::vector<AlgoEntry>& algos,
                   const std::string& png_filename = "performance_graph.png") {
    const std::string data_filename = "plot_data.dat";
    const std::string script_filename = "plot_script.gp";

    std::ofstream data(data_filename);
    for (const auto& r : results) {
        data << r.n;
        for (double t : r.times) data << " " << (t * 1e6);
        data << "\n";
    }
    data.close();

    std::ofstream script(script_filename);
    script << "set terminal pngcairo size 900,600 enhanced font 'Arial,10'\n";
    script << "set output '" << png_filename << "'\n";
    script << "set title 'Algorithm Performance Comparison'\n";
    script << "set xlabel 'Input Size (n) [log scale]'\n";
    script << "set ylabel 'Average Execution Time (microseconds)'\n";
    script << "set logscale x\n";
    script << "set logscale y\n";
    script << "set grid\n";
    script << "set key top left\n";
    script << "plot ";
    for (size_t i = 0; i < algos.size(); ++i) {
        script << "'" << data_filename << "' using 1:" << (i + 2)
               << " with linespoints title '" << algos[i].name << "' pt " << (7 + i);
        if (i + 1 < algos.size()) script << ", \\\n     ";
    }
    script << "\n";
    script.close();

    std::string cmd = "gnuplot " + script_filename;
    int ret = std::system(cmd.c_str());
    if (ret == 0) {
        std::cout << "Graph saved to " << png_filename << "\n";
    } else {
        std::cerr << "gnuplot failed (exit code " << ret << "). "
                     "Is gnuplot installed? Data file '" << data_filename
                  << "' is still available for plotting elsewhere.\n";
    }
}

int main() {
    auto algos = get_algorithms();
    auto results = run_experiment(algos);
    save_results_csv(results, algos);
    plot_results(results, algos);
    return 0;
}