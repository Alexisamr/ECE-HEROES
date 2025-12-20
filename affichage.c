#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "projet.h"

void Gotoxy(int x, int y) { // grâce a la bibliothèque windows.h nous pouvons utiliser cette fonction qui permet de décider 
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void Color(int t, int f) { // proviens aussi de windows.h et permet les couleurs dans le terminale windows basique 
    HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H, f * 16 + t);
}

int afficherMenu() { //affichage du menu
	system("cls");
	Color(15, 0);
	int choix;
	printf("          ██▄  ▄██ ██████ ███  ██ ██  ██ \n");
	printf("          ██ ▀▀ ██ ██▄▄   ██ ▀▄██ ██  ██ \n");
	printf("          ██    ██ ██▄▄▄▄ ██   ██ ▀████▀ \n\n");
	
    printf("1. Afficher les regles du jeu\n");
	printf("2. Nouvelle partie\n");
	printf("3. Reprendre une partie\n");
	printf("4. Quitter\n"); // J'ai ajouté le \n pour que ce soit propre
	
    printf("Choisissez un mode entre 1 et 4 : ");
	scanf("%d", &choix);
    
    //tant que l'utilisateur entre une valeur entre 1 et 4
	while(choix < 1 || choix > 4){ 
		printf("Choissisez un menu entre 1 et 4 :");
		scanf("%d", &choix);
	}
    
	printf("%d", choix);
	return choix;
}

void afficherRegles() { //affichage des reglages 
    system("cls"); //clear proprement l'affichage
	Gotoxy(0, 1), printf("██     ██████ ▄█████   █████▄  ██████  ▄████  ██     ██████ ▄█████");
	Gotoxy(0, 2), printf("██     ██▄▄   ▀▀▀▄▄▄   ██▄▄██▄ ██▄▄   ██  ▄▄▄ ██     ██▄▄   ▀▀▀▄▄▄");
	Gotoxy(0, 3), printf("██████ ██▄▄▄▄ █████▀   ██   ██ ██▄▄▄▄  ▀███▀  ██████ ██▄▄▄▄ █████▀\n\n");

    printf("BUT DU JEU : \n");
	printf("- Alignez 3 symboles identiques pour les détruire et marquer des points\n"); //le but du jeu, les contraintes, les figures spéciales, les contrôles
    printf("- Remplissez le CONTRAT avant la fin du temps imparti !\n\n");
	
    printf("COMMANDES : \n");
    printf("- Les deplacement se font avec les fléches de votre clavier en bas à droite\n");
	printf("- [ESPACE] pour selectionner et echanger deux cases. \n\n");
	
    printf("BONUS & DEFAITE :\n"); 
    printf("- Alignez 4 ou 5 items pour créer des EXPLOSIONS !\n");

	printf("  Appuyer sur une touche pour revenir au menu ...");
    getch();       // ça c'est hyper important en gros ça nétoie les petites merde ça purge pour les futures entrée de l'utilisateur (merci gemini 3 pro max ultra HD 4K) 
}

void afficherTemps(t_jeu jeu ){ //t_jeu qui correspond au struct qui se trouve dans le headers avec toute les variables importantes du code
    Gotoxy(55, 2);
    Color(14, 0); //encore jaune la couleur du pipi
    printf("TEMPS: %d s  ", jeu.temps_restant);
    Color(15,0);
}

void afficherVies(t_jeu jeu){  
    Gotoxy(18, 2); //rouge
    Color(12, 0);
    printf("VIES: %d", jeu.vies);
    Color(15, 0);
}

void afficherScore(t_jeu jeu){
    Gotoxy(32, 2); 
    Color(11,0); // cyan
    printf("| SCORE: %05d", jeu.score); // 05 ça met des zéro devant pour faire un score styléaisea_iazruhaze
    Color(15, 0); // et hop on remet blanco
}

