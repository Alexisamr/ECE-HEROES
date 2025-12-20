#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "projet.h"

// Cette fonction choisit un chiffre au hasard pour remplir une case
static int typeAleatoire() {
    // on calcule un nombre entre 1 et le nombre maximum de types 
    return (rand() % NB_TYPES) + 1;
}

// Cette fonction vérifie si poser une valeur crée déjà une suite de 3 (pour éviter d'avoir des points gratuits au début)
static int creeSuite3(int grille[LIGNES][COLONNES], int x, int y, int valeur) {
    // Si la valeur est vide, on ne fait rien
    if (valeur <= 0) return 0;

    // On regarde à gauche : si les deux cases précédentes sont identiques à la valeur, ça fait 3
    if (x >= 2) {
        if (grille[y][x - 1] == valeur && grille[y][x - 2] == valeur) return 1;
    }
    // On regarde en haut : si les deux cases au-dessus sont identiques, ça fait 3
    if (y >= 2) {
        if (grille[y - 1][x] == valeur && grille[y - 2][x] == valeur) return 1;
    }
    // Sinon, pas de suite de 3 détectée
    return 0;
}

// On met toutes les cases de la grille à "VIDE"
void initialisationplateau(int grille[LIGNES][COLONNES]) {
    int i, j;
    // On parcourt chaque ligne
    for (i = 0; i < LIGNES; i++) {
        // On parcourt chaque colonne
        for (j = 0; j < COLONNES; j++) {
            // On met la case à l'état vide
            grille[i][j] = TYPE_VIDE;
        }
    }
}

// On remplit la grille au début du jeu sans qu'il y ait déjà des alignements de 3
void genererGrilleSansSuite(int grille[LIGNES][COLONNES]) {
    int x, y;
    // On commence par vider le plateau
    initialisationplateau(grille);

    // On parcourt toute la grille case par case
    for (y = 0; y < LIGNES; y++) {
        for (x = 0; x < COLONNES; x++) {
            // On choisit une valeur au hasard
            int valeur = typeAleatoire();
            // TANT QUE cette valeur crée une suite de 3, on en choisit une autre
            while (creeSuite3(grille, x, y, valeur)) {
                valeur = typeAleatoire();
            }
            // On place la valeur validée dans la grille
            grille[y][x] = valeur;
        }
    }
}

// Cette fonction fait tomber les cases vers le bas si il y a des trous (TYPE_VIDE)
void appliquerGravite(int grille[LIGNES][COLONNES]) {
    int colonne;
    // On traite le problème colonne par colonne
    for (colonne = 0; colonne < COLONNES; colonne++) {
        // On commence par le bas de la colonne (LIGNES - 1)
        int ecriture = LIGNES - 1;
        int lecture;

        // On remonte la colonne pour lire les cases
        for (lecture = LIGNES - 1; lecture >= 0; lecture--) {
            // Si la case n'est pas vide
            if (grille[lecture][colonne] != TYPE_VIDE) {
                // On déplace la case vers la position "ecriture" (le plus bas possible)
                grille[ecriture][colonne] = grille[lecture][colonne];
                // Si la case a bougé, on vide l'ancien emplacement
                if (ecriture != lecture) grille[lecture][colonne] = TYPE_VIDE;
                // On remonte le pointeur d'écriture d'un cran
                ecriture--;
            }
        }

        // Si il reste des cases vides en haut, on s'assure qu'elles sont bien marquées vides
        while (ecriture >= 0) {
            grille[ecriture][colonne] = TYPE_VIDE;
            ecriture--;
        }
    }
}

// On remplit les trous qui restent après la chute des cases
void remplissagecasesvides(int grille[LIGNES][COLONNES]) {
    int x, y;
    // On parcourt tout le plateau
    for (y = 0; y < LIGNES; y++) {
        for (x = 0; x < COLONNES; x++) {
            // Si une case est vide, on lui donne une nouvelle valeur au hasard
            if (grille[y][x] == TYPE_VIDE) {
                grille[y][x] = typeAleatoire();
            }
        }
    }
}

