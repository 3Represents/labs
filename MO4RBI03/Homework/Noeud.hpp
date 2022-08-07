#pragma once
#include <iostream>
using namespace std;

template <class T>
class Noeud {
public:
    T val;                    // La valeur associée au noeud
    Noeud<T> *fg, *fd, *p;    // Pointeur sur fils gauche, fils droit et pere

    Noeud();                  // Constructeur par défaut
    Noeud(const T& i, Noeud<T>* leftSon = NULL, Noeud<T>* rightSon = NULL, Noeud<T>* father = NULL); // Constructeur principal
    Noeud(Noeud<T>& node);    // Constructeur par recopie
    template<class U> friend ostream& operator<<(ostream& o, const Noeud<U>& n);
};

template <class T> inline Noeud<T>::Noeud(): fg(NULL), fd(NULL), p(NULL) {}

template <class T> inline
Noeud<T>::Noeud(const T& i, Noeud<T>* leftSon, Noeud<T>* rightSon, Noeud<T>* father)
    : val(i)
    , fg(leftSon)
    , fd(rightSon)
    , p(father)
{}

template <class T> inline
Noeud<T>::Noeud(Noeud<T>& node)
    : val(node.val)
    , fg(node.fg)
    , fd(node.fd)
    , p(node.p)
{}

template<class U>
ostream& operator<<(ostream& o, const Noeud<U>& n) {
    o << "[" << n.val << " (p=";

    if (n.p)
        o << n.p;
    else
        o << "NULL";

    o << ", fg=";
    if (n.fg)
        o << n.fg;
    else
        o << "NULL";

    o << ", fd=";
    if (n.fd)
        o << n.fd;
    else
        o << "NULL";
        
    o << ") - @: " << &n << ']';

    return o;
}