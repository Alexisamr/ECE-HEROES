#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "projet.h"


// --- FONCTIONS UTILITAIRES LOCALES ---

// Vérifie si tous les objectifs du contrat sont remplis (return 1 si oui, 0 sinon)
int verifierVictoire(t_jeu jeu) {
    for (int i = 0; i < NB_TYPES; i++) {
        if (jeu.objectifs[i] > 0) return 0; // Il reste des choses à détruire
    }
    return 1; // Tout est à 0, c'est gagné
}

// Gère la lecture des flèches ET de ZQSD pour simplifier le main
int recupererDirection(int *dx, int *dy) {
    int touche = getch();
    *dx = 0; *dy = 0;

    // Gestion des flèches directionnelles (Code double : 224 puis code touche)
    if (touche == 0 || touche == 224) {
        touche = getch(); // On lit le deuxième code
        switch(touche) {
            case 72: *dy = -1; return 1; // Haut
            case 80: *dy = 1;  return 1; // Bas
            case 75: *dx = -1; return 1; // Gauche
            case 77: *dx = 1;  return 1; // Droite
        }
    }
    
    // Gestion ZQSD
    if (touche == 'z' || touche == 'Z') { *dy = -1; return 1; }
    if (touche == 's' || touche == 'S') { *dy = 1; return 1; }
    if (touche == 'q' || touche == 'Q') { *dx = -1; return 1; }
    if (touche == 'd' || touche == 'D') { *dx = 1; return 1; }
    
    // Autres touches (Espace, P, etc.)
    return touche; 
}

// Fonction pour rafraîchir tout l'écran proprement
void mettreAJourAffichage(t_jeu jeu, int cX, int cY, int selActive) {
    afficherNiveau(jeu);
    afficherVies(jeu);
    afficherScore(jeu);
    afficherTemps(jeu);
    afficherCoups(jeu);
    afficherContrat(jeu);
    afficherGrille(jeu, cX, cY, selActive);
    afficherAide(); // Affiche les commandes en bas
}

// --- MAIN PRINCIPAL ---

