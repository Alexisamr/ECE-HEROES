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
  int temps_restants;
  int coups_restants:
  int niveau_actuel;
  en_cours;
} t_jeu;

// TOUTES LES FONCTIONS 

void afficherMenu();
void afficherTitre();
void afficherRegles();
void afficherTemps(t_jeu jeu);
void afficherVieJoueur(t_jeu jeu);
void actualiser_grille(t_jeu jeu);

#endif
