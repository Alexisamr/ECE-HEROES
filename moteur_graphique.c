#include <stdio.h>
#include <windows.h>
#include "projet.h"

void afficherGrilleGraphique(t_jeu jeu, int curseurX, int curseurY, int selectionActive) {
    int startX = 15;
    int startY = 5;
    
    // symboles pour chaque type avec les couleurs 
    char* symboles[] = {" ", "O", "#", "@", "+", "*"};
    int couleurs[] = {8, 12, 10, 14, 11, 13};
    
    // cadre du haut
    Color(15, 0);
    Gotoxy(startX - 1, startY - 1); 
    printf("╔");
    int k;
    for(k = 0; k < COLONNES; k++) printf("══");
    printf("╗");
    
    // la grille
    int i, j;
    for (i = 0; i < LIGNES; i++) {
        Color(15, 0);
        Gotoxy(startX - 1, startY + i);
        printf("║");
        
        for (j = 0; j < COLONNES; j++) {
            int val = jeu.grille[i][j];
            if (val < 0 || val > 5) val = 0;
            
            int couleurTexte = couleurs[val];
            int couleurFond = 0;
            
            if (j == curseurX && i == curseurY) {
                couleurFond = selectionActive ? 5 : 8;
                if (val != 0) couleurTexte = 15;
            }
            
            Gotoxy(startX + (j * 2), startY + i);
            Color(couleurTexte, couleurFond);
            printf("%s ", symboles[val]);
        }
        
        Color(15, 0);
        printf("║");
    }
    
    // cadre du bas
    Gotoxy(startX - 1, startY + LIGNES);
    printf("╚");
    for(k = 0; k < COLONNES; k++) printf("══");
    printf("╝");
    
    Color(15, 0);
}
