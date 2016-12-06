#include <stdio.h>
#include <stdlib.h>
#include "fonction_traitement.h"
#include "initialisation.h"



/*
Protocole:

- Disposer les capteurs en losange en les numérotant de 1 à 4 (9h, 12h, 3h, 6h ou O, N, E, S)
- Renseigner la distance entre 1 et 3 : doe et entre 2 et 4 : dns.
- Tapper alternativement sur les capteurs dans l'ordre (1->4).
- Renseigner le temps to-te pour ti[0] et ti[2] et le temps tn-ts pour ti[1] et ti[3].
  (ti[i] correspondant au (i-1)ème tap sur le (i-1)ème capteur)
*/

double calcul_vitesse (double doe, double dns, int* ti){
	double vitesse = 0.0;
	int i;

	// Calcul de A ou B pour les cas intéressants
	double distance[4];
		// 1 : x = -doe/2 , y = 0		-> Calcul de A
		distance[0] = -doe;
		// 2 : x = 0 ,		y = dns/2	-> Calcul de B
		distance[1] = dns;
		// 3 : x = doe/2 , 	y = 0		-> Calcul de A
		distance[2] = doe;
		// 4 : x = 0 ,		y = -dns/2	-> Calcul de B
		distance[3] = -dns;

	// Calcul de la vitesse moyenne
	for (i=0; i<4; i++) {
		vitesse += distance[i] / ti[i];
	}

	return vitesse / 4;
}

void initialisation_fichiers(LOSANGE losange, double doe, double dns, int* ti){
    CARRE* tab1d;
    double nbcarre = doe*dns;
    double vitesse = calcul_vitesse(doe,dns,ti);
    double A = calcul_constantes(ti[0]-ti[2],vitesse);
    double B = calcul_constantes(ti[1]-ti[3],vitesse);
    switch(losange){
        case AUCUN:
            tab1d = creationtab1D(creationtab2D(doe,dns),doe,dns);
            ecrituretab(tab1d,"losangeredefini.txt",nbcarre);
            break;

        case PETIT:
            tab1d = creationtab1D(creationtab2D(doe,dns),doe,dns);
            ecrituretab(tab1d,"petitlosange.txt",nbcarre);
            break;

        case MOYEN:
            tab1d = creationtab1D(creationtab2D(doe,dns),doe,dns);
            ecrituretab(tab1d,"moyenlosange.txt",nbcarre);
            break;

        case GRAND:
            tab1d = creationtab1D(creationtab2D(doe,dns),doe,dns);
            ecrituretab(tab1d,"grandlosange.txt",nbcarre);
            break;
    }
}

