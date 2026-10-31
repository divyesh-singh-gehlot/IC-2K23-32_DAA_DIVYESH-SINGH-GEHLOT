// Job Sequencing with Deadlines (Greedy)
// Each job takes 1 unit of time and gives profit only if it finishes
// before its deadline. Sort jobs by profit and put each job in the
// latest free slot that is on or before its deadline.

#include <iostream>
#include <algorithm>
using namespace std;

struct Job {
    char id;
    int deadline;
    int profit;
};

bool compare(Job a, Job b) {
    return a.profit > b.profit;
}

int main() {
    int n = 5;
    Job jobs[5] = {
        {'a', 2, 100},
        {'b', 1, 19},
        {'c', 2, 27},
        {'d', 1, 25},
        {'e', 3, 15}
    };

    sort(jobs, jobs + n, compare);

    // find the largest deadline to know how many slots we need
    int maxDeadline = 0;
    for (int i = 0; i < n; i++) {
        if (jobs[i].deadline > maxDeadline) {
            maxDeadline = jobs[i].deadline;
        }
    }

    char slot[10];
    bool filled[10];
    for (int i = 0; i < maxDeadline; i++) {
        filled[i] = false;
    }

    int totalProfit = 0;
    int jobsDone = 0;

    for (int i = 0; i < n; i++) {
        // try the latest slot first, then go backwards
        for (int j = jobs[i].deadline - 1; j >= 0; j--) {
            if (filled[j] == false) {
                filled[j] = true;
                slot[j] = jobs[i].id;
                totalProfit = totalProfit + jobs[i].profit;
                jobsDone++;
                break;
            }
        }
    }

    cout << "Jobs sorted by profit: ";
    for (int i = 0; i < n; i++) {
        cout << jobs[i].id << "(" << jobs[i].profit << ") ";
    }
    cout << endl << endl;

    cout << "Job sequence: ";
    for (int i = 0; i < maxDeadline; i++) {
        if (filled[i]) {
            cout << slot[i] << " ";
        }
    }
    cout << endl;
    cout << "Jobs completed: " << jobsDone << endl;
    cout << "Total profit  : " << totalProfit << endl;
    return 0;
}