// On définit les réglages (objectifs, coups, temps) selon le niveau choisi
void chargerParametresNiveau(int niveau, int objectifs[NB_TYPES], int *coups_max, int *temps_max) {
    int i;
    // On met tous les objectifs à zéro par défaut
    for (i = 0; i < NB_TYPES; i++) objectifs[i] = 0;

    // Configuration pour le Niveau 1
    if (niveau == 1) {
        objectifs[0] = 35; // Il faut 35 du premier type
        objectifs[1] = 35; // 35 du deuxième
        objectifs[2] = 35; // 35 du troisième
        *coups_max = 35;   // En 35 coups maximum
        *temps_max = 90;   // En 90 secondes
        return;
    }

    // Configuration pour le Niveau 2
    if (niveau == 2) {
        objectifs[0] = 55;
        objectifs[1] = 45;
        objectifs[3] = 40;
        *coups_max = 40;
        *temps_max = 90;
        return;
    }

    // Configuration par défaut pour les autres niveaux (plus difficile)
    objectifs[1] = 70;
    objectifs[2] = 60;
    objectifs[3] = 55;
    objectifs[4] = 40;
    *coups_max = 45;
    *temps_max = 90;
}

// On inverse deux cases adjacentes (le mouvement de base du joueur)
int permuterCases(t_jeu *jeu, int x1, int y1, int x2, int y2) {
    int tmp;
    // On vérifie que les coordonnées sont bien dans la grille
    if (x1 < 0 || x1 >= COLONNES || y1 < 0 || y1 >= LIGNES) return 0;
    if (x2 < 0 || x2 >= COLONNES || y2 < 0 || y2 >= LIGNES) return 0;

    // On échange les valeurs des deux cases en passant par une variable temporaire
    tmp = jeu->grille[y1][x1];
    jeu->grille[y1][x1] = jeu->grille[y2][x2];
    jeu->grille[y2][x2] = tmp;
    return 1;
}

