#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n;
    cin >> n;

    vector<int> x(n);
    for (int& i : x) {
        cin >> i;
    }
    sort(x.begin(), x.end());

    int q;
    cin >> q;

    vector<int> m(q);
    for (int& i : m) {
        cin >> i;
    }

    for (const int& i : m) {
        int l(0), r(n), mid;

        while (l < r) {
            mid = (l + r) / 2;
            if (x[mid] <= i) {
                l = mid + 1;
            } else {
                r = mid;
            }
        }

        cout << l << endl;
    }

    return 0;
}