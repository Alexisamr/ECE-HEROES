#include "projet.h"
#include <time.h>
#include <conio.h> // pour getch() et kbhit()

int main() {
    srand(time(NULL));         // pour l'aléatoire
    SetConsoleOutputCP(65001); // pour les caractères spéciaux
    ecranAcceuil(); 

    t_jeu jeu; 
    int choixMenu;
    int application_lancee = 1;

    // variables pour lees entrée clavier
    int curseurX = 0, curseurY = 0, selectionActive = 0;
    char touche;

    while (application_lancee) {
        choixMenu = afficherMenu(); 

        switch (choixMenu) {
            case 1: // REGLES
                afficherRegles();
                break;

            case 2: 
                // initialisation de la partie 
                jeu.vies = 3;
                jeu.score = 0;
                jeu.niveau_actuel = 1; // on commence niveau 1 (faire les différents niveau quand tout sera terminé)
                
                int partie_terminee = 0;
				
                // tant que le joueur a une vie le jeu continue
                while (jeu.vies > 0 && partie_terminee == 0) {
                    
                    // initialisation du jeu
                    initialisationplateau(jeu.grille);   
                    remplissagecasesvides(jeu.grille);   
                    
                    jeu.temps_restant = 60; 
                    jeu.coups_restants = 20; 

					//départ du chrono
                    time_t debut_niveau = time(NULL);
    				// reset des positions
                    curseurX = 0; curseurY = 0; selectionActive = 0;

                    int niveau_gagne = 0;
                    int niveau_perdu = 0;

                    // boucle du jeu
                    while (niveau_gagne == 0 && niveau_perdu == 0) {
                        
                        // affichage
                        afficherNiveau(jeu);
                        afficherVies(jeu);
                        afficherScore(jeu);
                        afficherTemps(jeu);
                        afficherCoups(jeu);
                        afficherContrat(jeu); 
                        afficherGrille(jeu, curseurX, curseurY, selectionActive);

                        // ZQSD
                        if (kbhit()) {
                            touche = getch();
                            if (touche == 'z' && curseurY > 0) curseurY--;
                            if (touche == 's' && curseurY < LIGNES - 1) curseurY++;
                            if (touche == 'q' && curseurX > 0) curseurX--;
                            if (touche == 'd' && curseurX < COLONNES - 1) curseurX++;
                            
                            // Sélection
                            if (touche == ' ') {
                                if (selectionActive == 0) selectionActive = 1;
                                else {
                                    // a faire : mettre la fonction de permutation ici
                                    jeu.coups_restants--; 
                                    selectionActive = 0;
                                }
                            }
                            
                            // abondon si touche 'p'
                            if (touche == 'p') niveau_perdu = 1;
                        }

						
                        // la faut mettre tes fonctions ruben mais avant vérifier leurs bon fontctionnement 
                        // detectionSuite(...);
                        // gravite(...)

						
						
                        Sleep(50); // ralentir la boucle
						
						int secondes_ecoulees = (int)difftime(maintenant, debut_niveau);
                        jeu.temps_restant = temps_max_niveau - secondes_ecoulees;

                        }
                        // Vérif Défaite Temps
                        if (jeu.temps_restant <= 0) {
                            jeu.temps_restant = 0; // Pour l'affichage propre
                            niveau_perdu = 1;
                        }
						
                        // Vérif Défaite Coups
                        if (jeu.coups_restants <= 0) niveau_perdu = 1;
						//Vérif victoire ...
                        // conditions de fin de niveau (Exemple simple)
                        
                    }

                    // transition entre niveau
                    if (niveau_perdu) {
                        jeu.vies--;
                        if (jeu.vies == 0) {
                            afficherEcranDefaite(); // C'est PERDU pour de bon
                            partie_terminee = 1; // On sort de la boucle de jeu
                        } else {
                            afficherMessageViePerdue(jeu.vies);
                            // Pas besoin de 'partie_terminee = '1', ça va relancer le while avec vie - 1
                        }
                    } 
                    else if (niveau_gagne) {
                        jeu.niveau_actuel++;
                        if (jeu.niveau_actuel > 3) {
                            afficherEcranVictoire(); 
                            partie_terminee = 1; // on sort
                        } else {
							system("cls");
							printf("NIVEAU SUIVANT ...\n"); // a faire : un ascii art pour le passage au niveau suivant
							Sleep(1000)
						}
                    }
                }
                break;

            case 3: // CHARGERRRRR
                printf("Fonctionnalite a venir !\n");
                Sleep(1000);
                break;
            
            case 4: // QUITTER
                application_lancee = 0;
                break;
        }
    }
    
    // nettoyage finale (peut être utiliser une fonction pour faire l'affichage de fin stylé 
    system("cls");
    printf("Au revoir !\n");
    return 0;
}
