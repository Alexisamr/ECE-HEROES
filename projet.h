#ifndef PROJET_H
#define PROJET_H

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define LIGNES 25
#define COLONNES 45 // jsp si c'est vrmt utile les deux define mais bon

typedef struct { //ça c'est pour toi ruben
  int grille[LIGNES][COLONNES];
  int ancienne_grille[LIGNES][COLONNES];
  int score;
  int vies;
  int temps_restant;
  int coups_restants;
  int niveau_actuel;
  int en_cours;
  int objectifs[6];
} t_jeu;

// TOUTES LES FONCTIONS 

int afficherMenu();
void afficherAcceuil();
void afficherEcranVictoire();
void afficherEcranDefaite();
void afficherRegles();
void afficherTemps(t_jeu jeu);
void afficherVieJoueur(t_jeu jeu);
void afficherGrille(t_jeu jeu, int curseurX, int curseurY, int selectionActive);

#endif