int main() {
    // 1. Initialisation
    srand(time(NULL));
    SetConsoleOutputCP(65001); // UTF-8 pour les accents et symboles
    
    t_jeu jeu;
    char pseudoJoueur[32] = "Joueur"; // Variable locale pour le pseudo
    int application_lancee = 1;
    int choixMenu;

    // Affiche l'écran d'accueil une seule fois au lancement
    ecranAcceuil();

    while (application_lancee) {
        // 2. Menu Principal
        choixMenu = afficherMenu();

        if (choixMenu == 4) { // QUITTER
            application_lancee = 0;
        }
        else if (choixMenu == 1) { // REGLES
            afficherRegles();
        }
        else if (choixMenu == 2 || choixMenu == 3) { // NOUVELLE PARTIE ou CHARGER
            
            // Configuration initiale
            jeu.vies = 3;
            jeu.score = 0;
            jeu.niveau_actuel = 1;
            
            // Si on charge, on écrase les valeurs par celles du fichier
            if (choixMenu == 3) {
                system("cls");
                printf("Entrez votre pseudo pour charger : ");
                scanf("%31s", pseudoJoueur);
                int niv, vi, sc;
                if (chargerSauvegarde(pseudoJoueur, &niv, &vi, &sc)) {
                    jeu.niveau_actuel = niv;
                    jeu.vies = vi;
                    jeu.score = sc;
                    printf("Partie chargee ! Niveau %d", niv);
                    Sleep(1000);
                } else {
                    printf("Sauvegarde introuvable. Nouvelle partie lancee.\n");
                    Sleep(1500);
                }
            } else {
                // Nouvelle partie : on demande le pseudo pour la future sauvegarde
                system("cls");
                printf("Choisissez un pseudo : ");
                scanf("%31s", pseudoJoueur);
            }

            // Boucle des niveaux (Tant qu'on a des vies et qu'on n'a pas fini le niv 3)
            while (jeu.vies > 0 && jeu.niveau_actuel <= 3) {
                
                // --- PRÉPARATION DU NIVEAU ---
                int temps_max, coups_max;
                // On charge les objectifs via la fonction de jeu.c
                chargerParametresNiveau(jeu.niveau_actuel, jeu.objectifs, &coups_max, &temps_max);
                jeu.temps_restant = temps_max;
                jeu.coups_restants = coups_max;

                // Génération de la grille (sans combinaisons déjà faites)
                genererGrilleSansSuite(jeu.grille);

                // Variables du niveau
                time_t debut_niveau = time(NULL);
                int niveau_fini = 0; // 0=non, 1=gagné, 2=perdu, 3=quitter/sauvegarder
                
                int curseurX = 0, curseurY = 0;
                int selectionActive = 0;
                int selectionX = -1, selectionY = -1;
                
                // Tableau pour la détection des marques (match-3)
                int marques[LIGNES][COLONNES];

                // --- BOUCLE DE JEU (FRAME PAR FRAME) ---
                while (niveau_fini == 0) {
                    
                    // A. Gestion du Temps
                    time_t maintenant = time(NULL);
                    int temps_ecoule = (int)difftime(maintenant, debut_niveau);
                    jeu.temps_restant = temps_max - temps_ecoule;

                    // B. Affichage complet
                    mettreAJourAffichage(jeu, curseurX, curseurY, selectionActive);

                    // C. Gestion des Entrées (Input)
                    if (kbhit()) {
                        int dx = 0, dy = 0;
                        int touche = recupererDirection(&dx, &dy);

                        // Si c'est une direction (ZQSD ou Flèches)
                        if (dx != 0 || dy != 0) {
                            if (curseurX + dx >= 0 && curseurX + dx < COLONNES) curseurX += dx;
                            if (curseurY + dy >= 0 && curseurY + dy < LIGNES)   curseurY += dy;
                        }
                        // Sélection (Espace)
                        else if (touche == ' ') {
                            if (selectionActive == 0) {
                                // Première sélection
                                selectionActive = 1;
                                selectionX = curseurX;
                                selectionY = curseurY;
                            } else {
                                // Deuxième appui : Tentative de Permutation
                                // On vérifie si les cases sont voisines (distance de 1)
                                int dist = abs(curseurX - selectionX) + abs(curseurY - selectionY);
                                
                                if (dist == 1) {
                                    // 1. On permute
                                    permuterCases(&jeu, selectionX, selectionY, curseurX, curseurY);
                                    jeu.coups_restants--;
                                    selectionActive = 0;

                                    // 2. Vérification et Animation des chutes (CASCADE)
                                    // C'est ici qu'on gère la gravité en boucle tant qu'il y a des matchs
                                    int nb_matchs;
                                    do {
                                        // On redessine pour voir la permutation
                                        mettreAJourAffichage(jeu, curseurX, curseurY, 0);
                                        Sleep(200); // Petite pause visuelle

                                        // Détection des alignements
                                        nb_matchs = detecterMarques(&jeu, marques);

                                        if (nb_matchs > 0) {
                                            // Suppression et Score
                                            appliquerMarques(&jeu, marques);
                                            mettreAJourAffichage(jeu, curseurX, curseurY, 0);
                                            Sleep(200); // Pause "Explosion"

                                            // Gravité
                                            appliquerGravite(jeu.grille);
                                            mettreAJourAffichage(jeu, curseurX, curseurY, 0);
                                            Sleep(200); // Pause "Chute"

                                            // Remplissage
                                            remplissagecasesvides(jeu.grille);
                                        }
                                        else {
                                            // Si on vient de permuter et qu'il n'y a AUCUN match
                                            // Normalement dans Candy Crush on annule le coup.
                                            // Ici pour simplifier on laisse le coup (pénalité) ou on pourrait re-permuter.
                                        }

                                    } while (nb_matchs > 0); // On recommence tant que ça explose

                                } else {
                                    // Si on clique trop loin ou sur la même case, on annule/déplace la sélection
                                    selectionActive = 0; 
                                }
                            }
                        }
                        // Touche P : Pause / Sauvegarder
                        else if (touche == 'p' || touche == 'P') {
                            niveau_fini = 3; // Code pour sauvegarde
                        }
                    }

                    // D. Vérification Victoire / Défaite
                    if (verifierVictoire(jeu)) {
                        niveau_fini = 1; // Gagné
                    }
                    else if (jeu.temps_restant <= 0 || jeu.coups_restants <= 0) {
                        jeu.temps_restant = 0;
                        niveau_fini = 2; // Perdu
                    }

                    Sleep(50); // Ralentir un peu la boucle
                }

                // --- FIN DU NIVEAU ---
                if (niveau_fini == 1) {
                    // Victoire
                    jeu.niveau_actuel++;
                    if (jeu.niveau_actuel <= 3) {
                        system("cls");
                        printf("\n\n   BRAVO ! NIVEAU SUIVANT...\n");
                        // Proposition de sauvegarde rapide
                        printf("   Appuyez sur 'S' pour sauvegarder et quitter, ou une autre touche pour continuer.");
                        if (getch() == 's' || getch() == 'S') {
                            sauvegarderSauvegarde(pseudoJoueur, jeu.niveau_actuel, jeu.vies, jeu.score);
                            break; // Retour menu
                        }
                    }
                } 
                else if (niveau_fini == 2) {
                    // Défaite
                    jeu.vies--;
                    if (jeu.vies > 0) {
                        afficherMessageViePerdue(jeu.vies); // Fonction de affichage.c
                    } else {
                        afficherEcranDefaite();
                    }
                }
                else if (niveau_fini == 3) {
                    // Sauvegarde manuelle demandée par le joueur
                    sauvegarderSauvegarde(pseudoJoueur, jeu.niveau_actuel, jeu.vies, jeu.score);
                    system("cls");
                    printf("Partie sauvegardee. Retour au menu...\n");
                    Sleep(1000);
                    break; // On sort de la boucle des niveaux pour revenir au menu
                }

            } // Fin boucle niveaux

            if (jeu.niveau_actuel > 3) {
                afficherEcranVictoire();
            }

        } // Fin if choixMenu 2 ou 3
    }
    
    system("cls");
    printf("Au revoir !\n");
    return 0;
}
