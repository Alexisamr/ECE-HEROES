#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 5
#define P 10

void initialisationplateau(int tab[N][P]) {
	int i, j;
	for (i = 0; i < N; i++) {
        for (j = 0; j < P; j++) {
        	tab[i][j] = 0;
        }
    }
}

void remplissagecasesvides(int tab[N][P]) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < P; j++) {
        	if (tab[i][j]==0) {
        		tab[i][j] = rand() % 6;
        	}
        }
    }
}

void afficherplateauterminal(int tab[N][P]) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < P; j++) {
            printf("%3d ", tab[i][j]);
        }
        printf("\n");
    }
}

void gravite(int tab[N][P]) {
    int i, j;
	for (j = 0; j < P; j++) {                 
        for (i = N - 1; i > 0; i--) {        
            while (tab[i][j] == 0 && i > 0) {
                for (int k = i; k > 0; k--) {
                    tab[k][j] = tab[k - 1][j];
                }
                tab[0][j] = 0;
            }
        }
    }
}

void itemspeciaux(int tab[N][P]) {
    static int compteur = 1;
	for (int i = 0; i < N; i++) {
        for (int j = 0; j < P; j++) 
			if (tab[i][j] == 0) {
				if (compteur % 8 == 0)
                    tab[i][j] = '*';
                else if (compteur % 10 == 0)
                    tab[i][j] = '#';
				compteur++;
            }
        }
    }
}

void detectionsuite4horizontale(int tab[N][P]) {
	for (int i=0; i<N; i++) {
        int compteur=1;
		for (int j=1; j<P; j++) {
			if (tab[i][j]==tab[i][j-1]) {
                compteur++;
				if (count==4) {
                    tab[i][j]=0;
                    tab[i][j-1]=0;
                    tab[i][j-2]=0;
                    tab[i][j-3]=0;
					compteur();  
                    compteur = 1;   
                }
			} else {
                compteur = 1;
            }
        }
    }
}
void detectionSuite4Verticale(int tab[N][P]) {
	for (int j=0; j<P; j++) {        
        int compteur=1;
		for (int i=1; i<N; i++) {   
			if (tab[i][j]==tab[i-1][j]) {
                compteur++;
				if (compteur==4) {
					tab[i][j]=0;
                    tab[i-1][j]=0;
                    tab[i-2][j]=0;
                    tab[i-3][j]=0;
					compteur(); 
					compteur=1;
                }
			} else {
                compteurIdentiques = 1;
            }
        }
    }
}
void detectionCroix9(int tab[N][J]) {
    for(int i=1;i<N-1;i++){
        for(int j=1;j<J-1;j++){
            int x=tab[i][j];
            if(tab[i][j-1]==x && tab[i][j+1]==x && tab[i-1][j]==x && tab[i+1][j]==x){
                for(int col=0;col<J;col++)
					if (tab[i][col]==x) {
                    tab[i][col]=0;
					}
                for(int lin=0;lin<N;lin++)
					if (tab[lin][j]==x) {
                    tab[lin][j]=0;
					}
                compteur();
            }
        }
    }
}


void detectionCarre4(int tab[N][J]) {
    for(int i=0;i<=N-4;i++){
        for(int j=0;j<=J-4;j++){
            int x=tab[i][j];
            if(tab[i][j+1]==x &&
               tab[i][j+2]==x &&
               tab[i][j+3]==x &&
               tab[i+1][j]==x &&
               tab[i+1][j+1]==x &&
               tab[i+1][j+2]==x &&
               tab[i+1][j+3]==x &&
               tab[i+2][j]==x &&
               tab[i+2][j+1]==x &&
               tab[i+2][j+2]==x &&
               tab[i+2][j+3]==x &&
               tab[i+3][j]==x &&
               tab[i+3][j+1]==x &&
               tab[i+3][j+2]==x &&
               tab[i+3][j+3]==x){
                for(int a=0;a<4;a++)
                    for(int b=0;b<4;b++)
                        tab[i+a][j+b]=0;
                compteur();
            }
        }
    }
}


