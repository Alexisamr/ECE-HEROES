#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void Gotoxy(int x, int y) { // c'est le truc qui permet de faire une fenêtre et de bouger le cruser tout ça grâce à <windows.h>
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void Color() { //pour mettre des couleurs si jamais dieu le veux INSHALAHH si on veut aussi
    COORD c; // 0=Noir, 15=Blanc, 12=Rouge, 10=Vert, 14=Jaune, 11=Cyan, 13=Violet
    c.X = x:
    c.Y = y;
    SetConsoleTestAttribute(H, f* 16 + t);
}

void afficherTitre() {
    Gotoxy(0, 0); //important pour être certain que l'affichage commence vraiment au début
    Color(15, 0); //blanc sur noir
    printf("=========================================================================================\n");
    printf("   PROJET MATCH-3  |  ECE HEROES  |  ING1  2025\n");
    printf("=========================================================================================\n");
}

void afficherMenu() {
    system('cls');// clear l'affichage
    printf("\n\n");
    Color(14,0); //jaune 
    printf("************************************\n");
    printf("  --------ECE HEROES --------\n");
    printf("************************************\n\n");
    Color(15,0);
    printf("      1. Lancer une nouvelle partie\n");
    printf("      2. Charger une partie\n");
    printf("      3. Lire les regles\n");
    printf("      4. Quitter\n\n");
    
    printf("      Votre choix : ");
}

void afficherRegles() {
    system("cls");
    afficherTitre();
    printf("\n --- LES REGLES --- \n\n"); // à finir faut que bien que je vérifie les regles mais je pense c'est ça 
    printf("                          "); //le but du jeu, les contraintes, les figures spéciales, les contrôles
    printf("                          ");
    printf("                          ");
    printf("                          ");
    printf("                          ");
    printf("                          ");
    printf("                          ");
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
    
}

void afficherCoups(t_jeu jeu) { // Si plus de coups ou pas le temps fin de la partie gros nul
    
}

void afficherNiveau(t_jeu jeu) { //pour savoir à quelle niveau en est l'utilisateur 
    
}

void afficherEcranFin(int resultat) { 
    //Vicoitre Contrat rempli 
    //Défaite Temps écoulé 
    //Défaite Aucun coups restant 
    //Défaite Plus de vies
}