void afficherMessageViePerdue(int vies_restantes) {
	// le message doit apparaitre juste en dessous rapidement 
    int x_msg = 15; 
    int y_msg = 32; 

    // rouge 
    Color(12, 0); 
    Gotoxy(x_msg, y_msg);
    printf(">> Vous avez perdu une VIE !");

    Color(15, 0);
    Gotoxy(x_msg, y_msg + 1);
    printf(">> Attention : il ne vous reste plus que %d vies !", vies_restantes);

	// pause de 1,5s pour que le joueur lise
    Sleep(1500);

	// et on efface
    Gotoxy(x_msg, y_msg);
    printf("                                                  "); 
    Gotoxy(x_msg, y_msg + 1);
    printf("                                                  "); 
}

void afficherGrille(t_jeu jeu, int curseurX, int curseurY, int selectionActive) {
    int startX = 15; 
    int startY = 5;
    
	//définition des symboles qui composeront la grille
    char symboles[] = {'0', '1', '2', '3', '4', '5'}; 
    
    // 0:Gris, 1:Rouge, 2:Vert, 3:Jaune, 4:Cyan, 5:Violet
    int couleurs[] = {8, 12, 10, 14, 11, 13}; 

    // cadre du haut 
    Color(15, 0);
    Gotoxy(startX - 1, startY - 1); printf("╔");
    for(int k=0; k<COLONNES*2; k++) printf("═"); 
    printf("╗");

    for (int i = 0; i < LIGNES; i++) {
        // Bordure gauche
        Gotoxy(startX - 1, startY + i); 
        Color(15, 0); printf("║");

        for (int j = 0; j < COLONNES; j++) {
            int val = jeu.grille[i][j];
            
            // sécurité si jamais une valeur dépasse 5
            if (val > 5) val = 0; 

            int couleurTexte = couleurs[val];
            int couleurFond = 0; 

            // gestion du curseur 
            if (j == curseurX && i == curseurY) {
                if (selectionActive) couleurFond = 5; // fond violet quand l'utilisateur selectionne 
                else couleurFond = 8; // fond gris quand 
                
                if(val != 0) couleurTexte = 15; //
            }
            
            // affichage de la case 
            Gotoxy(startX + (j * 2), startY + i); 
            Color(couleurTexte, couleurFond);
            printf("%c ", symboles[val]); 
        }

        // bordure de droite
        Color(15, 0); printf("║");
    }

    // cadre du bas
    Gotoxy(startX - 1, startY + LIGNES); printf("╚");
    for(int k=0; k<COLONNES*2; k++) printf("═");
    printf("╝");
    

}

// le curser de base dans la console windows clignotte et cette fonction cacherCurser supprime cela
void cacherCurseur() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE; // FASLE = invisible
    SetConsoleCursorInfo(consoleHandle, &info);
}

void afficherContrat(t_jeu jeu) {
    // on place le contrat à Droite de la grille (x=115) pour ne pas gêner
    int x_depart = 115;
    int y_depart = 10;

	//définition des symboles qui composeront la grille
    char symboles[] = {'0', '1', '2', '3', '4', '5'}; 
    
    // 0:Gris, 1:Rouge, 2:Vert, 3:Jaune, 4:Cyan, 5:Violet
    int couleurs[] = {8, 12, 10, 14, 11, 13}; 


    Gotoxy(x_depart, y_depart);
    Color(15, 0); // blanc
    printf("---- CONTRAT ----");

    int i;
    int ligne_actuelle = 0;

    // On parcourt les 5 types d'items
    for(i = 1; i <= 5; i++) {
        
        // on affiche seulement s'il reste des items de ce type à détruire
        if (jeu.objectifs[i] > 0) {
            ligne_actuelle++;
            Gotoxy(x_depart, y_depart + (ligne_actuelle * 2)); // espacé de 2 ligne

            Color(couleurs[i], 0); 
            printf("[%c]", symboles[i]);

            Color(15, 0);
            // Le "   " à la fin sert à effacer les vieux chiffres (si on passe de 10 à 9)
            printf(" : x %d a eliminer   ", jeu.objectifs[i]);
        }
        else {
            // a rajouter plus tard : si l'objectif est fini (0), on affiche "OK" en vert
            ligne_actuelle++;
            Gotoxy(x_depart, y_depart + (ligne_actuelle * 2));
            Color(10, 0); // Vert
            printf("TYPE %d : TERMINE !      ", i);
        }
    }
    
    Color(15, 0); // on remet blanc
}


