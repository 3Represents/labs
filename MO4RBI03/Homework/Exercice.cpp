#include <iostream>
#include <set>
#include <string>

using namespace std;

int main() {
    // 1.
    set<char> ensemble({'a', 'b', 'c'});

    // 2.
    cout << "Itérateur : ";
    for (set<char>::iterator i(ensemble.begin()); i != ensemble.end(); i++) {
        cout << *i << ", ";
    }
    cout << endl;

    // 3.
    string chaine;
    chaine.resize(ensemble.size());
    copy(ensemble.begin(), ensemble.end(), chaine.begin());
    cout << "Chaîne de caractères : " << chaine << endl;

    // 4.
    cout << "Algorithme copy : ";
    copy(ensemble.begin(), ensemble.end(), ostream_iterator<char>(cout, ", "));
    cout << endl;

    return 0;
}