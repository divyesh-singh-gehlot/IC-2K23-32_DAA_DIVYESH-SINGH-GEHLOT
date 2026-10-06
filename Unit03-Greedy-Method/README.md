# Unit 03 — Greedy Method

This unit covers greedy optimization techniques, job scheduling, and minimum spanning tree algorithms.

**Progressive version:** 1 — 1 program documented.

## Programs

### 1. Fractional Knapsack (`FractionalKnapsack.cpp`)

**Approach:** Computes each item's value-to-weight ratio, sorts items by descending ratio, and takes a whole item or the fraction that fits until the capacity is full.

- **Time complexity:** O(n log n) due to sorting.
- **Space complexity:** O(n) for item storage and sorting support.
- **Program behavior:** Uses predefined items and capacity; prints ratios, fractions taken, and maximum total value.

**Output:**

![Fractional Knapsack output](Outputs/FractionalKnapsack.png)

*Output screenshot: `Outputs/FractionalKnapsack.png`*



## Compile and run

Compile an individual source file using a C++17 compiler:

```bash
g++ -std=c++17 ProgramName.cpp -o program
./program
```

On Windows PowerShell:

```powershell
g++ -std=c++17 ProgramName.cpp -o program.exe
.\program.exe
```

Replace `ProgramName.cpp` with the source file you want to run. Programs that request input will prompt in the terminal; others use sample data defined in the source.

## Notes

- Each C++ source file is a standalone program and should be compiled separately.
- Output images are stored in the adjacent `Outputs/` folder and are embedded using relative Markdown paths.
- Keep the `Outputs/` folder alongside `README.md` so images render correctly on GitHub.
