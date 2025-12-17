#include "projet.h"

int main() {
    //ça c'est pour l'aléatoire besoin de le mettre une seule fois 
    srand(time(NULL));

    t_jeu maPartie; 
    int choix;
    int fin_du_programme = 0;

    // variables pour le curseur de l'utilisateur 
    int curseurX = 0;
    int curseurY = 0;

    do {
        choix = afficherMenu();

        switch (choix) {
            case 1:
                afficherRegles();
                break;

            case 2: // nouvelle partie
                system("cls"); 

                // on initialise les données
                initialisationplateau(maPartie.grille);
                remplissagecasesvides(maPartie.grille);
                
                //on initialise les stats du jeu
                maPartie.vies = 3;
                maPartie.score = 0;
                maPartie.temps_restant = 60;
                maPartie.niveau_actuel = 1;
                maPartie.coups_restants = 20;
                //exemple d'un objectif
                maPartie.objectifs[1] = 10; // 10 items de type 1 à détruire

                // le coeur du programme la boucle de jeu 
                int niveau_en_cours = 1;
                while (niveau_en_cours) {
                    
                    //affichage
                    afficherTitre();
                    afficherNiveau(maPartie);
                    afficherVies(maPartie);
                    afficherScore(maPartie);
                    afficherTemps(maPartie);
                    afficherCoups(maPartie);
                    afficherContrat(maPartie);
                    
                    // On passe le curseur pour l'affichage
                    afficherGrille(maPartie, curseurX, curseurY, 0); 

                    // B. Gestion des touches (Z,Q,S,D pour bouger)
                    // C'est ici qu'il faudra ajouter la logique de déplacement
                    // Pour l'instant on met une pause pour voir l'affichage
					
                    Gotoxy(0, LIGNES + 10);
                    printf("Appuyez sur 'q' pour quitter le niveau...");
                    char touche = getch(); // Nécessite <conio.h> souvent inclus via windows.h ou à ajouter
                    if (touche == 'q') niveau_en_cours = 0;

                    // logique de jeu (Gravité, destructions...)
                    // appel des fonctions de ruben ...
                    // detectionSuite(maPartie.grille);
                    // gravite(maPartie.grille);
                    // remplissagecasesvides(maPartie.grille);
                }
                break;

            case 3:
                // reprendrePartie();
                printf("Fonctionnalite a venir !\n");
                Sleep(1000);
                break;
                
            case 0:
                fin_du_programme = 1;
                break;
        }
    } while (fin_du_programme == 0);

    return 0;

finPartie{
    
}
