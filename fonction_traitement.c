#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "fonction_traitement.h"



int compare(const void* ax, const void * ay) {
    const CARRE pax = *((CARRE*) ax);
    const CARRE pay = *((CARRE*) ay);
    double Ax = pax.A;
    double Ay = pay.A;
    if(Ax>Ay) return 1;
    if(Ax==Ay) return 0;
    if(Ax<Ay) return -1;
}

CARRE dichotom(CARRE* tab, double elem, int nbcarre){
    if (nbcarre==1) return tab[0];
    else{
        if (elem > tab[nbcarre/2].A) return dichotom(tab+nbcarre/2,elem,nbcarre/2+nbcarre%2);
        else return dichotom(tab,elem,nbcarre/2+nbcarre%2);
    }
}

double calcul_constantes(int delta_t, double vitesse){
    return vitesse*(double)delta_t;
}

CARRE** creationtab2D(int taillex, int tailley){
    int i,j;
    double A,B;

    //Allocation dynamique continu du tableau à 2 dimensions
    CARRE** t = calloc(tailley,sizeof(*t));
    *t = calloc(taillex*tailley,sizeof(**t));
    for (i=1;i<tailley;i++)    t[i] = t[i-1]+taillex;

    //Remplissage
    for (i=0; i<tailley; i++){
        for(j=0; j<taillex; j++){
            //Calcul de A et B pour chaque coordonnée (i,j)
            A=sqrt(pow(10*j,2.0)+pow(tailley/2.0-10*i,2.0)) - sqrt(pow(taillex-10*j,2.0)+pow(tailley/2.0-10*i,2.0));
            B=sqrt(pow(10*j-taillex/2.0,2.0)+pow(tailley-10*i,2.0)) - sqrt(pow(10*j-taillex/2.0,2.0)+pow(10*i,2.0));
            t[i][j].A=A;
            t[i][j].B=B;
            t[i][j].x=10*j-taillex/2.0;
            t[i][j].y=tailley/2.0-10*i;
        }
    }
    return t;
}

CARRE* creationtab1D(CARRE** tab, int taillex, int tailley){
    int i,j;
    CARRE* t = calloc(taillex*tailley,sizeof(*t));
    for (i=0;i<tailley;i++){
        for(j=0;j<taillex;j++){
            t[i*taillex+j] = tab[i][j];
        }
    }
    qsort(t,taillex*tailley,sizeof(*t),compare);
    return t;
}

void ecrituretab(CARRE* t,char* s,int nbcarre){
    FILE* f = fopen(s,"w+");
    int i;
    for (i=0;i<nbcarre;i++){
        fprintf(f,"%lf %lf %lf %lf\n",t[i].A,t[i].B,t[i].x,t[i].y);
    }
    fclose(f);
}

CARRE* lecturetab(char* s,int nbcarre){
    CARRE* t = calloc(nbcarre,sizeof(*t));
    FILE* f = fopen(s,"r");
    int i;
    for (i=0;i<nbcarre;i++){
        fscanf(f,"%lf %lf %lf %lf",&(t[i].A),&(t[i].B),&(t[i].x),&(t[i].y));
    }
    fclose(f);
    return t;
}

COORD recherchecoord(CARRE* t, double A, double B, double eps, int nbcarre){
    double Aeps = (dichotom(t,A,nbcarre)).A;
    COORD c;
    int ind = 0,i=0;
    for (i=0;i<nbcarre;i++){
        if (t[i].A == Aeps && abs(t[i].B - B)<eps){
            eps = abs(t[i].B - B);
            ind = i;
        }
    }
    c.x = t[ind].x;
    c.y = t[ind].y;
    return c;
}

COORD rechercheglobale(CARRE* t, int taillex, int tailley, double vitesse, int delta_tx, int delta_ty, double eps){
    double A = calcul_constantes(delta_tx,vitesse);
    double B = calcul_constantes(delta_ty,vitesse);
    return recherchecoord(t,A,B,eps,taillex*tailley);
    }
