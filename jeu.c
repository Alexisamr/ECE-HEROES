#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 5
#define P 10


void initialisationplateau(int tab[N][P]) {
	int i, j;
	for (i = 0; i < N; i++) {
        // boucle pour parcourir "les colonnes" de la ligne i
        for (j = 0; j < P; j++) {
        	tab[i][j] = 0;
        }
    }
}

void remplissagecasesvides(int tab[N][P]) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < P; j++) {
        	if (tab[i][j]==0) {
        		tab[i][j] = rand() % 6;
        	}
        }
    }
}

void afficherplateauterminal(int tab[N][P]) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < P; j++) {
            printf("%3d ", tab[i][j]);
        }
        printf("\n");
    }
}

void gravite(int tab[N][P]) {
    int i, j;

    for (j = 0; j < P; j++) {                 // colonne
        for (i = N - 1; i > 0; i--) {        // ligne
            while (tab[i][j] == 0 && i > 0) {
                for (int k = i; k > 0; k--) {
                    tab[k][j] = tab[k - 1][j];
                }
                tab[0][j] = 0;
            }
        }
    }
}

