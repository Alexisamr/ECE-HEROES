#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "projet.h"

static int typeAleatoire() {
    return (rand() % NB_TYPES) + 1;
}

static int creeSuite3(int grille[LIGNES][COLONNES], int x, int y, int valeur) {
    if (valeur <= 0) return 0;

    if (x >= 2) {
        if (grille[y][x - 1] == valeur && grille[y][x - 2] == valeur) return 1;
    }
    if (y >= 2) {
        if (grille[y - 1][x] == valeur && grille[y - 2][x] == valeur) return 1;
    }
    return 0;
}

void initialisationplateau(int grille[LIGNES][COLONNES]) {
    int i, j;
    for (i = 0; i < LIGNES; i++) {
        for (j = 0; j < COLONNES; j++) {
            grille[i][j] = TYPE_VIDE;
        }
    }
}

void genererGrilleSansSuite(int grille[LIGNES][COLONNES]) {
    int x, y;
    initialisationplateau(grille);

    for (y = 0; y < LIGNES; y++) {
        for (x = 0; x < COLONNES; x++) {
            int valeur = typeAleatoire();
            while (creeSuite3(grille, x, y, valeur)) {
                valeur = typeAleatoire();
            }
            grille[y][x] = valeur;
        }
    }
}

void appliquerGravite(int grille[LIGNES][COLONNES]) {
    int colonne;
    for (colonne = 0; colonne < COLONNES; colonne++) {
        int ecriture = LIGNES - 1;
        int lecture;

        for (lecture = LIGNES - 1; lecture >= 0; lecture--) {
            if (grille[lecture][colonne] != TYPE_VIDE) {
                grille[ecriture][colonne] = grille[lecture][colonne];
                if (ecriture != lecture) grille[lecture][colonne] = TYPE_VIDE;
                ecriture--;
            }
        }

        while (ecriture >= 0) {
            grille[ecriture][colonne] = TYPE_VIDE;
            ecriture--;
        }
    }
}

void remplissagecasesvides(int grille[LIGNES][COLONNES]) {
    int x, y;
    for (y = 0; y < LIGNES; y++) {
        for (x = 0; x < COLONNES; x++) {
            if (grille[y][x] == TYPE_VIDE) {
                grille[y][x] = typeAleatoire();
            }
        }
    }
}

void chargerParametresNiveau(int niveau, int objectifs[NB_TYPES], int *coups_max, int *temps_max) {
    int i;
    for (i = 0; i < NB_TYPES; i++) objectifs[i] = 0;

    if (niveau == 1) {
        objectifs[0] = 35;
        objectifs[1] = 35;
        objectifs[2] = 35;
        *coups_max = 35;
        *temps_max = 90;
        return;
    }

    if (niveau == 2) {
        objectifs[0] = 55;
        objectifs[1] = 45;
        objectifs[3] = 40;
        *coups_max = 40;
        *temps_max = 90;
        return;
    }

    objectifs[1] = 70;
    objectifs[2] = 60;
    objectifs[3] = 55;
    objectifs[4] = 40;
    *coups_max = 45;
    *temps_max = 90;
}

int permuterCases(t_jeu *jeu, int x1, int y1, int x2, int y2) {
    int tmp;
    if (x1 < 0 || x1 >= COLONNES || y1 < 0 || y1 >= LIGNES) return 0;
    if (x2 < 0 || x2 >= COLONNES || y2 < 0 || y2 >= LIGNES) return 0;

    tmp = jeu->grille[y1][x1];
    jeu->grille[y1][x1] = jeu->grille[y2][x2];
    jeu->grille[y2][x2] = tmp;
    return 1;
}