// C'est la fonction la plus complexe : elle cherche tous les alignements gagnants
int detecterMarques(t_jeu *jeu, int marque[LIGNES][COLONNES]) {
    int supprimerType[NB_TYPES + 1];
    int y, x, i;
    int nb = 0;

    // On remet le tableau des marques à zéro
    for (y = 0; y < LIGNES; y++) {
        for (x = 0; x < COLONNES; x++) marque[y][x] = 0;
    }
    // On remet à zéro les types à supprimer totalement
    for (i = 0; i <= NB_TYPES; i++) supprimerType[i] = 0;

    // ANALYSE HORIZONTALE
    for (y = 0; y < LIGNES; y++) {
        x = 0;
        while (x < COLONNES) {
            int val = jeu->grille[y][x];
            int debut = x;
            int longueur = 0;

            if (val < 1 || val > NB_TYPES) { x++; continue; }

            // On compte combien de cases identiques se suivent
            while (x < COLONNES && jeu->grille[y][x] == val) {
                longueur++;
                x++;
            }

            // Si plus de 6 : on marquera tout ce type pour suppression (bonus)
            if (longueur >= 6) supprimerType[val] = 1;
            // Si entre 3 et 5 : on marque ces cases pour être détruites
            if (longueur >= 3 && longueur < 6) {
                int k;
                for (k = debut; k < debut + longueur; k++) marque[y][k] = 1;
            }
        }
    }

    // ANALYSE VERTICALE (même logique que horizontale)
    for (x = 0; x < COLONNES; x++) {
        y = 0;
        while (y < LIGNES) {
            int val = jeu->grille[y][x];
            int debut = y;
            int longueur = 0;

            if (val < 1 || val > NB_TYPES) { y++; continue; }

            while (y < LIGNES && jeu->grille[y][x] == val) {
                longueur++;
                y++;
            }

            if (longueur >= 6) supprimerType[val] = 1;
            if (longueur >= 3 && longueur < 6) {
                int k;
                for (k = debut; k < debut + longueur; k++) marque[k][x] = 1;
            }
        }
    }

    // ANALYSE DES CARRES (4x4 de la même couleur)
    for (y = 0; y <= LIGNES - 4; y++) {
        for (x = 0; x <= COLONNES - 4; x++) {
            int val = jeu->grille[y][x];
            int yy, xx;
            int ok = 1;

            if (val < 1 || val > NB_TYPES) continue;

            // On vérifie si tout le bloc 4x4 est identique
            for (yy = 0; yy < 4; yy++) {
                for (xx = 0; xx < 4; xx++) {
                    if (jeu->grille[y + yy][x + xx] != val) ok = 0;
                }
            }

            // Si oui, on marque toutes les cases du carré
            if (ok) {
                for (yy = 0; yy < 4; yy++) {
                    for (xx = 0; xx < 4; xx++) marque[y + yy][x + xx] = 1;
                }
            }
        }
    }

    // ANALYSE EN CROIX (bonus spécial)
    for (y = 2; y <= LIGNES - 3; y++) {
        for (x = 2; x <= COLONNES - 3; x++) {
            int val = jeu->grille[y][x];
            if (val < 1 || val > NB_TYPES) continue;

            // On regarde si il y a une ligne de 5 ET une colonne de 5 qui se croisent
            if (jeu->grille[y - 1][x] == val && jeu->grille[y - 2][x] == val &&
                jeu->grille[y + 1][x] == val && jeu->grille[y + 2][x] == val &&
                jeu->grille[y][x - 1] == val && jeu->grille[y][x - 2] == val &&
                jeu->grille[y][x + 1] == val && jeu->grille[y][x + 2] == val) {

                int k;
                // On marque toute la ligne et toute la colonne
                for (k = 0; k < COLONNES; k++) if (jeu->grille[y][k] == val) marque[y][k] = 1;
                for (k = 0; k < LIGNES; k++) if (jeu->grille[k][x] == val) marque[k][x] = 1;
            }
        }
    }

    // Si un bonus "supprimerType" a été activé, on marque TOUTES les cases de ce type
    for (i = 1; i <= NB_TYPES; i++) {
        if (supprimerType[i]) {
            for (y = 0; y < LIGNES; y++) {
                for (x = 0; x < COLONNES; x++) {
                    if (jeu->grille[y][x] == i) marque[y][x] = 1;
                }
            }
        }
    }

    // On compte le nombre total de cases marquées pour la destruction
    for (y = 0; y < LIGNES; y++) {
        for (x = 0; x < COLONNES; x++) {
            if (marque[y][x]) nb++;
        }
    }

    return nb;
}

// On détruit effectivement les cases marquées
int appliquerMarques(t_jeu *jeu, int marque[LIGNES][COLONNES]) {
    int y, x;
    int nb = 0;

    for (y = 0; y < LIGNES; y++) {
        for (x = 0; x < COLONNES; x++) {
            // Si la case doit être supprimée
            if (marque[y][x]) {
                int val = jeu->grille[y][x];
                if (val >= 1 && val <= NB_TYPES) {
                    // On diminue l'objectif pour cette couleur si besoin
                    if (jeu->objectifs[val - 1] > 0) jeu->objectifs[val - 1]--;
                    // On ajoute 10 points au score
                    jeu->score += 10;
                }
                // On vide la case
                jeu->grille[y][x] = TYPE_VIDE;
                // On incrémente le compteur de cases supprimées
                nb++;
            }
        }
    }
    return nb;
}

