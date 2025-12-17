#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "projet.h"


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

int afficherMenu() {
	system("cls");
	Color(15, 0);
	int choix;
	printf("██▄  ▄██ ██████ ███  ██ ██  ██ ");
	printf("██ ▀▀ ██ ██▄▄   ██ ▀▄██ ██  ██ ");
	printf("██    ██ ██▄▄▄▄ ██   ██ ▀████▀ \n\n");
	
	printf("1. Afficher les regles du jeu\n");
	printf("2. Nouvelle partie\n");
	printf("3. Reprendre une partie\n");
	printf("Choisissez un mode : ");
	scanf("%d", &choix);
	while(choix < 1 || choix > 3){
		printf("Choissisez un menu entre 1 et 3 :");
		scanf("%d", &choix);
	}
	printf("%d", choix);
	return choix;
}

void afficherRegles() {
    system("cls");
    afficherTitre();
	printf("██     ██████ ▄█████   █████▄  ██████  ▄████  ██     ██████ ▄█████");
	printf("██     ██▄▄   ▀▀▀▄▄▄   ██▄▄██▄ ██▄▄   ██  ▄▄▄ ██     ██▄▄   ▀▀▀▄▄▄");
	printf("██████ ██▄▄▄▄ █████▀   ██   ██ ██▄▄▄▄  ▀███▀  ██████ ██▄▄▄▄ █████▀\n\n");

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
                case 0: couleur_texte = 0;  symbole = '0'; break;
                case 1: couleur_texte = 12; symbole = '1'; break;
                case 2: couleur_texte = 10; symbole = '2'; break;
                case 3: couleur_texte = 14; symbole = '3'; break;
                case 4: couleur_texte = 9;  symbole = '4'; break;
                case 5: couleur_texte = 13; symbole = '5'; break;
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

//ASCII ART
//ASCII ART


void ecranAcceuil() {   
	system("cls");
	int x_depart = 10;
    int y = 5;
    int espacement = 8; //pour les espacement entre chaque lettre

	int x = x_depart;
	Color(12, 0), //rouge rouge      
	Gotoxy(x, y); printf("  ██████");
	Gotoxy(x, y+1); printf("██▄▄  ");
	Gotoxy(x, y+2); printf("██▄▄▄▄");
	x += espacement;
	Color(4, 0), //rouge bresson
	Gotoxy(x, y); printf("  ▄█████");
	Gotoxy(x, y+1); printf("██    ");
	Gotoxy(x, y+2); printf("▀█████");
	x += espacement;
	Color(6, 0), //orange
	Gotoxy(x, y); printf("  ██████");
	Gotoxy(x, y+1); printf("██▄▄  ");
	Gotoxy(x, y+2); printf("██▄▄▄▄");
	x += 4; //espace
	
	x += espacement;
	Color(14, 0), //jaune jaune       
	Gotoxy(x, y); printf("  ██  ██");
	Gotoxy(x, y+1); printf("██████");
	Gotoxy(x, y+2); printf("██  ██");
	x += espacement;
	Color(10, 0), //vert vert
	Gotoxy(x, y); printf("  ██████");
	Gotoxy(x, y+1); printf("██▄▄  ");
	Gotoxy(x, y+2); printf("██▄▄▄▄");
	x += espacement;
	Color(2, 0), //vert bresson   
	Gotoxy(x, y); printf("  █████▄ ");
	Gotoxy(x, y+1); printf("██▄▄██▄");
	Gotoxy(x, y+2); printf("██   ██");
	x += espacement;
	Color(11, 0), //cyan       
	Gotoxy(x, y); printf("  ▄████▄");
	Gotoxy(x, y+1); printf("██  ██");
	Gotoxy(x, y+2); printf("▀████▀");
	x += espacement;
	Color(9, 0), //bleu bien bleu
	Gotoxy(x, y); printf("  ██████");
	Gotoxy(x, y+1); printf("██▄▄  ");
	Gotoxy(x, y+2); printf("██▄▄▄");
	x += espacement;
	Color(13, 0), //violet
  	Gotoxy(x, y); printf("  ▄█████");
	Gotoxy(x, y+1); printf("▀▀▀▄▄▄");
	Gotoxy(x, y+2); printf("█████▀");

	// sous titre 
	y += 5
	Color(15, 0);
	Gotoxy(20, y);
	printf("PROJET ECE HEROES - ING1 par ALEXIS, RUBEN, ROMEO, NICOLAS");

	int visible = 1;
	Color(14,0);
	//kbhit = keyboardhit donc !kbhit c'est l'inverse (donc tant qu'aucune touche du clavier n'est pressé)
	while(!kbhit()) {
        Gotoxy(35, y + 3);
        if (visible)
            printf(">>> APPUYEZ POUR COMMENCER <<<");
        else //ça alterne entre les deux
            printf("                              "); // Efface
        
        visible = !visible;
        Sleep(500); // 0.5 sec
    }
    getch(); //vide la touche appuyé                                                                                                                                                                                                                                                                                               
}

void afficherEcranVictoire(){          
	system("cls");
	int x = 40; int y = 5;
	Color(14, 0); // Or
	Gotoxy(x, y++);      printf("   ______________      ");
	Gotoxy(x, y++);    printf("     '._==_==_=_.'     ");
	Gotoxy(x, y++);    printf("     .-\\:      /-.     ");
	Gotoxy(x, y++);    printf("    | (|:.     |) |    ");
	Gotoxy(x, y++);    printf("     '-|:.     |-'     ");
	Gotoxy(x, y++);    printf("       \\::.   /       ");
	Gotoxy(x, y++);    printf("        '::. .'        ");
    Gotoxy(x, y++);    printf("          ) (          ");
    Gotoxy(x, y++);    printf("        _.' '._        ");
    Gotoxy(x, y++);    printf("       mmmmmmmmm    ");

	y += 2;
	x = 15;
	Color(10, 0); //vert
	Gotoxy(x, y++); printf("██  ██ ██ ▄█████ ██████ ▄████▄ ██ █████▄  ██████ ");
	Gotoxy(x, y++); printf("██▄▄██ ██ ██       ██   ██  ██ ██ ██▄▄██▄ ██▄▄   ");
	Gotoxy(x, y++); printf(" ▀██▀  ██ ▀█████   ██   ▀████▀ ██ ██   ██ ██▄▄▄▄ ");
	Color(15, 0);
    Gotoxy(35, y + 4);
    printf("Appuyez sur une touche pour quitter...");
    getch();
}

void afficherEcranDefaite(){
	system("cls");
	int x = 15;
	int y = 10;
	
	Color(12, 0); //rouge
	Gotoxy(x, y++); printf(" ▄████  ▄████▄ ██▄  ▄██ ██████   ▄████▄ ██  ██ ██████ █████▄  ");
	Gotoxy(x, y++); printf("██  ▄▄▄ ██▄▄██ ██ ▀▀ ██ ██▄▄     ██  ██ ██▄▄██ ██▄▄   ██▄▄██▄ ");
	Gotoxy(x, y++); printf(" ▀███▀  ██  ██ ██    ██ ██▄▄▄▄   ▀████▀  ▀██▀  ██▄▄▄▄ ██   ██ \n\n");
	Color(15, 0);
    Gotoxy(35, y + 4);
    printf("Appuyez sur une touche pour quitter...");
    getch();
}



