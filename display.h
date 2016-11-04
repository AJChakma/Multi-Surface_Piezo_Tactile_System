#ifndef DISPLAY_H
#define DISPLAY_H
#include"dialogsetpoint.h"
#include"tdialogcalibrage.h"
#include"initialisation.h"
#include"fonction_traitement.h"
#include"dialogsetpoint.h"
#include <QtWidgets/QWidget>
#include<QPoint>
#include<QPaintEvent>
#include<QRect>
#include <QPen>
#include<QMessageBox>
#include<qpainter.h> // ça ne compile pas avec <QPainter>
#include<QVector>
/*!
 * @file display.h
 * @brief header la classe display.
 * @author Rimbaud Sébastien
 * @date 22 Mai 2016
  */


/*!
 * \brief Classe qui a pour rôle d'afficher les points sur une surface blanche.
 *
 * Cette classe est une surcharge de QWidget dont on a blanchi le fond et fixé une taille minimale.
 * Nous avons surchargé la méthode QWidget::paintEvent(QPaintEvent* ) qui permet de dessiner sur le widget. A chaque appel de cette méthode
 * les points enregistrés sont dessinés sur la surface.@n
 * Fonctionnement :@n
 * La classe ajoute dans des listes les informations sur les points. Une liste pour les coordonnées, une pour les couleurs et une pour les tailles.
 * Elle possède 2 valeurs courantes de couleurs et taille. Ces valeurs correspondent aux tailles et couleurs des futurs points enregistrés.
 * Il faut donc les mettre à jour avant d'enregistrer un point. On peut ne pas y toucher ce qui permet de conserver le même format pour les points.
 *
 * Requiert :
 * \arg QT += core
 * \arg QT += widgets
 * \arg QT += gui
 *
 * \author Rimbaud Sébastien
 * \date 22 Mai 2016
 * \bug Changement de couleur non fonctionnel. Tous les points changent de couleur. Impossible d'avoir des poins de différentes couleurs.
 */
class Display : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Constructeur de la classe, il initialise les tailles à 4 Pixels par défaut et la couleur au rouge.
     *
     * Ce constructeur crée une boite de dialogue qui sera mise à jour et affichée à chaque problème. On économise ainsi du temps passé
     * à allouer une boite de dialogue à chaque erreur.
     *
     * \param parent
     */
    Display(QWidget* parent = 0);
    /*!
     * \brief Fonction qui affiche les points enregistrés.
     *
     * Elle fait appel à QWidget::update() qui appelle Display::paintEvent(QPaintEvent*). Vous devez utiliser cette fonction pour mettre à jour la surface.
     */
    void Print();
    /*!
     * \brief Surcharge de QWdiget::PaintEvent(QPaintEvent* )
     *
     * Voir documentation Qt pour plus d'information.
     */
    void paintEvent(QPaintEvent*);// Surchage de paintEvent de Widget
    ~Display();
    /*!
     * \brief Enregistre les données du point à afficher et l'affiche.
     *
     * En pratique une conversion vers des pixels est faite systèmatiquement. Les fonctions d'affichage ne fonctionnent qu'avec des coordonnées entières en pixels
     *
     * \param donnee : point à enregistrer.
     */
    void SetData(PointData* donnee);
protected :
    /*!
     * \brief Surcharge de QWdiget::minimumSizeHint()
     *
     * Fixe la taille minimale de l'objet. Ici par défaut la taille est de 400x400 pixels.
     *
     * \return
     */
    QSize minimumSizeHint() const; // Surcharge d'une méthode QWidget. Elle définit la taille minimale de la surface


