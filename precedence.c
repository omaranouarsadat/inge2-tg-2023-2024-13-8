#include <malloc.h>
#include <mbstring.h>
#include <stdio.h>
#include "precedence.h"
#include<stdlib.h>

Graphe * lire_graphe(FILE * nomFichier)
{
    Graphe* graphe;

    FILE * ifs = fopen(nomFichier,"r");
    int taille, ordre, s1, s2, poids;

    if (!ifs)
    {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }


    fscanf(ifs,"%d",&ordre);

    void determiner_nombre_sommet_arc(FILE* nomFichier, int* nombre_sommets, int* nombre_arcs)
    {
        int s1, s2, poids;
        int num_sommet_max = 0; // Utilisé pour suivre le sommet le plus élevé rencontré

        *nombre_sommets = 0;
        *nombre_arcs = 0;

        if (!nomFichier) {
            printf("Erreur de lecture fichier\n");
            return;
        }

        // Lire les arcs du graphe à partir du fichier
        while (fscanf(nomFichier, "%d %d %d", &s1, &s2, &poids) == 3)
        {
            // Mettre à jour le nombre d'arcs
            (*nombre_arcs)++;

            // Mettre à jour le sommet le plus élevé rencontré
            if (s1 > num_sommet_max) {
                max_sommet = s1;
            }
            if (s2 > num_sommet_max) {
                max_sommet = s2;
            }
        }

        // Le nombre de sommets est le sommet le plus élevé rencontré
        *nombre_sommets = max_sommet + 1;

        fclose(nomFichier);
    }
    graphe=CreerGraphe(ifs, ordre); // créer le graphe d'ordre sommets

    fscanf(ifs,"%d",&taille);





    graphe->ordre=ordre;

    // créer les arêtes du graphe
    for (int i=0; i<taille; ++i) {
        fscanf(ifs, "%d%d%d", &s1, &s2, &poids);
        graphe->pSommet = CreerArete(graphe->pSommet, s1, s2, poids);
        graphe->pSommet = CreerArete(graphe->pSommet, s2, s1, poids);
    }

    return graphe;
}

