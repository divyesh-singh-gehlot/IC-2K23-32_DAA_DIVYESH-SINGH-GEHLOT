// 0/1 Knapsack using Branch and Bound
// Items are sorted by value/weight ratio. Each node of the search tree
// stands for a decision about one item (take it / leave it).
// For every node we calculate an UPPER BOUND on the profit it can still
// reach (by allowing a fraction of the next item). A node is discarded
// if its bound is not better than the best profit found so far.
// A priority queue always expands the node with the highest bound first.

#include <iostream>
#include <queue>
#include <algorithm>
using namespace std;

struct Item {
    int id;
    int weight;
    int value;
};

struct Node {
    int level;          // index of the last item decided (-1 for the root)
    int profit;
    int weight;
    double bound;
    long long chosen;   // bit i is 1 if the i-th sorted item is taken
};

struct CompareNode {
    bool operator()(const Node& a, const Node& b) {
        return a.bound < b.bound;       // largest bound first
    }
};

bool compareRatio(Item a, Item b) {
    return (double)a.value / a.weight > (double)b.value / b.weight;
}

// maximum profit this node could possibly reach
double calcBound(Node u, int n, int capacity, Item items[]) {
    if (u.weight > capacity) {
        return 0;
    }
    double bound = u.profit;
    int j = u.level + 1;
    int totalWeight = u.weight;

    // take whole items while they fit
    while (j < n && totalWeight + items[j].weight <= capacity) {
        totalWeight = totalWeight + items[j].weight;
        bound = bound + items[j].value;
        j++;
    }
    // take a fraction of the next item
    if (j < n) {
        bound = bound + (capacity - totalWeight) * ((double)items[j].value / items[j].weight);
    }
    return bound;
}

int main() {
    int n = 4;
    int capacity = 10;
    Item items[4] = {
        {1, 4, 40},
        {2, 7, 42},
        {3, 5, 25},
        {4, 3, 12}
    };

    sort(items, items + n, compareRatio);

    priority_queue<Node, vector<Node>, CompareNode> pq;
    Node root;
    root.level = -1;
    root.profit = 0;
    root.weight = 0;
    root.chosen = 0;
    root.bound = calcBound(root, n, capacity, items);
    pq.push(root);

    int maxProfit = 0;
    long long bestChosen = 0;
    int nodesExpanded = 0;

    while (!pq.empty()) {
        Node u = pq.top();
        pq.pop();

        // this node cannot give a better answer, so cut it off
        if ((int)u.bound <= maxProfit) {
            continue;
        }
        nodesExpanded++;

        int next = u.level + 1;
        if (next >= n) {
            continue;
        }

        // branch 1: take the next item
        Node withItem = u;
        withItem.level = next;
        withItem.weight = u.weight + items[next].weight;
        withItem.profit = u.profit + items[next].value;
        withItem.chosen = u.chosen | (1LL << next);

        if (withItem.weight <= capacity && withItem.profit > maxProfit) {
            maxProfit = withItem.profit;
            bestChosen = withItem.chosen;
        }
        withItem.bound = calcBound(withItem, n, capacity, items);
        if (withItem.weight <= capacity && (int)withItem.bound > maxProfit) {
            pq.push(withItem);
        }

        // branch 2: leave the next item
        Node withoutItem = u;
        withoutItem.level = next;
        withoutItem.bound = calcBound(withoutItem, n, capacity, items);
        if ((int)withoutItem.bound > maxProfit) {
            pq.push(withoutItem);
        }
    }

    cout << "Capacity: " << capacity << endl;
    cout << "Items sorted by value/weight (id, weight, value):" << endl;
    for (int i = 0; i < n; i++) {
        cout << "  item " << items[i].id << "  w=" << items[i].weight
             << "  v=" << items[i].value << endl;
    }

    cout << endl << "Maximum profit = " << maxProfit << endl;
    cout << "Items taken: ";
    int totalWeight = 0;
    for (int i = 0; i < n; i++) {
        if (bestChosen & (1LL << i)) {
            cout << items[i].id << " ";
            totalWeight = totalWeight + items[i].weight;
        }
    }
    cout << "(total weight " << totalWeight << ")" << endl;
    cout << "Nodes expanded: " << nodesExpanded << " (a full tree has "
         << (1 << (n + 1)) - 1 << " nodes)" << endl;
    return 0;
}
