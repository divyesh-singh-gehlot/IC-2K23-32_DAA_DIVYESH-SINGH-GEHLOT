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
//
// NOTE ON SORTING ALGORITHMS SPECIFICALLY:
//   Unlike binary/linear search, sorting algorithms MUTATE
//   their input. If we built the array once and reused it
//   across trials, the 2nd..Nth trial would run on an
//   ALREADY-SORTED array, which is not a fair/representative
//   timing (and for some algorithms is a best case, not the
//   average/worst case). So for every algorithm below, the
//   returned lambda makes a FRESH COPY of the unsorted data
//   on every single call, and only the sort itself is timed
//   as part of that call. This costs a copy per call, but the
//   copy is O(n) and negligible next to O(n^2) sort costs, and
//   it keeps every trial comparable.
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
#include <utility>

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
    // algorithm can finish faster than the clock's resolution
    // (~100ns-1us on Windows), which makes every sample read
    // as 0. Fix: run the call `batch` times back-to-back inside
    // one timed block, growing `batch` until that block clears
    // a minimum duration, then report time-per-call = block/batch.
    //
    // IMPORTANT for sorting algorithms: because each call
    // rebuilds+resorts its own fresh copy of the data (see the
    // note above the AlgoEntry section), batching many calls
    // together is still safe/fair -- every call in the batch
    // still sorts freshly-unsorted data, it's just that the
    // copy+sort cost is what's being amortized across the batch.
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
//                 for that n.
// -----------------------------------------------------------
struct AlgoEntry {
    std::string name;
    std::function<std::function<void()>(long long n)> build_call;
};

// ===========================================================
// EDIT HERE (1/3): your input sizes
//
// These are O(n^2) sorting algorithms, so sizes are kept much
// smaller than a search benchmark (1,000,000 would take far
// too long -- n^2 for a million elements is 10^12 comparisons
// per single call). Adjust freely, but scale up carefully.
// ===========================================================
std::vector<long long> input_sizes = {
    100, 250, 500, 1000, 2000, 4000, 8000, 12000, 16000, 20000
};

// ===========================================================
// EDIT HERE (2/3): your algorithms
// Add/remove entries here. Everything below is generic.
// ===========================================================

// ---- pasted from user code, with fixes noted below ----

void iterativeBubbleSort(int arr[], int n) {
    for (int i = 0; i < (n - 1); i++) {
        for (int j = 0; j < (n - i - 1); j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void iterativeInsertionSort(std::vector<int>& arr) {
    for (int i = 0; i < (int)arr.size(); i++) {
        int j = i;
        while (j > 0 && arr[j - 1] > arr[j]) {
            std::swap(arr[j - 1], arr[j]);
            j--;
        }
    }
}

// FIX: original called `BubbleSort(arr, n - 1)`, which doesn't exist --
// it should recurse into itself.
void recursiveBubbleSort(int arr[], int n) {
    if (n == 1) return;

    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            std::swap(arr[i], arr[i + 1]);
        }
    }

    recursiveBubbleSort(arr, n - 1);
}

// FIX: original called `InsertionSort(arr, n, i + 1)`, which doesn't
// exist -- it should recurse into itself.
void recursiveInsertionSort(std::vector<int>& arr, int n, int i) {
    int j = i;
    if (j > n - 1) return;
    while (j > 0 && arr[j - 1] > arr[j]) {
        std::swap(arr[j], arr[j - 1]);
        j--;
    }
    recursiveInsertionSort(arr, n, i + 1);
}

void SelectionSort(int arr[], int n) {
    for (int i = 0; i < (n - 1); i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min]) {
                min = j;
            }
        }
        std::swap(arr[i], arr[min]);
    }
}

// Shared base data generator: a fixed random permutation for a given n,
// so every algorithm is timed against the SAME unsorted input at that
// size (fair comparison). Each algorithm then copies this base and
// sorts its own copy on every call, since sorting is destructive.
static std::shared_ptr<std::vector<int>> make_base_data(long long n) {
    auto base = std::make_shared<std::vector<int>>(n);
    std::mt19937 rng(12345);  // fixed seed -> same unsorted input across algorithms
    std::uniform_int_distribution<int> dist(0, static_cast<int>(n) * 2);
    for (auto& x : *base) x = dist(rng);
    return base;
}

std::vector<AlgoEntry> get_algorithms() {
    return {
        {
            "iterativeBubbleSort",
            [](long long n) -> std::function<void()> {
                auto base = make_base_data(n);
                return [base]() {
                    std::vector<int> arr(*base);  // fresh unsorted copy every call
                    iterativeBubbleSort(arr.data(), static_cast<int>(arr.size()));
                    sink = arr.empty() ? 0 : arr[0];
                };
            }
        },
        {
            "iterativeInsertionSort",
            [](long long n) -> std::function<void()> {
                auto base = make_base_data(n);
                return [base]() {
                    std::vector<int> arr(*base);
                    iterativeInsertionSort(arr);
                    sink = arr.empty() ? 0 : arr[0];
                };
            }
        },
        {
            "recursiveBubbleSort",
            [](long long n) -> std::function<void()> {
                auto base = make_base_data(n);
                return [base]() {
                    std::vector<int> arr(*base);
                    if (!arr.empty())
                        recursiveBubbleSort(arr.data(), static_cast<int>(arr.size()));
                    sink = arr.empty() ? 0 : arr[0];
                };
            }
        },
        {
            "recursiveInsertionSort",
            [](long long n) -> std::function<void()> {
                auto base = make_base_data(n);
                return [base]() {
                    std::vector<int> arr(*base);
                    recursiveInsertionSort(arr, static_cast<int>(arr.size()), 0);
                    sink = arr.empty() ? 0 : arr[0];
                };
            }
        },
        {
            "SelectionSort",
            [](long long n) -> std::function<void()> {
                auto base = make_base_data(n);
                return [base]() {
                    std::vector<int> arr(*base);
                    SelectionSort(arr.data(), static_cast<int>(arr.size()));
                    sink = arr.empty() ? 0 : arr[0];
                };
            }
        },
    };
}

// ===========================================================
// EDIT HERE (3/3): trial count per size (optional tuning)
//
// Lowered vs. the search-benchmark default: with O(n^2) work
// and a per-call array copy+sort, large n at 200 trials can get
// slow. Drop this further (e.g. 20-50) if the biggest sizes take
// too long on your machine.
// ===========================================================
constexpr int TRIALS_PER_SIZE = 50;

// -----------------------------------------------------------
// Everything below is generic -- no need to touch it.
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
    script << "set title 'Sorting Algorithm Performance Comparison'\n";
    script << "set xlabel 'Input Size (n)'\n";
    script << "set ylabel 'Average Execution Time (microseconds) [log scale]'\n";
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