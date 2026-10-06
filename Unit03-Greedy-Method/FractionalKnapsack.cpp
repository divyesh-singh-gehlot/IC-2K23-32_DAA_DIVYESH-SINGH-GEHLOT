// Fractional Knapsack (Greedy)
// Take items in decreasing order of value/weight ratio.
// If the last item does not fit completely, take only a fraction of it.

#include <iostream>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Item {
    int id;
    double weight;
    double value;
    double ratio;
};

// sort so that the item with the highest ratio comes first
bool compare(Item a, Item b) {
    return a.ratio > b.ratio;
}

int main() {
    int n = 3;
    double capacity = 50;

    Item items[3] = {
        {1, 10, 60, 0},
        {2, 20, 100, 0},
        {3, 30, 120, 0}
    };

    for (int i = 0; i < n; i++) {
        items[i].ratio = items[i].value / items[i].weight;
    }

    sort(items, items + n, compare);

    double remaining = capacity;
    double totalValue = 0;

    cout << "Knapsack capacity: " << capacity << endl << endl;
    cout << "Item  Weight  Value     Ratio     Taken" << endl;

    for (int i = 0; i < n; i++) {
        double taken;
        if (items[i].weight <= remaining) {
            // whole item fits
            taken = 1.0;
            remaining = remaining - items[i].weight;
            totalValue = totalValue + items[i].value;
        } else {
            // take only the part that fits
            taken = remaining / items[i].weight;
            totalValue = totalValue + items[i].value * taken;
            remaining = 0;
        }

        cout << setw(4) << items[i].id << setw(8) << items[i].weight
             << setw(7) << items[i].value << setw(10) << items[i].ratio
             << setw(10) << taken << endl;

        if (remaining == 0) {
            break;
        }
    }

    cout << endl << "Maximum value = " << totalValue << endl;
    return 0;
}