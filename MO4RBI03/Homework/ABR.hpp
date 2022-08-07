#pragma once
#include "Arbre.hpp"

template <class T>
class ABR : public Arbre<T> {
private:
    void insert(Noeud<T>* r, const T& v);// insert v dans le sous-arbre de sommet r
    void supprime(Noeud<T>* r, const T& v); // supprime v "
    void deleteNode(Noeud<T>* n); // supprime le noeud n dans l’arbre de
    Noeud<T>* find(Noeud<T>* r, const T& v); // cherche v "
    Noeud<T>* min(Noeud<T>* r); // recherche du noeud min dans le S-A de sommet r
    Noeud<T>* max(Noeud<T>* r); // recherche du noeud max dans le S-A de sommet r
public:
    /*** Constructeur/Destructeur ***/
    ABR() : Arbre<T>() {}
    ABR(Noeud<T>* r) : Arbre<T>(r) {}
    ABR(const T& v) : Arbre<T>(v) {}
    ABR(const ABR<T>& a) : Arbre<T>(a) {}
    ~ABR() {}

    // /*** Méthodes d’ajout/retrait/recherche ***/
    void insert(const T& v) { insert(this->_racine, v); } // insert v
    void supprime(const T& v) { supprime(this->_racine, v); } // supprime v
    Noeud<T>* find(const T& v) { return find(this->_racine, v); } // recherche v
    Noeud<T>* min() { return min(this->_racine); } // renvoie le noeud min
    Noeud<T>* max() { return max(this->_racine); } // renvoie le noeud min
    Noeud<T>* successor(Noeud<T>* r); // renvoie le successeur de r
};

template <class T>
Noeud<T>* ABR<T>::min(Noeud<T>* r) {
    assert(r != NULL);
    if (r->fg == NULL)
        return r;

    return min(r->fg);
}

template <class T>
Noeud<T>* ABR<T>::max(Noeud<T>* r) {
    assert(r != NULL);
    if (r->fd == NULL)
        return r;

    return max(r->fd);
}

template <class T>
Noeud<T>* ABR<T>::find(Noeud<T>* r, const T& v) {
    if (!r)
        return NULL;
    if (v == r->val)
        return r;
    if (v < r->val)
        return find(r->fg, v);
    else
        return find(r->fd, v);
}

template <class T>
Noeud<T>* ABR<T>::successor(Noeud<T>* r) {
    if (!r)
        return NULL;

    if (r->fd) {
        T target(r->val);
        r = r->fd;
        while (r->fg) {
            if (r->fg->val > target)
                r = r->fg;
        }
        return r;
    }
    
    if (r->p->fg == r)
        return r->p;

    while (r->p->fd == r)
        r = r->p;
    return r->p;
}

template <class T>
void ABR<T>::insert(Noeud<T> * r, const T& v) {
    if (r == NULL) {
        try {
            r = new Noeud<T>(v);
        } catch (const bad_alloc& e) {
            cerr << ERR_BA << endl;
            exit(1);
        }
    } else {
        this->setCurrent(r);
        while (this->_courant) {
            if (v < this->_courant->val) {
                if (this->_courant->fg) {
                    this->left();
                } else {
                    this->addLeftSon(v);
                    break;
                }
            } else {
                if (this->_courant->fd) {
                    this->right();
                } else {
                    this->addRightSon(v);
                    break;
                }
            }
        }
    }
}

template <class T>
void ABR<T>::supprime(Noeud<T>* r, const T& v) {
    Noeud<T>* toDelete(find(r, v));
    deleteNode(toDelete);
}

template <class T>
void ABR<T>::deleteNode(Noeud<T>* n) {
    if (not n) {
        cerr << "Noeud n'existe pas." << endl;
        exit(1);
    }

    if (!(n->fg) && !(n->fd)) {
        if (n->p) {
            if (n->p->fg == n)
                n->p->fg = NULL;
            else
                n->p->fd = NULL;
        }
        delete n;
    } else if (n->fg && !(n->fd)) {
        n->val = n->fg->val;
        delete n->fg;
        n->fg = NULL;
    } else if (n->fd && !(n->fg)) {
        n->val = n->fd->val;
        delete n->fd;
        n->fd = NULL;
    } else {
        Noeud<T>* nSucc(successor(n));
        n->val = nSucc->val;
        deleteNode(nSucc);
    }
}