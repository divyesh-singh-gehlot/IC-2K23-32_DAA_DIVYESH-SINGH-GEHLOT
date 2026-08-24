#include<bits/stdc++.h>
using namespace std;

void insertionSort(vector<int>& arr){
    for(int i=0; i<arr.size(); i++){
        int j = i;
        while(j > 0 && arr[j-1] > arr[j]){
            swap(arr[j-1], arr[j]);
            j--;
        }
    }
}

int main(){
    vector<int> arr(6);

    cout << "Enter elements: ";
    for(int i=0; i<arr.size(); i++){
        cin >> arr[i];
    }

    cout << "Unsorted array: ";
    for(auto it: arr){
        cout << it << " ";
    }

    cout << endl;

    insertionSort(arr);

    cout << "Sorted array: ";
    for(auto it: arr){
        cout << it << " ";
    }
}