#pragma once
#include "Noeud.hpp"
#include <algorithm>
#define ERR_BA "Erreur : plus assez de mémoire !"

template <class T>
class Arbre {
protected :
    Noeud<T>* _racine;  // Pointeur sur la racine
    Noeud<T>* _courant; // Pointeur sur l’élément courant
    int _nbElem;        // Nombre de noeuds
    // Copie récursive d’un arbre de racine r;
    Noeud<T>* copyTree(Noeud<T>* r, Noeud<T>* father);
public:
    /*** Constructeurs/Desctructeur ***/
    Arbre() : _racine(NULL), _courant(NULL), _nbElem(0) {}
    Arbre(Noeud<T>* r); // Constructeur C1: creer l’arbre de racine r
    Arbre(const T& v);  // Constructeur C2: creer l’arbre de racine à valeur v
    Arbre(const Arbre<T>& a); // Constructeur par recopie
    ~Arbre();

    /*** Modification de l’arbre ***/
    // Permet d’ajouter un élément par rapport à l’élément courant
    void addLeftSon(const T& valg);  // leve l’exception bad_alloc en cas de pb
    void addRightSon(const T& vald); // idem
    void remove(Noeud<T>* r); // Suppression du noeud r et du sous-arbre de sommet r
    void clear();             // Supprime tous les noeuds de l’arbre; maj des pointeurs

    /*** Accesseurs ***/
    Noeud<T>* rootNode() const { return _racine; }
    int size() const { return _nbElem; }
    Noeud<T>* currentNode() const { return _courant; }
    Noeud<T>* leftSon() const { if (_courant != NULL) return _courant->fg; }
    Noeud<T>* rightSon() const { if (_courant != NULL) return _courant->fd; }
    Noeud<T>* father() const { if (_courant != NULL) return _courant->p; }
    // Renvoit la valeur du noeud courant
    T value() const { assert(_courant != NULL); return _courant->val; }
    T& value() { assert(_courant != NULL); return _courant->val; }
    // Renvoie les valeurs des fils et du pere sans changer le pointeur _courant
    T peekLeft() const;
    T peekRight() const;
    T peekFather() const;

    /*** Navigation dans l’arbre - Deplacement du pointeur _courant ***/
    void left() { if (_courant != NULL) _courant=_courant->fg; }
    void right() { if (_courant != NULL) _courant=_courant->fd; }
    void father(){ if (_courant != NULL) _courant=_courant->p; }
    void reset() { _courant=_racine; }             // retour à la racine
    void setCurrent(Noeud<T>* r) { _courant = r; } // déplace _courant en r
    
    /*** Accès au fils gauche/droit et au pere du noeud r, NULL sinon ***/
    Noeud<T>* leftSon(Noeud<T>* r) { return ((r != NULL)?(r->fg):NULL); }
    Noeud<T>* rightSon(Noeud<T>* r) { return ((r != NULL)?(r->fd):NULL); }
    Noeud<T>* father(Noeud<T>* r) { return ((r != NULL)?(r->p):NULL); }

    /*** Récupérations d’infos sur l’arbre ***/
    int h() const { return h(_racine); }              // hauteur de l’arbre
    int nbLeaf() const { return nbLeaf(_racine); }    // nombre de feuilles de l’arbre
    bool isEmpty() const { return isEmpty(_racine); } // l’arbre est il vide?
    // Mêmes fonctions mais pour l’arbre de racine r
    int h(Noeud<T>* r) const;
    int nbLeaf(Noeud<T>* r) const;
    bool isEmpty(Noeud<T>* r) const { return (r == NULL); }
    // Accès au caractéristiques d’un noeud
    bool isLeaf(Noeud<T>* r) const { return (r!= NULL)&&(r->fg==NULL)&& (r->fd==NULL); } // r est-t-il une feuille?
    unsigned long long nbNode(Noeud<T>* r); // nombre de noeud dans le sous-arbre
    // Affichage des informations contenant l’arbre
    void printInfo();
    
    /*** Affichage selon diff. parcours - suppose que T supporte l’opérateur << ***/
    void printInfixe() { printInfixe(_racine); }    // Parcours infixe
    void printPrefixe() { printPrefixe(_racine); }  // Parcours prefixe
    void printPostfixe(){ printPostfixe(_racine); } // Parcours postfixe
    // idem, mais a partir d’un noeud r
    void printInfixe(Noeud<T>* r);   // Parcours infixe
    void printPrefixe(Noeud<T>* r);  // Parcours prefixe
    void printPostfixe(Noeud<T>* r); // Parcours postfixe
};

template <class T>
Arbre<T>::Arbre(Noeud<T>* r): _racine(r), _courant(_racine), _nbElem(nbNode(_racine)) {}

template <class T>
Arbre<T>::Arbre(const T& v) {
    try {
        _racine = new Noeud<T>(v);
    } catch (const bad_alloc& e) {
        cerr << ERR_BA << endl ;
        exit(1);
    }

    _courant = _racine;
    _nbElem = 1;
}