void afficherCoups(t_jeu jeu) { //permet de connaitre le nombre restant de coups possible
    Gotoxy(80, 2);
    Color(13, 0);
    printf("COUPS : %d", jeu.coups_restants);
}

void afficherNiveau(t_jeu jeu) { //pour savoir à quelle niveau en est l'utilisateur 
    Gotoxy(0,0);
    Color(4, 2);
    printf("NIVEAU : %d", jeu.niveau_actuel);
}

void afficherAide() {
    Gotoxy(10, 5 + LIGNES + 1);
    Color(15, 0);
    printf("                                                                           \n");
    printf("                                                                           \n");
    printf("Espace=selection/valider | X=annuler | Fleches ou ZQSD | P=sauvegarder+menu                    ");
}


//ASCII ART
//ASCII ART


void ecranAcceuil() {   
	system("cls");
	int x_depart = 10;
    int y = 5;
    int espacement = 8; //pour les espacement entre chaque grosse lettre

	int x = x_depart;
	Color(12, 0); //rouge rouge      
	Gotoxy(x, y);   printf("██████");
	Gotoxy(x, y+1); printf("██▄▄  ");
	Gotoxy(x, y+2); printf("██▄▄▄▄");
	x += espacement;
	Color(4, 0); //rouge bresson
	Gotoxy(x, y);   printf("▄█████");
	Gotoxy(x, y+1); printf("██    ");
	Gotoxy(x, y+2); printf("▀█████");
	x += espacement;
	Color(6, 0); //orange
	Gotoxy(x, y);   printf("██████");
	Gotoxy(x, y+1); printf("██▄▄  ");
	Gotoxy(x, y+2); printf("██▄▄▄▄");
	x += 4; //espace
	
	x += espacement;
	Color(14, 0); //jaune jaune       
	Gotoxy(x, y);   printf("██  ██");
	Gotoxy(x, y+1); printf("██████");
	Gotoxy(x, y+2); printf("██  ██");
	x += espacement;
	Color(10, 0); //vert vert
	Gotoxy(x, y);   printf("██████");
	Gotoxy(x, y+1); printf("██▄▄  ");
	Gotoxy(x, y+2); printf("██▄▄▄▄");
	x += espacement;
	Color(2, 0); //vert bresson   
	Gotoxy(x, y);   printf("█████▄ ");
	Gotoxy(x, y+1); printf("██▄▄██▄");
	Gotoxy(x, y+2); printf("██   ██");
	x += espacement;
	Color(11, 0); //cyan       
	Gotoxy(x, y);   printf("▄████▄");
	Gotoxy(x, y+1); printf("██  ██");
	Gotoxy(x, y+2); printf("▀████▀");
	x += espacement;
	Color(9, 0); //bleu bien bleu
	Gotoxy(x, y);   printf("██████");
	Gotoxy(x, y+1); printf("██▄▄  ");
	Gotoxy(x, y+2); printf("██▄▄▄");
	x += espacement;
	Color(13, 0); //violet
  	Gotoxy(x, y);   printf("▄█████");
	Gotoxy(x, y+1); printf("▀▀▀▄▄▄");
	Gotoxy(x, y+2); printf("█████▀");

	// sous titre 
	y += 5;
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
        else //ça alterne entre les deux pour faire l'effet qui clignote
            printf("                              "); 
        
        visible = !visible;
        Sleep(500); // 0.5 sec
    }
    getch(); //vide la touche pressé                                                                                                                                                                                                                                                                                           
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
	Gotoxy(x, y++); printf("		██  ██ ██ ▄█████ ██████ ▄████▄ ██ █████▄  ██████ ");
	Gotoxy(x, y++); printf("		██▄▄██ ██ ██       ██   ██  ██ ██ ██▄▄██▄ ██▄▄   ");
	Gotoxy(x, y++); printf("	 	 ▀██▀  ██ ▀█████   ██   ▀████▀ ██ ██   ██ ██▄▄▄▄ ");
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
