#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void Gotoxy(int x, int y) { // c'est le truc qui permet de faire une fenêtre et de bouger le cruser tout ça grâce à <windows.h>
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void Color(int t, int f) {
    HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H, f * 16 + t);
}

void afficherTitre() {
    Gotoxy(0, 0); //important pour être certain que l'affichage commence vraiment au début
    Color(15, 0); //blanc sur noir
    printf("=========================================================================================\n");
    printf("               PROJET MATCH-3  |  ECE HEROES  |  ING1  2025\n");
    printf("=========================================================================================\n");
}

int afficherMenu() {
	system("cls");
	Color(15, 0);
	int choix;
	printf("************************************\n");
    printf("         ECE HEROES-MENU             \n");
    printf("************************************\n\n");
	printf("1. Afficher les regles du jeu\n");
	printf("2. Nouvelle partie\n");
	printf("3. Reprendre une partie\n");
	printf("Choisissez un mode : ");
	scanf("%d", &choix);
	while(choix < 0 || choix > 4){
		printf("Choissisez un menu entre 1 et 3 :");
		scanf("%d", &choix);
	}
	printf("%d", choix);
	return choix;
}

void afficherRegles() {
    system("cls");
    afficherTitre();
    printf("\n --- LES REGLES --- \n\n"); // à finir faut que bien que je vérifie les regles mais je pense c'est ça 
    printf("BUT DU JEU : ");
	printf("- Alignez 3 symboles identiques pour les détruire et marquer des points"); //le but du jeu, les contraintes, les figures spéciales, les contrôles
    printf("- Remplissez le CONTRAT avant la fin du temps imparti !\n\n");
	
    printf("COMMANDES : ");
    printf("- Deplacement : [Z] Haut, [S] Bas, [Q] Gauche, [D] Droite \n");
	printf("- [ESPACE] pour selectionner et echanger deux cases. \n\n");
	
    printf("BONUS & DEFAITE :\n"); 
    printf("- Alignez 4 ou 5 items pour créer des EXPLOSIONS !\n");

	printf("  Appuyer sur une touche pour revenir au menu ...");
    getchar(); getchar(); // ça c'est hyper important en gros ça nétoie les petites merde ça purge pour les futures entrée de l'utilisateur (merci gemini 3 pro max ultra HD 4K) 
}

void afficherTemps(t_jeu jeu ){ //donc la jvais utiliser gotoxy pour cibler l'endroit ou le temps sera afficher et changé uniquement cette endroit
//ça sera pas les valeurs finales dans les gotoxy() juste pour qu'on capte 
    Gotoxy(60, 1);
    Color(14, 0); //encore jaune la couleur du pipi
    printf("TEMPS: %d s  ", jeu.temps_restant);
    Color(15,0);
}

void afficherVies(t_jeu jeu){ //t_jeu jeu c'est des listes qui contiennent avec les struct (ruben bande) le score la vie et le temps
//faudra bien tout relier avec le main et le headers
    Gotoxy(35, 0); //rouge
    Color(12, 0);
    printf("VIES: %d", jeu.vies);
    Color(15, 0);
}

void afficherScore(t_jeu jeu){
    Gotoxy(45, 0); 
    Color(11,0); // cyan
    printf("| SCORE: %05d", jeu.score); // 05 ça met des zéro devant pour faire un score styléaisea_iazruhaze
    Color(15, 0); // et hop on remet blanco
}