template <class T>
Noeud<T>* Arbre<T>::copyTree(Noeud<T>* r, Noeud<T>* father) {
    Noeud<T>* newRoot;
    try {
        newRoot = new Noeud<T>(*r);
    } catch (const bad_alloc& e) {
        cerr << ERR_BA << endl ;
        exit(1);
    }

    Noeud<T> *newLeft(NULL), *newRight(NULL);
    if (newRoot->fg)
        newLeft  = copyTree(newRoot->fg, newRoot);
    if (newRoot->fd)
        newRight = copyTree(newRoot->fd, newRoot);
    
    newRoot->fg = newLeft;
    newRoot->fd = newRight;
    newRoot->p  = father;
    
    return newRoot;
}

template <class T>
Arbre<T>::Arbre(const Arbre<T>& a)
    : _racine(copyTree(a._racine, NULL))
    , _courant(_racine)
    , _nbElem(a._nbElem)
{}

template <class T>
void Arbre<T>::remove(Noeud<T>* r) {
    if (r) {
        remove(r->fg);
        remove(r->fd);

        if (r->p) {
            if (r->p->fg == r)
                r->p->fg = NULL;
            else
                r->p->fd = NULL;
        }
        
        _courant = r->p; 
        if (r == _racine)
            _racine = NULL;

        delete r;
    }
}

template <class T>
void Arbre<T>::clear() {
    remove(_racine);
}

template <class T>
Arbre<T>::~Arbre() {
    clear();
}

template <class T>
void Arbre<T>::addLeftSon(const T& valg) {
    if (_racine == NULL) {
        try {
            _racine = new Noeud<T>(valg);
        } catch (const bad_alloc& e) {
            cerr << ERR_BA << endl ;
            exit(1);
        }
        reset();
    } else {
        assert(_courant != NULL);
        if (_courant->fg) {
            _courant->fg->val = valg;
        } else {
            Noeud<T>* newNode;

            try {
                newNode = new Noeud<T>(valg);
            } catch(const bad_alloc& e) {
                cerr << ERR_BA << endl ;
                exit(1);
            }
            
            _courant->fg = newNode;
            newNode->p = _courant;
        }
    }
}

template <class T>
void Arbre<T>::addRightSon(const T& vald) { 
    if (_racine == NULL) {
        try {
            _racine = new Noeud<T>(vald);
        } catch (const bad_alloc& e) {
            cerr << ERR_BA << endl ;
            exit(1);
        }
        reset();
    } else {
        assert(_courant != NULL);
        if (_courant->fd) {
            _courant->fd->val = vald;
        } else {
            Noeud<T>* newNode;

            try {
                newNode = new Noeud<T>(vald);
            } catch(const bad_alloc& e) {
                cerr << ERR_BA << endl ;
                exit(1);
            }

            _courant->fd = newNode;
            newNode->p = _courant;
        }
    }
}

template <class T>
T Arbre<T>::peekLeft() const {
    assert(_courant->fg != NULL);
    return _courant->fg->val;
}

template <class T>
T Arbre<T>::peekRight() const {
    assert(_courant->fd != NULL);
    return _courant->fd->val;
}

template <class T>
T Arbre<T>::peekFather() const  {
    assert(_courant->p != NULL);
    return _courant->p->val;
}

template <class T>
int Arbre<T>::h(Noeud<T>* r) const {
    if (r == NULL)
        return -1;
    
    return max(h(r->fg), h(r->fd)) + 1;
}

template <class T>
int Arbre<T>::nbLeaf(Noeud<T>* r) const {
    if (r == NULL)
        return 0;
    if ((r->fg == NULL) && (r->fd == NULL))
        return 1;

    return nbLeaf(r->fg) + nbLeaf(r->fd);
}

template <class T>
unsigned long long Arbre<T>::nbNode(Noeud<T>* r) {
    if (r == NULL)
        return 0;
    if ((r->fg == NULL) && (r->fd == NULL))
        return 1;
    
    return 1 + nbNode(r->fg) + nbNode(r->fd);
}

template <class T>
void Arbre<T>::printInfixe(Noeud<T>* r) {
    assert(r != NULL);
    if (r->fg)
        printInfixe(r->fg);
    cout << r->val << " ";
    if (r->fd)
        printInfixe(r->fd);
}

template <class T>
void Arbre<T>::printPrefixe(Noeud<T>* r) {
    assert(r != NULL);
    cout << r->val << " ";
    if (r->fg)
        printPrefixe(r->fg);
    if (r->fd)
        printPrefixe(r->fd);
}

template <class T>
void Arbre<T>::printPostfixe(Noeud<T>* r) {
    assert(r != NULL);
    if (r->fg)
        printPostfixe(r->fg);
    if (r->fd)
        printPostfixe(r->fd);
    cout << r->val << " ";
}