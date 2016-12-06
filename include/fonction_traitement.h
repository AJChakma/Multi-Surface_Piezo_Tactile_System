#ifndef FONCTIONS_TRAITEMENT_H_INCLUDED
#define FONCTIONS_TRAITEMENT_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @file fonction_traitement.h
 * @author Alan-Joyti Chakma et Mathias Brunet.
 * @date 22 Mai 2016
 *
 * @brief Fichier permettant de générer le tableau de référence et d'y trouver les coordonnées correspondantes à 2 différences de temps.
 *
 * La table de référence contient le quadrillage de la surface tactile.@n
 * Ses éléments permettent de relier les constantes A et B aux coordonnées x et y correspondantes. Les valeurs de A et B peuvent être
 * être calculées de deux manières : lorsque les différences de temps des appuis de l'utilisateur sont récupérées, elles sont multipliées
 * par la vitesse. Les constantes sont alors comparées à celles de la table de référence, obtenues par un calcul utilisant les coordonnées
 * de chaque noeud du quadrillage.
 *
 */

//Définition des cases du tableau
/*!
 * @struct carre
 * @brief Structure qui stocke les variables d'une case du tableau.
*/
typedef struct carre {
    double A;/*!< Constante A des équations */
    double B;/*!< Constante B des équations */
    double x;/*!< Coordonnée sur x en mm*/
    double y;/*!< Coordonnée sur y en mm*/
} CARRE;


//Définition d'une structure de coordonnées
/*!
 * @struct coord
 * @brief Structure qui stocke des coordonnées x et y
 */

typedef struct coord {
    double x;
    double y;
} COORD;

/*!
 * \brief Enumération losange qui correspond aux dimensions possibles. Les valeurs de losange correspondant à celle de TDialogCalibrage::CalibMode.
 */
enum losange{
    AUCUN, PETIT, MOYEN, GRAND
} ;
typedef enum losange LOSANGE;

//Fonction de comparaison utilisée pour qsort
/*!
 * \brief Fonction de comparaison utile au tri.
 * \param ax : qui correspond à un CARRE*
 * \param ay : qui correspond à un CARRE*
 * \return Renvoie un entier valant 1 si @p ax->A > @p ay->A; 0 si @p ax->A== @p ay->A ; et -1 sinon.
 */
int compare(const void* ax, const void * ay);

//Fonction de dichotomie utilisée pour la recherche des coordonnées
/*!
 * \brief Fonction qui cherche @p elem dans @p tab de taille @p nbcarre par dichotomie.
 * \param tab : Tableau dans lequel on cherche.
 * \param elem: Elément que l'on cherche.
 * \param nbcarre : Taille du tableau.
 * \return Renvoie un @p CARRE contenant @p elem.
 *
 * Cette fonction est récursive.
 */
CARRE dichotom(CARRE* tab, double elem, int nbcarre);

/*!
 * \brief Crée un tableau bidimensionnel de CARRE.
 * \param taillex
 * \param tailley
 * \return Pointeur sur le tableau.
 *
 * Le tableau alloué par cette fonction est contigüe en mémoire. Pour chaque coordonnée i et j on calcule les constantes A et B de CARRE.
 */
CARRE** creationtab2D(int taillex, int tailley);
/*!
 * \brief Même fonction de creationtab2D() mais pour un tableau unidimensionnel.
 * \param tab
 * \param taillex
 * \param tailley
 * \return Pointeur sur le tableau.
 */
CARRE* creationtab1D(CARRE** tab, int taillex, int tailley);

//Ecriture du tableau dans un fichier
/*!
 * \brief Ecrit le tableau t de taille nbcarre dans le fichier s.
 * \param t : Tableau à écrire.
 * \param s : Nom du fichier de destination.
 * \param nbcarre : Taille du tableau.
 */
void ecrituretab(CARRE* t,char* s,int nbcarre);

//Lecture du fichier créé
/*!
 * \brief Lit un fichier créé par ecrituretab() et renvoie la liste de CARRE qui correspond.
 * \param s : Nom du fichier.
 * \param nbcarre : Taille du tableau.
 * \return Pointeur sur la liste chargée.
 *
 * Cette fonction n'effectue pas d'allocation dynamique il faut avoir alloué préalablement le tableau.
 */
CARRE* lecturetab(char* s,int nbcarre);

//Recherche des coordonnées associées aux double A et B obtenus avec les capteurs
/*!
 * \brief Recherche dans t les coordonnées associées aux doubles A et B avec la précision eps.
 * \param t : Tableau dans lequel se fait la recherche.
 * \param A : Constante A.
 * \param B : Constante B.
 * \param eps : Précision.
 * \param nbcarre : Taille tableau.
 * \return Renvoie les coordonnées trouvées.
 *
 * Les constantes A et B sont déduites des différences de temps émises par la tablette.
 */
COORD recherchecoord(CARRE* t, double A, double B, double eps, int nbcarre);

//Fonction globale
/*!
 * \brief A partir du tableau t de taille taillex * tailley pour une onde se propageant à vitesse, détermine les coordonnées associées à delta_tx et delta_ty.
 * \param t : Tableau dans lequel on cherche les coordonnées.
 * \param taillex : Taille selon x du tableau.
 * \param tailley : Taille selon y.
 * \param vitesse : Vitesse de l'onde (calculée par le calibrage).
 * \param delta_tx : Différence de temps selon l'axe x envoyée par la tablette.
 * \param delta_ty : Différence de temps selon l'axe x envoyée par la tablette.
 * \param eps : Précision
 * \return Renvoie les coordonnées trouvées dans la table.
 *
 * Le résultat de cette fonction dépend fortement de la précision de la vitesse. Si elle trop éloignée de la réalité on obtient des résultats étranges.
 */
COORD rechercheglobale(CARRE* t, int taillex, int tailley, double vitesse, int delta_tx, int delta_ty, double eps);

#ifdef __cplusplus
}
#endif

#endif // FONCTIONS_TRAITEMENT_H_INCLUDED
