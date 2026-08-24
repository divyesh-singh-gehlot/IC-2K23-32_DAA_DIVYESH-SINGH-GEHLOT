#include<bits/stdc++.h>
using namespace std;

void InsertionSort(vector<int>& arr, int n, int i){
    int j = i;
    if(j > n-1) return;
    while(j > 0 && arr[j-1] > arr[j]){
        swap(arr[j], arr[j-1]);
        j--;
    }
    InsertionSort(arr, n , i+1);
}

int main(){
    int n;
    cout << "Enter the number of values you want: ";
    cin >> n;

    vector<int> arr(n);

    cout << "Enter elements: ";
    for(int i=0; i<arr.size(); i++){
        cin >> arr[i];
    }

    cout << "Unsorted array: ";
    for(auto it: arr){
        cout << it << " ";
    }

    cout << endl;

    InsertionSort(arr, arr.size(), 1);

    cout << "Sorted array: ";
    for(auto it: arr){
        cout << it << " ";
    }
}