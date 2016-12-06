#ifndef INITIALISATION_H_INCLUDED
#define INITIALISATION_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif
#include"fonction_traitement.h"
/*!
 * @file initialisation.h
 * @author Damien Cathrine, Alan-Jyoti Chakma.
 * @date 22 Mai 2016
 * @brief fonctions permettant le calibrage de l'interface.
 *
 * Protocole de calibrage :
 * \arg Disposer les capteurs en losange en les numérotant de 1 à 4 (9h, 12h, 3h, 6h ou O, N, E, S)
 * \arg Renseigner la distance entre 1 et 3 : doe (distance ouest-est) et entre 2 et 4 : dns (distance nord-sud).
 * \arg Taper alternativement sur les capteurs dans l'ordre (1->4).
 * \arg Renseigner le temps to-te pour ti[0] et ti[2] et le temps tn-ts pour ti[1] et ti[3].
 * (ti[i] correspondant au (i-1)ème tap sur le (i-1)ème capteur).
 *
 */

// Prototypes

//Fonction de calcul de la vitesse en connaissant la distance entre les capteurs et les temps d'initialisation
/*!
 * \brief Calcule la vitesse à partir des distances d13 = Ouest/Est et d24= Nord/Sud et des temps correspondant (voir calibrage)
 * \param d13 : distance Ouest/Est en mm.
 * \param d24 : Distance Nord/Sud en mm.
 * \param ti : Temps renvoyé par l'interface de calibrage.
 * \return Renvoie la vitesse calculée.
 */
double calcul_vitesse (double d13, double d24, int* ti); //d13 = doe, d24 = dns

//Ecriture des tables dans les fichiers en fonction du losnage choisi par l'utilisateur
/*!
 * \brief Ecriture des tables dans les fichiers en fonction du losange choisi par l'utilisateur.
 * \param losange : Format choisi du losange.
 * \param doe : Distance O/E.
 * \param dns : Distance N/S.
 * \param ti : Tableau de temps reçus du calibrage.
 *
 *
 */
void initialisation_fichiers(LOSANGE losange, double doe, double dns, int* ti);

#ifdef __cplusplus
}
#endif

#endif // INITIALISATION_H_INCLUDED
