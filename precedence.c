#include <malloc.h>
#include <mbstring.h>
#include <stdio.h>
#include "precedence.h"
#include<stdlib.h>
#include <limits.h>


void determiner_nombre_sommet_arc(FILE* nomFichier, int* nombre_sommets, int* nombre_arcs)
{
    int s1, s2, poids;
    int num_sommet_max = 0; // sommet le plus élevé est le 1er

    *nombre_sommets = 0;
    *nombre_arcs = 0;

    if (!nomFichier) {
        printf("Erreur de lecture fichier\n");
        return;
    }

    // Lit les arcs du graphe à partir du fichier
    while (fscanf(nomFichier, "%d %d %d", &s1, &s2, &poids) == 3)
    {
        // Met à jour  le nombre d'arcs
        (*nombre_arcs)++;

        // trouver le sommet le plus élevé
        if (s1 > num_sommet_max) {
            num_sommet_max = s1;
        }
        if (s2 > num_sommet_max) {
            num_sommet_max = s2;
        }
    }

    //determ ordre
    *nombre_sommets = num_sommet_max + 1;

}
//l'algorithme classique du plus long chemin fonctionne mieux avec des poids négatifs
void ajouter_arc(Graphe* graphe, int sommet_depart, int sommet_arrivee, int poids) {
    // Vérifier si les sommets sont valides
    if (sommet_depart < 0 || sommet_depart >= graphe->ordre ||
        sommet_arrivee < 0 || sommet_arrivee >= graphe->ordre) {
        printf("Erreur : sommet(s) invalide(s)\n");
        return;
    }

    // Créer un nouvel arc
     pArc nouvel_arc = ( pArc )malloc(sizeof( struct Arc));
    if (!nouvel_arc) {
        printf("Erreur d'allocation de mémoire pour un arc\n");
        exit(-1);
    }

    nouvel_arc->s = sommet_arrivee;
    nouvel_arc->arc_suivant = NULL;
    nouvel_arc->poids = -poids;

    // Trouver le dernier arc dans la liste d'arcs du sommet de départ
    pArc dernier_arc = graphe->pSommet[sommet_depart]->arc;
    if (!dernier_arc) {
        // Si la liste d'arcs est vide, ajouter le nouvel arc directement
        graphe->pSommet[sommet_depart]->arc = nouvel_arc;
    } else {
        // Sinon, parcourir la liste jusqu'au dernier arc
        while (dernier_arc->arc_suivant) {
            dernier_arc = dernier_arc->arc_suivant;
        }

        // Ajouter le nouvel arc à la fin de la liste
        dernier_arc->arc_suivant = nouvel_arc;
    }
}

Graphe * lire_graphe(FILE * nomFichier) {
    Graphe *graphe;

    FILE *ifs = fopen(nomFichier, "r");
    int taille, ordre, s1, s2, poids;

    if (!ifs) {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }
    // permet de déterminer le nb de  sommets
    fscanf(ifs, "%d", &ordre);

    int nombre_sommets, nombre_arcs;
    determiner_nombre_sommet_arc(ifs, &nombre_sommets, &nombre_arcs);

    // allouer de la memoire pour le graphe
    graphe = (Graphe*)malloc(sizeof(Graphe));
    if (!graphe)
    {
        printf("Erreur d'allocation de mémoire pour le graphe\n");
        exit(-1);
    }

    graphe->ordre = nombre_sommets; // Utiliser le nombre de sommets calculé
    graphe->taille = nombre_arcs;
    graphe->pSommet = (pSommet*)malloc(nombre_sommets * sizeof(pSommet));
    if (!graphe->pSommet)
    {
        printf("Erreur d'allocation de mémoire pour le tableau de sommets\n");
        exit(-1);
    }

    // Initialiser les sommets et stocke les infos sur les sommets
    graphe->pSommet = (pSommet*)malloc(nombre_sommets * sizeof(pSommet));
    if (!graphe->pSommet)
    {
        printf("Erreur d'allocation de mémoire pour le tableau de sommets\n");
        exit(-1);
    }
    for (int i = 0; i < nombre_sommets; i++)
    {
        graphe->pSommet[i] = (pSommet)malloc(sizeof( struct Sommet));
        if (!graphe->pSommet[i])
        {
            printf("Erreur d'allocation de mémoire pour un sommet\n");
            exit(-1);
        }
        graphe->pSommet[i]->valeur = i;
        graphe->pSommet[i]->arc = NULL; // Initialiser la liste des arcs à NULL
    }

    // Remettre le curseur au début du fichier
    rewind(ifs);
    while (fscanf(ifs, "%d %d %d", &s1, &s2, &poids) == 3)
    {
        // Appeler une fonction pour ajouter l'arc au graphe (à implémenter)
        ajouter_arc(graphe, s1, s2, poids);

    }


    fclose(ifs); // Fermer le fichier après utilisation

   return graphe;
}

void bellman_ford_plus_long_chemin(Graphe* graphe, int sommet_depart) {
    // Initialiser les distances à -INFINI
    for (int i = 0; i < graphe->ordre; i++) {
        graphe->pSommet[i]->distance = 0;
        graphe->pSommet[i]->pred = 0;



    }

    // La distance du sommet de départ est 0
    graphe->pSommet[sommet_depart]->distance = 0;

    // Appliquer l'algorithme de Bellman-Ford
    for (int i = 0; i < graphe->ordre - 1; i++) {
        for (int j = 0; j < graphe->ordre; j++) {
            pArc arc_actuel = graphe->pSommet[j]->arc;
            while (arc_actuel != NULL) {
                int sommet_arrivee = arc_actuel->s;
                int poids = arc_actuel->poids;

                // Relaxation de l'arc
                if (graphe->pSommet[j]->distance != INT_MIN &&
                    graphe->pSommet[j]->distance + poids > graphe->pSommet[sommet_arrivee]->distance) {
                    graphe->pSommet[sommet_arrivee]->distance = graphe->pSommet[j]->distance + poids;
                    graphe->pSommet[sommet_arrivee]->pred = j;
                }

                arc_actuel = arc_actuel->arc_suivant;
            }
        }
    }
}

// Fonction pour imprimer le chemin le plus long
void afficher_plus_long_chemin(Graphe* graphe, int sommet_depart, int sommet_arrivee) {
    if (graphe->pSommet[sommet_arrivee]->pred == -1) {
        printf("%d ", sommet_arrivee);
        return;
    }

    afficher_plus_long_chemin(graphe, sommet_depart, graphe->pSommet[sommet_arrivee]->pred);
    printf("%d ", sommet_arrivee);
}

// Fonction pour trouver le plus long chemin et l'imprimer
void trouver_et_imprimer_plus_long_chemin(Graphe* graphe, int sommet_depart, int sommet_arrivee) {
    bellman_ford_plus_long_chemin(graphe, sommet_depart);

    printf("Le plus long chemin de %d à %d est : ", sommet_depart, sommet_arrivee);
    imprimer_plus_long_chemin(graphe, sommet_depart, sommet_arrivee);
    printf("\n");
}