int detecterMarques(t_jeu *jeu, int marque[LIGNES][COLONNES]) {
    int supprimerType[NB_TYPES + 1];
    int y, x, i;
    int nb = 0;

    for (y = 0; y < LIGNES; y++) {
        for (x = 0; x < COLONNES; x++) marque[y][x] = 0;
    }
    for (i = 0; i <= NB_TYPES; i++) supprimerType[i] = 0;

    for (y = 0; y < LIGNES; y++) {
        x = 0;
        while (x < COLONNES) {
            int val = jeu->grille[y][x];
            int debut = x;
            int longueur = 0;

            if (val < 1 || val > NB_TYPES) { x++; continue; }

            while (x < COLONNES && jeu->grille[y][x] == val) {
                longueur++;
                x++;
            }

            if (longueur >= 6) supprimerType[val] = 1;
            if (longueur >= 3 && longueur < 6) {
                int k;
                for (k = debut; k < debut + longueur; k++) marque[y][k] = 1;
            }
        }
    }

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

    for (y = 0; y <= LIGNES - 4; y++) {
        for (x = 0; x <= COLONNES - 4; x++) {
            int val = jeu->grille[y][x];
            int yy, xx;
            int ok = 1;

            if (val < 1 || val > NB_TYPES) continue;

            for (yy = 0; yy < 4; yy++) {
                for (xx = 0; xx < 4; xx++) {
                    if (jeu->grille[y + yy][x + xx] != val) ok = 0;
                }
            }

            if (ok) {
                for (yy = 0; yy < 4; yy++) {
                    for (xx = 0; xx < 4; xx++) marque[y + yy][x + xx] = 1;
                }
            }
        }
    }

    for (y = 2; y <= LIGNES - 3; y++) {
        for (x = 2; x <= COLONNES - 3; x++) {
            int val = jeu->grille[y][x];
            if (val < 1 || val > NB_TYPES) continue;

            if (jeu->grille[y - 1][x] == val && jeu->grille[y - 2][x] == val &&
                jeu->grille[y + 1][x] == val && jeu->grille[y + 2][x] == val &&
                jeu->grille[y][x - 1] == val && jeu->grille[y][x - 2] == val &&
                jeu->grille[y][x + 1] == val && jeu->grille[y][x + 2] == val) {

                int k;
                for (k = 0; k < COLONNES; k++) if (jeu->grille[y][k] == val) marque[y][k] = 1;
                for (k = 0; k < LIGNES; k++) if (jeu->grille[k][x] == val) marque[k][x] = 1;
            }
        }
    }

    for (i = 1; i <= NB_TYPES; i++) {
        if (supprimerType[i]) {
            for (y = 0; y < LIGNES; y++) {
                for (x = 0; x < COLONNES; x++) {
                    if (jeu->grille[y][x] == i) marque[y][x] = 1;
                }
            }
        }
    }

    for (y = 0; y < LIGNES; y++) {
        for (x = 0; x < COLONNES; x++) {
            if (marque[y][x]) nb++;
        }
    }

    return nb;
}

int appliquerMarques(t_jeu *jeu, int marque[LIGNES][COLONNES]) {
    int y, x;
    int nb = 0;

    for (y = 0; y < LIGNES; y++) {
        for (x = 0; x < COLONNES; x++) {
            if (marque[y][x]) {
                int val = jeu->grille[y][x];
                if (val >= 1 && val <= NB_TYPES) {
                    if (jeu->objectifs[val - 1] > 0) jeu->objectifs[val - 1]--;
                    jeu->score += 10;
                }
                jeu->grille[y][x] = TYPE_VIDE;
                nb++;
            }
        }
    }
    return nb;
}

