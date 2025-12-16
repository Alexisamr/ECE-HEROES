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

void afficherMenu() {
	syteme('cls');
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
    printf("BUT DU JEU : ")
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
    printf("VIES: %d, jeu.vies);
    Color(15, 0);
}

void afficherScore(t_jeu jeu){
    Gotoxy(45, 0); 
    Color(11,0); // cyan
    printf("| SCORE: %05d", jeu.score): // 05 ça met des zéro devant pour faire un score styléaisea_iazruhaze
    Color(15, 0); // et hop on remet blanco
}

void actualiserGrille(jeu){
//fonction de nazi flm je la fait après
    
}

void afficherContrat(t_jeu jeu) { //faut que l'utilisateur sache quoi faire pour gagner
    Gotoxy(4, 0);
    Color(14, 0);
    printf("")
}

void afficherCoups(t_jeu jeu) { // Si plus de coups ou pas le temps fin de la partie gros nul
    Gotoxy(75, 1);
    Color(13, 0);
    printf("COUPS : %d, jeu.coups_restants);
}

void afficherNiveau(t_jeu jeu) { //pour savoir à quelle niveau en est l'utilisateur 
    Gotoxy(0,0);
    Color(14, 0)
    printf("NIVEAU : %d, jeu.niveau_actuel);
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
