#include <stdio.h>
#include <stdlib.h>

typedef struct exclusion{
    int sommet1;
    int sommet2;
}t_exclusion;

typedef struct machine {
    int numeroMachine;
    int *operations;
    int nombreOperations;
} t_machine;

int main() {
    FILE *fichier = NULL;
    int sommet1, sommet2, i, j, k, l;
    int nombreSommets = 0;
    int nombreAretes = 0;
    int nb_machines = 0;
    int num_operation;
    int decision = 1;
    int doublon = 0;
    int temp;
    int compteur = 0;
    int compteur_adjacence = 0;
    int prev_k = 0;
    t_exclusion *tab_exclusion = NULL;
    t_machine *machines;

    fichier = fopen("exclusion.txt", "r");
    if(fichier == NULL){
        perror("Impossible d'ouvrir le fichier");
        exit(-1);
    }

    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {
        nombreAretes++;
        if (sommet1 > nombreSommets) {
            nombreSommets = sommet1;
        }
        if (sommet2 > nombreSommets) {
            nombreSommets = sommet2;
        }
    }
    fseek(fichier, 0, SEEK_SET);

    tab_exclusion = malloc(sizeof(t_exclusion) * nombreAretes);
    for(i=0; i<nombreAretes; i++){
        fscanf(fichier, "%d %d", &sommet1, &sommet2);
        tab_exclusion[i].sommet1 = sommet1;
        tab_exclusion[i].sommet2 = sommet2;
    }

    int **matrice_adjacence = malloc(sizeof(int*) * nombreSommets);
    for(i=0; i<nombreSommets; i++){
        matrice_adjacence[i] = malloc(nombreSommets * sizeof(int));
    }

    for(i=0; i<nombreSommets; i++){
        for(j=0; j<nombreSommets; j++){
            matrice_adjacence[i][j] = 0;
        }
    }

    for(i=0; i<nombreAretes; i++){
        matrice_adjacence[tab_exclusion[i].sommet1 - 1][tab_exclusion[i].sommet2 - 1] = 1;
        matrice_adjacence[tab_exclusion[i].sommet2 - 1][tab_exclusion[i].sommet1 - 1] = 1;
    }

    machines = malloc(sizeof(t_machine) * nombreSommets);
    for (i = 0; i < nombreSommets; i++) {
        machines[i].numeroMachine = 0;
        machines[i].operations = NULL;
        machines[i].nombreOperations = 0;
    }

    for(i=0; i<nombreSommets; i++){
        for(j=0; j<nombreSommets; j++){
            if(matrice_adjacence[i][j] == 1){
                decision = 0;
                for(k=nb_machines; k>=0; k--){
                    if (prev_k != k) {
                        compteur_adjacence = 0;
                    }
                    for(l=0; l<machines[k].nombreOperations; l++){
                        prev_k = k;

                        temp = machines[k].operations[l];
                        if(i+1 == temp){
                            doublon=1;
                            decision = 0;
                        }
                        if(matrice_adjacence[i][temp-1] == 0){
                            compteur_adjacence++;
                        }
                        if(compteur_adjacence == machines[k].nombreOperations){
                            decision = k;
                        }
                        else if((matrice_adjacence[i][temp-1] == 1) && (decision != k+2)){
                            decision = k+1;
                        }
                    }
                }
                if(doublon == 0){
                    if(decision > nb_machines){
                        nb_machines = decision;
                    }
                    if(machines[decision].nombreOperations == 0){
                        machines[decision].operations = malloc(sizeof(int));
                        machines[decision].operations[0] = i+1;
                        machines[decision].nombreOperations++;
                    }
                    else{
                        num_operation = machines[decision].nombreOperations;
                        machines[decision].operations = realloc(machines[decision].operations, (num_operation+1)*sizeof(int));
                        machines[decision].operations[num_operation] = i+1;
                        machines[decision].nombreOperations++;
                    }
                }
                doublon = 0;

                decision = 0;
                for(k=nb_machines; k>=0; k--){
                    if (prev_k != k) {
                        compteur_adjacence = 0;
                    }
                    for(l=0; l<machines[k].nombreOperations; l++){
                        prev_k = k;

                        temp = machines[k].operations[l];
                        if(j+1 == temp){
                            doublon=1;
                            decision = 0;
                        }
                        if(matrice_adjacence[j][temp-1] == 0){
                            compteur_adjacence++;
                        }

                        if(compteur_adjacence == machines[k].nombreOperations){
                            decision = k;
                        }
                        else if((matrice_adjacence[j][temp-1] == 1) && (decision != k+2)){
                            decision = k+1;
                        }
                    }
                }
                if(doublon==0){
                    if(decision > nb_machines){
                        nb_machines = decision;
                    }
                    if(machines[decision].nombreOperations == 0){
                        machines[decision].operations = malloc(sizeof(int));
                        machines[decision].operations[0] = j+1;
                        machines[decision].nombreOperations++;
                    }
                    else{
                        num_operation = machines[decision].nombreOperations;
                        machines[decision].operations = realloc(machines[decision].operations, (num_operation+1)*sizeof(int));
                        machines[decision].operations[num_operation] = j+1;
                        machines[decision].nombreOperations++;
                    }
                }
                doublon = 0;
            }
            else{
                compteur++;
            }

            if(compteur == nombreSommets){
                num_operation = machines[0].nombreOperations;
                machines[0].operations = realloc(machines[0].operations, (num_operation+1)*sizeof(int));
                machines[0].operations[num_operation] = i+1;
                machines[0].nombreOperations++;
            }
        }
        compteur = 0;
    }


    printf("Nombre minimal de machines nécessaires : %d\n", nb_machines+1);

    for (i=0; i<nb_machines+1; i++) {
        printf("Machine %d : ", i+1);
        for (j=0; j<machines[i].nombreOperations; j++) {
            printf("%d ", machines[i].operations[j]);
        }
        printf("\n");
        free(machines[i].operations);
    }
    free(machines);

    for (i = 0; i < nombreSommets; ++i) {
        free(matrice_adjacence[i]);
    }
    free(matrice_adjacence);
    free(tab_exclusion);
}