// On regarde si un joueur existe déjà dans le fichier texte
int pseudoExiste(const char *pseudo) {
    FILE *f = fopen("sauvegarde.txt", "r");
    char pseudoLu[32];
    int niveauLu, viesLu, scoreLu;

    if (f == NULL) return 0;

    // On lit le fichier ligne par ligne
    while (fscanf(f, "%31s %d %d %d", pseudoLu, &niveauLu, &viesLu, &scoreLu) == 4) {
        // Si le nom correspond, on a trouvé !
        if (strcmp(pseudoLu, pseudo) == 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

// On récupère les données (niveau, vies, score) d'un joueur précis
int chargerSauvegarde(const char *pseudo, int *niveau, int *vies, int *score) {
    FILE *f = fopen("sauvegarde.txt", "r");
    char pseudoLu[32];
    int niveauLu, viesLu, scoreLu;

    if (f == NULL) return 0;

    while (fscanf(f, "%31s %d %d %d", pseudoLu, &niveauLu, &viesLu, &scoreLu) == 4) {
        if (strcmp(pseudoLu, pseudo) == 0) {
            // On remplit les variables avec les valeurs lues
            *niveau = niveauLu;
            *vies = viesLu;
            *score = scoreLu;
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

// On enregistre la progression du joueur
int sauvegarderSauvegarde(const char *pseudo, int niveau, int vies, int score) {
    char pseudos[200][32];
    int niveaux[200];
    int viesTab[200];
    int scores[200];
    int nb = 0;
    int trouve = 0;
    int i;

    // 1. On commence par lire toute la liste actuelle des joueurs pour ne pas les effacer
    FILE *f = fopen("sauvegarde.txt", "r");
    if (f != NULL) {
        while (nb < 200 && fscanf(f, "%31s %d %d %d", pseudos[nb], &niveaux[nb], &viesTab[nb], &scores[nb]) == 4) {
            nb++;
        }
        fclose(f);
    }

    // 2. On cherche si notre joueur est déjà dans la liste
    for (i = 0; i < nb; i++) {
        if (strcmp(pseudos[i], pseudo) == 0) {
            // Si oui, on met à jour ses infos
            niveaux[i] = niveau;
            viesTab[i] = vies;
            scores[i] = score;
            trouve = 1;
        }
    }

    // 3. Si c'est un nouveau joueur, on l'ajoute à la fin
    if (!trouve) {
        if (nb >= 200) return 0; // Trop de joueurs enregistrés
        strcpy(pseudos[nb], pseudo);
        niveaux[nb] = niveau;
        viesTab[nb] = vies;
        scores[nb] = score;
        nb++;
    }

    // 4. On réécrit tout le fichier avec les nouvelles données
    f = fopen("sauvegarde.txt", "w");
    if (f == NULL) return 0;

    for (i = 0; i < nb; i++) {
        fprintf(f, "%s %d %d %d\n", pseudos[i], niveaux[i], viesTab[i], scores[i]);
    }
    fclose(f);
    return 1;
}

// Détecte une forme en L ou en T de 5 items
void detecterFigureL_T(t_jeu *jeu, int marque[LIGNES][COLONNES]) {
    int y, x;
    
    // On parcourt la grille (en évitant les bords pour ne pas sortir du tableau)
    for (y = 0; y < LIGNES; y++) {
        for (x = 0; x < COLONNES; x++) {
            int val = jeu->grille[y][x];
            if (val <= 0 || val > NB_TYPES) continue;

            // détection du T
            if (x > 0 && x < COLONNES - 1 && y > 0 && y < LIGNES - 1) {
                if (jeu->grille[y][x-1] == val && jeu->grille[y][x+1] == val && // Horizontal
                    jeu->grille[y-1][x] == val && jeu->grille[y+1][x] == val) { // Vertical
                    
                    // Effet : On marque une zone 3x3 autour du centre (Explosion)
                    int i, j;
                    for(i = -1; i <= 1; i++) {
                        for(j = -1; j <= 1; j++) {
                            marque[y+i][x+j] = 1;
                        }
                    }
                }
            }

            // détection du L 
            if (y >= 2 && x >= 2) {
                if (jeu->grille[y-1][x] == val && jeu->grille[y-2][x] == val && // Vertical haut
                    jeu->grille[y][x-1] == val && jeu->grille[y][x-2] == val) { // Horizontal gauche
                    
                    // explosion sur 3 cases
                     int k;
                     for(k=1; k<=3; k++) {
                         if(y-k >= 0) marque[y-k][x] = 1; // Haut
                         if(x-k >= 0) marque[y][x-k] = 1; // Gauche
                     }
                     marque[y][x] = 1;
                }
            }
            
        }
    }
}