int pseudoExiste(const char *pseudo) {
    FILE *f = fopen("sauvegarde.txt", "r");
    char pseudoLu[32];
    int niveauLu, viesLu, scoreLu;

    if (f == NULL) return 0;

    while (fscanf(f, "%31s %d %d %d", pseudoLu, &niveauLu, &viesLu, &scoreLu) == 4) {
        if (strcmp(pseudoLu, pseudo) == 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int chargerSauvegarde(const char *pseudo, int *niveau, int *vies, int *score) {
    FILE *f = fopen("sauvegarde.txt", "r");
    char pseudoLu[32];
    int niveauLu, viesLu, scoreLu;

    if (f == NULL) return 0;

    while (fscanf(f, "%31s %d %d %d", pseudoLu, &niveauLu, &viesLu, &scoreLu) == 4) {
        if (strcmp(pseudoLu, pseudo) == 0) {
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

int sauvegarderSauvegarde(const char *pseudo, int niveau, int vies, int score) {
    char pseudos[200][32];
    int niveaux[200];
    int viesTab[200];
    int scores[200];
    int nb = 0;
    int trouve = 0;
    int i;

    FILE *f = fopen("sauvegarde.txt", "r");
    if (f != NULL) {
        while (nb < 200 && fscanf(f, "%31s %d %d %d", pseudos[nb], &niveaux[nb], &viesTab[nb], &scores[nb]) == 4) {
            nb++;
        }
        fclose(f);
    }

    for (i = 0; i < nb; i++) {
        if (strcmp(pseudos[i], pseudo) == 0) {
            niveaux[i] = niveau;
            viesTab[i] = vies;
            scores[i] = score;
            trouve = 1;
        }
    }

    if (!trouve) {
        if (nb >= 200) return 0;
        strcpy(pseudos[nb], pseudo);
        niveaux[nb] = niveau;
        viesTab[nb] = vies;
        scores[nb] = score;
        nb++;
    }

    f = fopen("sauvegarde.txt", "w");
    if (f == NULL) return 0;

    for (i = 0; i < nb; i++) {
        fprintf(f, "%s %d %d %d\n", pseudos[i], niveaux[i], viesTab[i], scores[i]);
    }
    fclose(f);
    return 1;
}

// ==================== STRUCTURES ====================

typedef struct {
    int type;        // Type d'objectif (ex: 1=détruire X bonbons rouges, 2=atteindre X points, etc.)
    int target;      // Nombre à atteindre pour compléter l'objectif
    int current;     // Progression actuelle
} Objective;

typedef struct {
    int number;           // Numéro du niveau
    int moves_limit;      // Nombre de coups maximum
    int moves_used;       // Coups utilisés
    Objective objective;  // Objectif à compléter
    int completed;        // 1 si complété, 0 sinon
} Level;

typedef struct {
    int lives;           // Nombre de vies actuelles
    int max_lives;       // Nombre maximum de vies
    int current_level;   // Niveau actuel du joueur
} Player;

// ==================== VARIABLES GLOBALES ====================

Player player;
Level levels[100];  // Tableau pour stocker jusqu'à 100 niveaux
int total_levels = 0;

// ==================== FONCTIONS DE BASE ====================

// Initialiser le joueur
void init_player(int starting_lives, int max_lives) {
    player.lives = starting_lives;
    player.max_lives = max_lives;
    player.current_level = 1;
}

// Créer un nouveau niveau
void create_level(int level_num, int moves, int obj_type, int obj_target) {
    if (total_levels >= 100) {
        printf("Erreur: Nombre maximum de niveaux atteint!\n");
        return;
    }
    
    levels[total_levels].number = level_num;
    levels[total_levels].moves_limit = moves;
    levels[total_levels].moves_used = 0;
    levels[total_levels].objective.type = obj_type;
    levels[total_levels].objective.target = obj_target;
    levels[total_levels].objective.current = 0;
    levels[total_levels].completed = 0;
    
    total_levels++;
}

// ==================== GESTION DES VIES ====================

// Perdre une vie
int lose_life() {
    if (player.lives > 0) {
        player.lives--;
        printf("Vie perdue! Vies restantes: %d\n", player.lives);
        return 1;
    }
    printf("Game Over! Plus de vies!\n");
    return 0;
}

// Gagner une vie
void gain_life() {
    if (player.lives < player.max_lives) {
        player.lives++;
        printf("Vie gagnée! Vies: %d\n", player.lives);
    } else {
        printf("Vies déjà au maximum!\n");
    }
}

// Vérifier si le joueur a des vies
int has_lives() {
    return player.lives > 0;
}

// ==================== GESTION DES NIVEAUX ====================

// Obtenir le niveau actuel
Level* get_current_level() {
    for (int i = 0; i < total_levels; i++) {
        if (levels[i].number == player.current_level) {
            return &levels[i];
        }
    }
    return NULL;
}

// Incrémenter un coup
void use_move() {
    Level* lvl = get_current_level();
    if (lvl != NULL) {
        lvl->moves_used++;
    }
}

// Mettre à jour la progression de l'objectif
void update_objective(int progress) {
    Level* lvl = get_current_level();
    if (lvl != NULL) {
        lvl->objective.current += progress;
        printf("Progression: %d/%d\n", lvl->objective.current, lvl->objective.target);
    }
}

// Vérifier si l'objectif est atteint
int is_objective_completed() {
    Level* lvl = get_current_level();
    if (lvl != NULL) {
        return lvl->objective.current >= lvl->objective.target;
    }
    return 0;
}

// Vérifier si le joueur a dépassé la limite de coups
int is_out_of_moves() {
    Level* lvl = get_current_level();
    if (lvl != NULL) {
        return lvl->moves_used >= lvl->moves_limit;
    }
    return 0;
}

// Terminer le niveau (appelé à la fin d'une partie)
int end_level() {
    Level* lvl = get_current_level();
    if (lvl == NULL) return 0;
    
    // Vérifier si l'objectif est complété
    if (is_objective_completed()) {
        lvl->completed = 1;
        printf("\n=== NIVEAU %d COMPLÉTÉ! ===\n", lvl->number);
        player.current_level++;
        return 1;  // Niveau réussi
    }
    
    // Objectif non complété = échec
    printf("\n=== NIVEAU %d ÉCHOUÉ! ===\n", lvl->number);
    lose_life();
    
    // Réinitialiser le niveau pour réessayer
    lvl->moves_used = 0;
    lvl->objective.current = 0;
    
    return 0;  // Niveau échoué
}
