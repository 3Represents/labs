#include "ABR.hpp"

int main() {
    Noeud<int>* n1(new Noeud<int>(10));
    Noeud<int>* n2(new Noeud<int>(5));
    Noeud<int>* n3(new Noeud<int>(15));
    Noeud<int>* n4(new Noeud<int>(2));
    Noeud<int>* n5(new Noeud<int>(7));
    Noeud<int>* n6(new Noeud<int>(12));
    Noeud<int>* n7(new Noeud<int>(18));
    Noeud<int>* n8(new Noeud<int>(1));
    Noeud<int>* n9(new Noeud<int>(3));
    Noeud<int>* n10(new Noeud<int>(14));

    n1->fg = n2; n1->fd = n3;
    n2->p = n1; n2->fg = n4; n2->fd = n5;
    n3->p = n1; n3->fg = n6; n3->fd = n7;
    n4->p = n2; n4->fg = n8; n4->fd = n9;
    n5->p = n2;
    n6->p = n3; n6->fd = n10;
    n7->p = n3;
    n8->p = n4;
    n9->p = n4;
    n10->p = n6;
    
    ABR<int> tree(n1);
    tree.printInfixe();

    return 0;
}