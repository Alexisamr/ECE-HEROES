
#include <stdio.h>
#include <windows.h>
#include "projet.h"

// Fonction pour afficher UN sprite 15x15 à une position donnée
void afficherSprite(int x, int y, int type, int couleurFond) {
    // Sprites 15x15 pour chaque type de bonbon
    // Type 0 = vide, Type 1-5 = les bonbons
    
    char* sprite0[15] = {
        "               ",
        "               ",
        "               ",
        "               ",
        "               ",
        "               ",
        "               ",
        "               ",
        "               ",
        "               ",
        "               ",
        "               ",
        "               ",
        "               ",
        "               "
    };
    
    char* sprite1[15] = {
        "   ██████████  ",
        "  ████████████ ",
        " ██████████████",
        " ██████████████",
        "████████████████",
        "████████████████",
        "████████████████",
        "████████████████",
        "████████████████",
        " ██████████████",
        " ██████████████",
        "  ████████████ ",
        "   ██████████  ",
        "               ",
        "               "
    };
    
    char* sprite2[15] = {
        "      ████     ",
        "     ██████    ",
        "    ████████   ",
        "   ██████████  ",
        "  ████████████ ",
        " ██████████████",
        " ██████████████",
        "████████████████",
        " ██████████████",
        " ██████████████",
        "  ████████████ ",
        "   ██████████  ",
        "    ████████   ",
        "     ██████    ",
        "      ████     "
    };
    
    char* sprite3[15] = {
        "  ████    ████ ",
        " ██████  ██████",
        "██████████████ ",
        "████████████████",
        "████████████████",
        " ██████████████",
        "  ████████████ ",
        "   ██████████  ",
        "  ████████████ ",
        " ██████████████",
        "████████████████",
        "████████████████",
        "██████████████ ",
        " ██████  ██████",
        "  ████    ████ "
    };
    
    char* sprite4[15] = {
        "     ██████    ",
        "    ████████   ",
        "   ██████████  ",
        "  ████████████ ",
        " ██████████████",
        "████████████████",
        "████████████████",
        "████████████████",
        "████████████████",
        "████████████████",
        " ██████████████",
        "  ████████████ ",
        "   ██████████  ",
        "    ████████   ",
        "     ██████    "
    };
    
    char* sprite5[15] = {
        "   ██      ██  ",
        "  ████    ████ ",
        " ██████  ██████",
        " ██████████████",
        "████████████████",
        "████████████████",
        " ██████████████",
        "  ████████████ ",
        "   ██████████  ",
        "    ████████   ",
        "     ██████    ",
        "      ████     ",
        "       ██      ",
        "               ",
        "               "
    };
    
    // On choisit le bon sprite selon le type
    char** spriteActuel;
    int couleurTexte = 15;
    
    if (type == 0) {
        spriteActuel = sprite0;
        couleurTexte = 8;
    } else if (type == 1) {
        spriteActuel = sprite1;
        couleurTexte = 12; // Rouge
    } else if (type == 2) {
        spriteActuel = sprite2;
        couleurTexte = 10; // Vert
    } else if (type == 3) {
        spriteActuel = sprite3;
        couleurTexte = 14; // Jaune
    } else if (type == 4) {
        spriteActuel = sprite4;
        couleurTexte = 11; // Cyan
    } else {
        spriteActuel = sprite5;
        couleurTexte = 13; // Violet
    }
    
    // On affiche ligne par ligne
    int i;
    for (i = 0; i < 15; i++) {
        Gotoxy(x, y + i);
        Color(couleurTexte, couleurFond);
        printf("%s", spriteActuel[i]);
    }
    
    Color(15, 0); // On remet la couleur normale
}

// Fonction principale du moteur graphique
void afficherGrilleGraphique(t_jeu jeu, int curseurX, int curseurY, int selectionActive) {
    int startX = 2;
    int startY = 6;
    int tailleSprite = 15;
    
    int i, j;
    
    // On parcourt toute la grille
    for (i = 0; i < LIGNES; i++) {
        for (j = 0; j < COLONNES; j++) {
            int valeur = jeu.grille[i][j];
            
            // Si la valeur est invalide, on la met à 0
            if (valeur < 0 || valeur > 5) valeur = 0;
            
            // On calcule la position en pixels/caractères
            int posX = startX + (j * tailleSprite);
            int posY = startY + (i * tailleSprite);
            
            // On détermine la couleur de fond (pour le curseur)
            int couleurFond = 0;
            if (j == curseurX && i == curseurY) {
                if (selectionActive) {
                    couleurFond = 5; // Fond violet si sélection active
                } else {
                    couleurFond = 8; // Fond gris pour le curseur
                }
            }
            
            // On affiche le sprite
            afficherSprite(posX, posY, valeur, couleurFond);
        }
    }
}