public slots :
    /*!
     * \brief Affiche un point sur la surface. Unités en pixels.
     *
     * Cette fonction effectue une allocation dynamique d'un QPoint. Ce QPoint sera libéré si vous appelez clearBoard() qui efface tous les points affichés.
     * Ce slot appelle Pointe(QPoint* point).
     *
     * \param x : Coordonnées sur x
     * \param y : Coordonnées sur y
     */
    void Pointe(int x= 100, int y= 100);
    /*!
     * \brief Affiche un point sur la surface. Unité en pixels.
     *
     * Ce slot vérifie que le point sera visible sur la surface. S'il est hors champ une boite de dialogue warning s'ouvre.
     * Ce slot enregistre le QPoint* qui ne sera libéré qu'à l'appel de clearBoard().
     *
     * \param point
     */
    void Pointe( QPoint *point);
    /*!
     * \brief Supprime tous les points affichés sur la surface.
     *
     * Cette fonction désalloue tous les points alloués.
     * Et met à jour la surface par update. On a après ce slot une surface vierge.
     * Fait appel à la méthode privée vide().
     */
    void clearBoard(); // nettoie la tableau


    /*!
     * \brief Met à jour la couleur courante.
     *
     * Tous les prochains points auront la couleur annoncée.
     *
     * \param couleur : Au format QColor (voir QColor)
     * \bug En pratique tous les points de la surface changent de couleur.
     */
    void SetCouleur(const QColor& couleur) ;
    /*!
     * \brief Surcharge de SetCouleur(const QColor&)
     * \param r : Niveau rouge
     * \param g : Niveau vert
     * \param b : Niveau bleu
     * \param a : Transparence
     */
    void SetCouleur(int r, int g, int b, int a =255);
    /*!
     * \brief Fonction qui affiche une QMessageBox avec l'avertissement : message. Par défaut mode Warning.
     * \param message : Message à afficher à l'utilisateur
     * \param mode : Type de Message Box (Warning (defaut), Information, Critical)
     */
    void RiseMessage(const QString& message, const QString& mode = "Warning");

    /* Taille */
    /*!
     * \brief Change la taille du widget (en pixels)
     * \param size
     *
     * appelle la méthode QWidget::resize()
     */
    void SetSize(const QSize& size);
    /*!
     * \brief Surcharge de SetSize(const QSize&)
     * \param x : Taille selon x en pixels
     * \param y : Taille selon y en pixels
     */
    void SetSize(int x, int y);
    /*!
     * \brief Change la valeur courante du rayon en pixels du point à afficher.
     * \param t : Rayon point en pixels.
     */
    void SetPointSize(int t);

    /*Affichage avec traitement*/
    /*!
     * \brief Effectue le calcul des coordonnées du point en utilisant une table préalablement établie.
     * \param dtOE : Différences de temps en microsecondes entre le capteur Est et Ouest
     * \param dtNS : Différences de temps en microsecondes entre le capteur Nord et Sud
     *
     * La table est définie par le slot GetDATACalibrage;
     */
    void TraitementTable (int dtOE, int dtNS);
    /*!
     * \brief Calcul de la position par résolution numérique d'un système d'équations. NON IMPLEMENTEE.
     * \param x
     * \param y
     * @bug Pas implémentée.
     */
    void TraitementCalculatoire(int x, int y);

    //Calibrage
    /*!
     * \brief Mise à jour des dimensions de la tablette.
     * \param data : Liste d'entiers format {Mode, DistN/S, DistE/O} en mm
     * @deprecated Fonction remplacée par GetDATACalibrage()
     */
    void GetTableCalibrage(QList<int> data);
    /*!
     * \brief Mise à jour des temps pour le calibrage
     * \param data : Liste d'entiers en microsecondes au format : {TNord, TSUD, TEST, TOuest}
     */
    void GetTempsCalibrage(QList<int> data);
    /*!
     * \brief Met à jour les valeurs nécessaires au calibrage et lance le calibrage.
     * \param data : 2 listes : les dimensions (en mm) et les temps (en us)
     *
     * \arg Format dimension : {Mode, DistN/S, DistE/O}
     * \arg Format temps : {TNord, TSUD, TEST, TOuest}
     */
    void GetDATACalibrage(QList<QList<int> > data);




private :

    QVector<QPoint*>* m_Points ; /*!< Ensemble des points à afficher */
    QVector<int>* m_PointSize; /*!< Tailles des points à afficher */
    QVector<QColor*>* m_fill ; /*!< Couleurs des points à afficher */
    int m_CurrentPointSize ; /*!< Taille du prochain point que l'on ajoutera à la liste des points à afficher */
    QColor* m_CurrentColor; /*!< Couleur du prochain point que l'on ajoutera à la liste des points à afficher*/
    int m_len ; /*!< Nombre de points enregistrés. Compteur très important à incrémenter à chaque ajout d'un point. Ré-initialisé dans vide().*/



    QMessageBox* m_info ;/*!< Boite d'information affichable par RiseMessage() */
    // Méthodes de manipulation des vecteurs
    /*!
     * \brief Enregistre un point à afficher
     * \param point : Point alloué dynaminquement.
     *
     * Si le point n'est pas alloué dynamiquement on a des crashs.
     * La couleur et la taille du point enregistré sont données par m_CurrentColor et m_CurrentPointSize.
     */
    void ajouter(QPoint* point); // Ajoute un point à la liste
    /*!
     * \brief Vide toutes les listes. Réalise les free() nécessaires.
     */
    void vider(); // vide la liste, utile pour supprimer tous les points
    /*!
     * \brief Donne le point numéro i.
     * \param i : Indice du point.
     * \return Renvoie le i-ème point enregistré.
     */
    QPoint getPoint(int i) const ;
    /*!
     * \brief Donne la couleur du point i
     * \param i : Indice du point
     * \return Renvoie la couleur du i-ème point enregistré.
     */
    QColor getColor(int i) const;
    /*!
     * \brief Donne la taille du point i
     * \param i : Indice du point.
     * \return Renvoie la taille du i-ème point enregistré.
     */
    int getSize(int i) const;
    /*!
     * \brief Trace le point point de couleur couleur et de rayon r.
     * \param painter : Objet qui réalise le dessin (voir documentation Qt)
     * \param couleur : Couleur du point.
     * \param point : Coordonnées du point
     * \param r : Rayon du point
     *
     *
     * Cette fonction est appelée dans PaintEvent() de manière récursive sur chacun des points enregistrés.
     */
    void drawCercle(QPainter* painter, QColor* couleur, QPoint* point,int r);

    // Calibrage
    QList<int> m_DonneeCalib;/*!< Données des dimensions de la tablette */
    QList<int> m_Temps; /*!< Données des temps de la phase de calibrage. */
    double m_vitesse; /*!< Vitesse calculée par le calibrage, par GetDATACalibrage() */
    CARRE* m_Table; /*!< Dimensions de la tablette. Variable utile pour la compatibilité avec le code C*/



};

#endif // DISPLAY_H