void afficherGrille(t_jeu jeu, int curseurX, int curseurY, int selectionActive) {
    // grille soit centrée et pas collée au bord
    int x_depart = 15; 
    int y_depart = 5;  
    
    int i, j;
    int val_case;
    int couleur_fond;
    int couleur_texte;
    char symbole;

    // --- DESSIN CADRE HAUT ---
    Color(15, 0); // Blanc 
    Gotoxy(x_depart - 1, y_depart - 1);
    printf("%c", 201); // Coin double ╔
    for(j=0; j<COLONNES; j++) printf("%c", 205); // Barre double ═
    printf("%c", 187); // Coin double ╗

    // --- PARCOURS GRILLE ---
    for (i = 0; i < LIGNES; i++) {
        // Mur de Gauche
        Gotoxy(x_depart - 1, y_depart + i);
        Color(15, 0); 
        printf("%c", 186); // Barre verticale double ║

        for (j = 0; j < COLONNES; j++) {
            // On récupère la valeur dans la mémoire du jeu
            val_case = jeu.grille[i][j];
            
            // Gestion du curseur
            couleur_fond = 0; // Noir 
            // Si on est sur la case du curseur (X,Y)
            if (j == curseurX && i == curseurY) {
                if (selectionActive == 1) {
                    couleur_fond = 13; // Fond Violet
                } else {
                    couleur_fond = 8;  // Fond Gris
                }
            }

            // Gestion du symbole
            switch(val_case) {
                case 0: couleur_texte = 0;  symbole = ' '; break;
                case 1: couleur_texte = 12; symbole = 'O'; break;
                case 2: couleur_texte = 10; symbole = '&'; break;
                case 3: couleur_texte = 14; symbole = '#'; break;
                case 4: couleur_texte = 9;  symbole = '@'; break;
                case 5: couleur_texte = 13; symbole = '$'; break;
                default: couleur_texte = 15; symbole = '?'; break;
            }
            
            // Affichage de la case
            Gotoxy(x_depart + j, y_depart + i); 
            Color(couleur_texte, couleur_fond); 
            printf("%c", symbole);
        }
        
        // Mur de droite (Position Forcée)
        Gotoxy(x_depart + COLONNES, y_depart + i);
        Color(15, 0);
        printf("%c", 186); // Barre verticale double ║
    }

    // --- DESSIN CADRE BAS ---
    Gotoxy(x_depart - 1, y_depart + LIGNES);
    printf("%c", 200); // Coin double ╚
    for(j=0; j<COLONNES; j++) printf("%c", 205); // Barre double ═
    printf("%c", 188); // Coin double ╝

    // Reset couleur
    Color(15, 0);
}


void afficherContrat(t_jeu jeu) {
    // On place le contrat à Droite de la grille (x=65) pour ne pas gêner
    int x_depart = 65;
    int y_depart = 5;

    // Tableaux pour retrouver les mêmes couleurs/symboles que la grille
    // Index 0 inutile, on utilise 1 à 5
    char symboles[] = {' ', 'O', '&', '#', '@', '$'}; 
    int couleurs[] = {0, 12, 10, 14, 9, 13};

    Gotoxy(x_depart, y_depart);
    Color(15, 0); // Blanc
    printf("--- CONTRAT ---");

    int i;
    int ligne_actuelle = 0;

    // On parcourt les 5 types d'items
    for(i = 1; i <= 5; i++) {
        
        // On affiche seulement s'il reste des items de ce type à détruire
        if (jeu.objectifs[i] > 0) {
            ligne_actuelle++;
            Gotoxy(x_depart, y_depart + (ligne_actuelle * 2)); // Espacé de 2 lignes

            // 1. Le Symbole en couleur
            Color(couleurs[i], 0); 
            printf("[%c]", symboles[i]);

            // 2. Le nombre restant en blanc
            Color(15, 0);
            // Le "   " à la fin sert à effacer les vieux chiffres (si on passe de 10 à 9)
            printf(" : x %d a eliminer   ", jeu.objectifs[i]);
        }
        else {
            // Optionnel : Si l'objectif est fini (0), on affiche "OK" en vert
            // Tu peux supprimer ce 'else' si tu préfères que la ligne disparaisse
            ligne_actuelle++;
            Gotoxy(x_depart, y_depart + (ligne_actuelle * 2));
            Color(10, 0); // Vert
            printf("TYPE %d : TERMINE !      ", i);
        }
    }
    
    Color(15, 0); // Reset blanc
}


void afficherCoups(t_jeu jeu) { // Si plus de coups ou pas le temps fin de la partie gros nul
    Gotoxy(75, 1);
    Color(13, 0);
    printf("COUPS : %d", jeu.coups_restants);
}

void afficherNiveau(t_jeu jeu) { //pour savoir à quelle niveau en est l'utilisateur 
    Gotoxy(0,0);
    Color(14, 0);
    printf("NIVEAU : %d", jeu.niveau_actuel);
}
    //Vicoitre Contrat rempli 
    //Défaite Temps écoulé 
    //Défaite Aucun coups restant 
    //Défaite Plus de vies
	// Affiche un message de fin selon si on a gagné ou perdu
void afficherEcranFin(int resultat) {
    system("cls"); 
    Gotoxy(20, 10);
    printf("========================================");

    Gotoxy(35, 12); // le millieu du cadre 

    if (resultat == 1) { // SI Victoire
        Color(10, 0); // Vert fluo
        printf("BRAVO ! NIVEAU REUSSI !");
    } 
    else { // SI DEFAITE 
        Color(12, 0); // Rouge 
        printf("GAME OVER...");
        
        Gotoxy(30, 16);
        Color(15, 0);
        if (resultat == -1) printf("Raison : Temps ecoule !");
        if (resultat == -2) printf("Raison : Plus de coups !");
        if (resultat == -3) printf("Raison : Plus de vies !");
    }

    Gotoxy(25, 20);
    Color(15, 0);
    printf("Appuyez sur une touche pour quitter...");
    getchar(); getchar(); 
}
}
