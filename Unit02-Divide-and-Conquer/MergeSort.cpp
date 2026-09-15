#include<bits/stdc++.h>
using namespace std;

void merge(vector<int>& arr, int low, int mid, int high){
    vector<int> temp;
    int left = low;
    int right = mid + 1;

    while(left <= mid && right <= high){
        if(arr[left] <= arr[right]){
            temp.push_back(arr[left]);
            left++;
        }
        else{
            temp.push_back(arr[right]);
            right++;
        }
    }

    while(left <= mid){
        temp.push_back(arr[left]);
        left++;
    }

    while(right <= high){
        temp.push_back(arr[right]);
        right++;
    }

    for(int i = low; i<=high; i++){
        arr[i] = temp[i - low];
    }
}

void mergeSort(vector<int>& arr, int low, int high){
    if (low == high) return;
    int mid = (low + high) / 2;
    mergeSort(arr, low, mid);
    mergeSort(arr, mid + 1, high);
    merge(arr, low, mid, high);
}

int main(){
    vector<int> arr(6);

    cout << "Enter 6 elements: ";
    for(int i=0; i<arr.size(); i++){
        cin >> arr[i];
    }

    cout << "Unsorted array: ";
    for(auto it: arr){
        cout << it << " ";
    }

    cout << endl;

    mergeSort(arr, 0, arr.size()-1);

    cout << "Sorted array: ";
    for(auto it: arr){
        cout << it << " ";
    }
}