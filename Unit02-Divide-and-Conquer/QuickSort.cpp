#include<bits/stdc++.h>
using namespace std;

int partition(vector<int>& arr, int low, int high){
    int i = low, j = high, pivot=arr[low];
    while(i < j){
        while(arr[i] <= pivot && i < high){
            i++;
        }
        while(arr[j] > pivot && j > low){
            j--;
        }
        if(i<j) swap(arr[i], arr[j]);
    }
    swap(arr[low], arr[j]);
    return j;
}

void quickSort(vector<int>& arr, int low, int high){
    if(low < high){
        int partIndex = partition(arr, low, high);
        quickSort(arr, low, partIndex - 1);
        quickSort(arr, partIndex + 1, high);
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

    quickSort(arr, 0, arr.size()-1);

    cout << "Sorted array: ";
    for(auto it: arr){
        cout << it << " ";
    }
}