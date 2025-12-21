#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "projet.h"

// Vérifie si tous les objectifs du contrat sont remplis 
int verifierVictoire(t_jeu jeu) {
    for (int i = 0; i < NB_TYPES; i++) {
        if (jeu.objectifs[i] > 0) return 0; // Il reste des choses à détruire
    }
    return 1; // Tout est à 0, c'est gagné
}

// Gère la lecture des flèches / ZQSD
int recupererDirection(int *dx, int *dy) {
    int touche = getch();
    *dx = 0; *dy = 0;

    // Gestion des flèches directionnelles
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
    
    // Autres touches 
    return touche; 
}

// Fonction pour rafraichir
void mettreAJourAffichage(t_jeu jeu, int cX, int cY, int selActive) {
    afficherNiveau(jeu);
    afficherVies(jeu);
    afficherScore(jeu);
    afficherTemps(jeu);
    afficherCoups(jeu);
    afficherContrat(jeu);
    afficherGrilleGraphique(jeu, cX, cY, selActive); 
    afficherAide(); // Affiche les commandes
}

//main

int main() {
    //Initialisation
    srand(time(NULL));
    SetConsoleOutputCP(65001); // UTF-8 pour accents et symboles
    system("mode con lines=40 cols=120");
    cacherCurseur();
    t_jeu jeu;
    char pseudoJoueur[32] = "Joueur"; // Variable locale pour pseudo
    int application_lancee = 1;
    int choixMenu;

    // Affiche ecran d'accueil une seule fois au lancement
    ecranAcceuil();

    while (application_lancee) {
        // Menu Principal
        choixMenu = afficherMenu();

        if (choixMenu == 4) { // quitte
            application_lancee = 0;
        }
        else if (choixMenu == 1) { // regles
            afficherRegles();
            choixMenu = afficherMenu();
            
        }
        else if (choixMenu == 2 || choixMenu == 3) { // nouvelle partie / charge
            
            // Configuration de base
            jeu.vies = 3;
            jeu.score = 0;
            jeu.niveau_actuel = 1;
            
            // Si on charge on ecrase les valeurs par celles du fichier
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
                //  on demande le pseudo pour la future sauvegarde
                system("cls");
                printf("Choisissez un pseudo : ");
                scanf("%31s", pseudoJoueur);
            }

            // boucle niveau
            while (jeu.vies > 0 && jeu.niveau_actuel <= 3) {
                
                // prepa niveaux
                int temps_max, coups_max;
                // chargment objectifs
                chargerParametresNiveau(jeu.niveau_actuel, jeu.objectifs, &coups_max, &temps_max);
                jeu.temps_restant = temps_max;
                jeu.coups_restants = coups_max;

                // Génération de la grille 
                genererGrilleSansSuite(jeu.grille);

                // Variables du niveau
                time_t debut_niveau = time(NULL);
                int niveau_fini = 0; // 0=non, 1=gagné, 2=perdu, 3=quitter/sauvegarder
                
                int curseurX = 0, curseurY = 0;
                int selectionActive = 0;
                int selectionX = -1, selectionY = -1;
                int marques[LIGNES][COLONNES];

                system("cls"); // On nettoie ecran
                int doit_rafraichir = 1; // rafraichit
                int dernier_temps_affiche = -1;

                while (niveau_fini == 0) {
                // boucle
                while (niveau_fini == 0) {
                    
                    // Gestion temps
                    time_t maintenant = time(NULL);
                    int temps_ecoule = (int)difftime(maintenant, debut_niveau);
                    jeu.temps_restant = temps_max - temps_ecoule;

                    // Affichage complet
                    mettreAJourAffichage(jeu, curseurX, curseurY, selectionActive);

                    // Imput
                    if (kbhit()) {
                        int dx = 0, dy = 0;
                        int touche = recupererDirection(&dx, &dy);

                        // direction
                        if (dx != 0 || dy != 0) {
                            if (curseurX + dx >= 0 && curseurX + dx < COLONNES) curseurX += dx;
                            if (curseurY + dy >= 0 && curseurY + dy < LIGNES)   curseurY += dy;
                        }
                        // Sélection 
                        else if (touche == ' ') {
                            if (selectionActive == 0) {
                                // 1 selection
                                selectionActive = 1;
                                selectionX = curseurX;
                                selectionY = curseurY;
                            } else {
                                // Permutation
                                // case vosiines
                                int dist = abs(curseurX - selectionX) + abs(curseurY - selectionY);
                                
                                if (dist == 1) {
                                    // permutation
                                    permuterCases(&jeu, selectionX, selectionY, curseurX, curseurY);
                                    jeu.coups_restants--;
                                    selectionActive = 0;

                                    int nb_matchs;
                                    do {
                                        // On redessine pour voir la permutation
                                        mettreAJourAffichage(jeu, curseurX, curseurY, 0);
                                        Sleep(200); 

                                        // Détection alignements
                                        nb_matchs = detecterMarques(&jeu, marques);

                                        if (nb_matchs > 0) {
                                            // Suppression et Score
                                            appliquerMarques(&jeu, marques);
                                            mettreAJourAffichage(jeu, curseurX, curseurY, 0);
                                            Sleep(200); 

                                            // Gravite
                                            appliquerGravite(jeu.grille);
                                            mettreAJourAffichage(jeu, curseurX, curseurY, 0);
                                            Sleep(200); 

                                            // Remplissage
                                            remplissagecasesvides(jeu.grille);
                                        }
                                        else {
                                        }

                                    } while (nb_matchs > 0); // recommence

                                } else {
                                    // Si on clique trop loin
                                    selectionActive = 0; 
                                }
                            }
                        }
                        if (touche == 27) { //echap
                            system("cls");
                            printf("Sortie\n");
                            exit(0); // Ferme 
                        }

                        // sauvegarde
                        else if (touche == 'p' || touche == 'P') {
                            niveau_fini = 3; 
                        }
                    }

                    // Gain Perte
                    if (verifierVictoire(jeu)) {
                        niveau_fini = 1; // Gagne
                    }
                    else if (jeu.temps_restant <= 0 || jeu.coups_restants <= 0) {
                        jeu.temps_restant = 0;
                        niveau_fini = 2; // Perdu
                    }

                    Sleep(50); //Sinon c'est trop rapide
                }

                if (niveau_fini == 1) {
                    // Victoire
                    jeu.niveau_actuel++;
                    if (jeu.niveau_actuel <= 3) {
                        system("cls");
                        printf("\n\n   BRAVO ! NIVEAU SUIVANT...\n");
                        // sauvegarde
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
                    // Sauvegarde 
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

        } 
    }
    
    system("cls");
    printf("Au revoir !\n");
    return 0;
    }
}
