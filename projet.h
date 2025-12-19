#ifndef PROJET_H
#define PROJET_H

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h> 

// --- CONSTANTES GLOBALES ---
#define LIGNES 25
#define COLONNES 45
#define NB_TYPES 5      // Nombre d'items différents (1 à 5)
#define TYPE_VIDE 0     // Valeur d'une case vide

// --- STRUCTURE DU JEU ---
typedef struct {
  int grille[LIGNES][COLONNES];
  int score;
  int vies;
  int temps_restant;
  int coups_restants;
  int niveau_actuel;
  
  // Tableau des objectifs (Index 0 inutilisé, 1 à 5 correspondent aux types d'items)
  // On met NB_TYPES + 1 pour être tranquille avec les index
  int objectifs[NB_TYPES + 1]; 
} t_jeu;

// ============================================================
//                 PROTOTYPES : AFFICHAGE (affichage.c)
// ============================================================

// Outils console
void Gotoxy(int x, int y);
void Color(int t, int f);

// Ecrans complets
int afficherMenu();
void ecranAcceuil();
void afficherRegles();
void afficherEcranVictoire();
void afficherEcranDefaite();
void afficherMessageViePerdue(int vies_restantes);

// HUD (Interface en jeu)
void afficherNiveau(t_jeu jeu);
void afficherVies(t_jeu jeu);
void afficherScore(t_jeu jeu);
void afficherTemps(t_jeu jeu);
void afficherCoups(t_jeu jeu);
void afficherContrat(t_jeu jeu);
void afficherAide(); // Affiche les touches en bas
void cacherCurseur();

// La Grille
void afficherGrille(t_jeu jeu, int curseurX, int curseurY, int selectionActive);


// ============================================================
//                 PROTOTYPES : MOTEUR DE JEU (jeu.c)
// ============================================================

// Gestion de la grille
void initialisationplateau(int grille[LIGNES][COLONNES]);
void genererGrilleSansSuite(int grille[LIGNES][COLONNES]);
void remplissagecasesvides(int grille[LIGNES][COLONNES]);
void appliquerGravite(int grille[LIGNES][COLONNES]);

// Mécaniques de jeu (Match-3)
int permuterCases(t_jeu *jeu, int x1, int y1, int x2, int y2);
int detecterMarques(t_jeu *jeu, int marque[LIGNES][COLONNES]);
int appliquerMarques(t_jeu *jeu, int marque[LIGNES][COLONNES]);

// Configuration
void chargerParametresNiveau(int niveau, int objectifs[NB_TYPES], int *coups_max, int *temps_max);

// Sauvegarde
int chargerSauvegarde(const char *pseudo, int *niveau, int *vies, int *score);
int sauvegarderSauvegarde(const char *pseudo, int niveau, int vies, int score);

#endif
