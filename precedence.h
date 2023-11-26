#ifndef PRECEDENCE_H
#define PRECEDENCE_H

typedef struct Arc* pArc;

/* Structure d'un sommet*/
struct Sommet
{
    struct Arc* arc;
    int valeur;
    char couleur;
    int distance;
    int pred;
};
typedef struct Sommet* pSommet;




// Structure pour représenter un graphe
typedef struct Graphe {

    int taille;
    int ordre;
    // Tableau de sommets
    pSommet* pSommet;



} Graphe;


struct Arc
{
    int sommet; // numéro de sommet d'un arc adjacent au sommet initial

    struct Arc* arc_suivant;
    int poids;



};

typedef struct  {
    int longueur;
    struct Cellule* tete;
    struct  Cellule* queue;
} File;



#endif //PRECEDENCE_H