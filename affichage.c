#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void Gotoxy(int x, int y) { // c'est le truc qui permet de faire une fenêtre et de placer le cruser tout ça grâce à <windows.h>
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

//void Color() { pour mettre des couleurs si jamais dieu le veux INSHALAHH si on veut aussi

//}


void afficherMenu() {
    system('cls');
    printf("\n\n");
    printf("************************************\n");
    printf("  --------ECE HEROES SAGA--------\n");
    printf("************************************\n\n");
    
    printf("      1. Lancer une nouvelle partie\n");
    printf("      2. Charger une partie\n");
    printf("      3. Lire les regles\n");
    printf("      4. Quitter\n\n");
    
    printf("      Votre choix : ");
}

void afficherTitre() {
    Gotoxy(0, 0);
    Color(15, 0); 
    printf("=========================================================================================\n");
    printf("   PROJET MATCH-3  |  ECE HEROES  |  ING1  2025\n");
    printf("=========================================================================================\n");
}

void afficherRegles{

}

afficherTemps{

}

afficherVieJoueur{

}
