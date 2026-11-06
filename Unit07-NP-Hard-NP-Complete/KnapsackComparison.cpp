// Comparison of 0/1 Knapsack: Dynamic Programming vs Branch and Bound
// Both methods are run on the same inputs. We compare the answer (it must be
// the same), the time taken, and the work done:
//   Dynamic Programming : fills a table of n x capacity cells
//   Branch and Bound    : number of search-tree nodes expanded

#include <iostream>
#include <iomanip>
#include <queue>
#include <vector>
#include <algorithm>
#include <chrono>
using namespace std;

// ---------------- Dynamic Programming ----------------
int knapsackDP(vector<int> weight, vector<int> value, int capacity) {
    int n = weight.size();
    vector< vector<int> > dp(n + 1, vector<int>(capacity + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            dp[i][w] = dp[i - 1][w];
            if (weight[i - 1] <= w) {
                int take = value[i - 1] + dp[i - 1][w - weight[i - 1]];
                if (take > dp[i][w]) {
                    dp[i][w] = take;
                }
            }
        }
    }
    return dp[n][capacity];
}

// ---------------- Branch and Bound ----------------
struct Item {
    int weight;
    int value;
};

struct Node {
    int level;
    int profit;
    int weight;
    double bound;
};

struct CompareNode {
    bool operator()(const Node& a, const Node& b) {
        return a.bound < b.bound;
    }
};

bool compareRatio(Item a, Item b) {
    return (double)a.value / a.weight > (double)b.value / b.weight;
}

double calcBound(Node u, int n, int capacity, vector<Item>& items) {
    if (u.weight > capacity) {
        return 0;
    }
    double bound = u.profit;
    int j = u.level + 1;
    int totalWeight = u.weight;
    while (j < n && totalWeight + items[j].weight <= capacity) {
        totalWeight = totalWeight + items[j].weight;
        bound = bound + items[j].value;
        j++;
    }
    if (j < n) {
        bound = bound + (capacity - totalWeight) * ((double)items[j].value / items[j].weight);
    }
    return bound;
}

int knapsackBB(vector<int> weight, vector<int> value, int capacity, long long& nodesExpanded) {
    int n = weight.size();
    vector<Item> items(n);
    for (int i = 0; i < n; i++) {
        items[i].weight = weight[i];
        items[i].value = value[i];
    }
    sort(items.begin(), items.end(), compareRatio);

    priority_queue<Node, vector<Node>, CompareNode> pq;
    Node root;
    root.level = -1;
    root.profit = 0;
    root.weight = 0;
    root.bound = calcBound(root, n, capacity, items);
    pq.push(root);

    int maxProfit = 0;
    nodesExpanded = 0;

    while (!pq.empty()) {
        Node u = pq.top();
        pq.pop();

        if ((int)u.bound <= maxProfit) {
            continue;
        }
        nodesExpanded++;

        int next = u.level + 1;
        if (next >= n) {
            continue;
        }

        Node withItem = u;
        withItem.level = next;
        withItem.weight = u.weight + items[next].weight;
        withItem.profit = u.profit + items[next].value;
        if (withItem.weight <= capacity && withItem.profit > maxProfit) {
            maxProfit = withItem.profit;
        }
        withItem.bound = calcBound(withItem, n, capacity, items);
        if (withItem.weight <= capacity && (int)withItem.bound > maxProfit) {
            pq.push(withItem);
        }

        Node withoutItem = u;
        withoutItem.level = next;
        withoutItem.bound = calcBound(withoutItem, n, capacity, items);
        if ((int)withoutItem.bound > maxProfit) {
            pq.push(withoutItem);
        }
    }
    return maxProfit;
}

// ---------------- test data and timing ----------------
unsigned long long seed = 12345;

int randomNumber(int low, int high) {
    seed = (seed * 1103515245ULL + 12345ULL) % 2147483648ULL;
    return low + (int)(seed % (high - low + 1));
}

void compare(int n, vector<int> weight, vector<int> value, int capacity) {
    int repeat = 200;       // run many times and take the average time

    auto t1 = chrono::high_resolution_clock::now();
    int dpAnswer = 0;
    for (int r = 0; r < repeat; r++) {
        dpAnswer = knapsackDP(weight, value, capacity);
    }
    auto t2 = chrono::high_resolution_clock::now();

    long long nodes = 0;
    int bbAnswer = 0;
    for (int r = 0; r < repeat; r++) {
        bbAnswer = knapsackBB(weight, value, capacity, nodes);
    }
    auto t3 = chrono::high_resolution_clock::now();

    double dpTime = chrono::duration<double, micro>(t2 - t1).count() / repeat;
    double bbTime = chrono::duration<double, micro>(t3 - t2).count() / repeat;

    cout << setw(4) << n << setw(10) << capacity
         << setw(10) << dpAnswer << setw(12) << fixed << setprecision(2) << dpTime
         << setw(10) << bbAnswer << setw(12) << bbTime
         << setw(10) << nodes << setw(10) << ((dpAnswer == bbAnswer) ? "Yes" : "NO!") << endl;
}

int main() {
    cout << "   n  capacity   DP value   DP time(us)  B&B value  B&B time(us)  B&B nodes   Same?" << endl;

    // the small example from the Branch and Bound program
    compare(4, {4, 7, 5, 3}, {40, 42, 25, 12}, 10);

    // random examples of growing size
    int sizes[5] = {10, 15, 20, 25, 30};
    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        vector<int> weight, value;
        int totalWeight = 0;
        for (int i = 0; i < n; i++) {
            weight.push_back(randomNumber(5, 40));
            value.push_back(randomNumber(10, 100));
            totalWeight = totalWeight + weight[i];
        }
        compare(n, weight, value, totalWeight / 2);
    }
    return 0;
}
