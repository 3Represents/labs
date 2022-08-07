#include <iostream>
#include <vector>
using namespace std;

void max_heapify(vector<int>& a, const int& i, const int& n) {
    int l(2*i + 1), r(l + 1), largest(i);

    if (l < n && a[l] > a[largest]) {
        largest = l;
    }
    if (r < n && a[r] > a[largest]) {
        largest = r;
    }

    if (largest != i) {
        swap(a[i], a[largest]);
        max_heapify(a, largest, n);
    }
}

void heapsort(vector<int>& a, const int& n) {
    for (int i(n/2 - 1); i >= 0; i--) {
        max_heapify(a, i, n);
    }

    for (int i(n - 1); i >= 0; i--) {
        swap(a[0], a[i]);
        max_heapify(a, 0, i);
    }
}

int main() {
    int n;
    cin >> n;

    vector<int> a(n);
    for (int& i : a) {
        cin >> i;
    }

    heapsort(a, n); // or simply sort(a.begin(), a.end());
    
    for (const int& i : a) {
        cout << i << " ";
    }
    cout << endl;

    return 0;
}