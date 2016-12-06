#ifndef DIALOGSETPOINT_H
#define DIALOGSETPOINT_H

#include <QWidget>
#include<QDialog>
#include<QColor>
#include<QPoint>
#include<qmath.h>
#include<QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include<QPushButton>

#include<QScreen>
#include<QGuiApplication>
#include<QComboBox>

#include<QDebug>

/*!
 * @file dialogsetpoint.h
 * @author Rimbaud Sébastien
 * @brief Fichier permettant la transmission de données d'affichage d'un point.
 * @date 22 Mai 2016
 *
 * Requiert dans le .pro :
 *
 * \arg QT += gui
 * \arg QT += core
 * \arg QT += widgets
 *
 */

/*!
 * \brief Enumération du type unit
 * Permet de décrire les différentes unités disponible mm : millimètres, pixels.
 */
enum unit {mm, pixels};
/*!
 * @class PointData
 * @brief Format de données permettant le stockage d'information d'affichage d'un point. Permet également des conversion pixels/Mm
 *
 */
class PointData : public QObject
{

    Q_OBJECT
public :
    //static :

    //static void init();
    /*!
     * \brief Conversion de millimètres vers pixels selon l'axe X
     * \param Ax : Coordonnée d'abscisse en millimètre
     * \return Entier en pixels
     */
    int convertMMtoPX(int Ax);
    /*!
     * \brief Conversion millimètres vers pixels selon l'axe Y
     * \param Ay : Coordonnée d'ordonnée en millimètre
     * \return Entier en pixels
     */
    int convertMMtoPY(int Ay);
    /*!
     * \brief Surcharge de convertPtoMMX
     * \param Ax
     * \return flottant en pixels
     */
    float convertPtoMMX(int Ax);
    /*!
     * \brief Surcharge de convertPtoMMY
     * \param Ay
     * \return flottant en pixels
     */
    float convertPtoMMY(int Ay);
    /*!
     * \brief Conversion d'un type QString vers un type unit.
     *
     * ATTENTION aucune gestion des erreurs. Cette fonction fonctionne avec les chaines suivantes :
     * @arg "Milimètres" -> unit::mm
     * @arg "Pixels"     -> unit::pixels
     *
     * \param a : Chaine à convertir en type unit.
     * \return
     */
    unit convertStringtoUnit(QString a) ;
    // Constructeurs
    // de base on rentre les coordonnées en pixels
    /*!
     * \brief Constructeur de PointData
     *
     * Par défaut on initialise les données de la manière suivante :
     * @arg La taille de base vaut 4.
     * @arg L'unité est le pixel
     * @arg La couleur est rouge (Qt::red)
     *
     */
    PointData();
    /*!
     * \brief Surcharge constructeur PointData
     *
     * \param point : Coordonnées du point stocké dans un objet QPoint
     * \param r : rayon en pixels du point.
     * \param couleur : Couleur du point stocké dans un QColor.
     */
    PointData(QPoint point, int r, QColor couleur = Qt::red);
    //Plein de constructeurs surchargés (utilité discutable )
    /*!
     * \brief surcharge constructeur PointData
     * \param x : Coordonnée selon l'axe x
     * \param y : Coordonnée selon l'axe y
     * \param ra : Rayon en pixels du point.
     * \param r : Niveau de rouge (0 à 255) par défaut 255.
     * \param g : Niveau de vert (défaut 0)
     * \param b : Niveau de bleu (défaut 0)
     * \param a : Canal alpha (défaut 255), traduit la transparence du point.
     */
    PointData(int x, int y, int ra, int r = 255, int g =0 , int b =0, int a = 255);
    /*!
     * \brief Donne la couleur du point enregistré.
     * \return
     */
    QColor GetCouleur() const ;
    /*!
     * \brief Donne le point enregistré.
     * \return Coordonnée sous forme d'objet Qpoint.
     */
    QPoint GetPoint() const ;
    /*!
     * \brief Donne l'unité du point enregistré.
     * \return
     */
    int GetUnit()const  ;
    /*!
     * \brief Donne le rayon en pixels du point enregistré.
     * \return
     */
    int GetRayon() const;
    /*!
     * \brief Change l'unité enregistrée sans faire de calcul de conversion.
     * \param unite
     */
    void SetBrutalUnit(unit unite) ;
    //Accesseurs
public slots :
    /*!
     * \brief Met à jour la couleur du point enregistré
     * \param couleur : format QColor.
     */
    void SetCouleur(QColor* couleur);
    /*!
     * \brief Surcharge de SetCouleur(QColor* couleur)
     * \param r : Niveau de rouge
     * \param g : Niveau de vert
     * \param b : Niveau de bleu
     * \param a : Canal alpha (transparence)
     */
    void SetCouleur(int r, int g, int b, int a);
    /*!
     * \brief Met à jour le rayon du point enregistré
     * \param r
     */
    void SetRayon(int r);
    /*!
     * \brief Surcharge de SetRayon(int r)
     *
     * Utile pour connecter le slot à QLineEdit::textChanged(const QString&) et QLineEdit::textEdited(const QString &)
     * La conversion est effectuée en appelant la méthode statique QString::number(int, int base=10)
     *
     * \param r : Format QString
     */
    void SetRayon(QString r);
    /*!
     * \brief Met à jour les coordoonnées enregistrées.
     * \param point
     */
    void SetPoint(QPoint* point);
    /*!
     * \brief Surcharge de SetPoint(QPoint* point)
     * \param x : Coordonnée sur x
     * \param y : Coordonnée sur y
     */
    void SetPoint(int x, int y);
    /*!
     * \brief Met à jour la coordonnée x du point.
     *
     * Utile pour connecter le slot à QLineEdit::textChanged(const QString&) et QLineEdit::textEdited(const QString &)
     * La conversion est effectuée en appelant la méthode statique QString::number(int, int base=10)
     * Si la conversion échoue une boite de dialogue est affiché annonçant une mauvaise valeur (voir la méthode privée check(bool* v))
     *
     * \param x : Format chaine de caractère
     */
    void SetxCoord(QString x);
    /*!
     * \brief Met à jour la coordonnée y du point
     *
     * Voir SetxCoord.
     *
     * \param y : Format chaine de caractère

     */
    void SetyCoord(QString y);


    // Fonctions
    /*!
     * \brief Met à jour le point en passant d'un répère carthésien dans une unité vers un autre dans une autre unité.
     *
     * Cette fonction effectue un changement d'échelle par le biais d'une règle de 3.
     * Elle permet en plus d'effectuer un changement d'unité en plus du changement d'échelle.\n
     * Ex :\n
     * Conversion d'un point de coordonnée (100,100) dans un repère en pixels de dimension (1920x1080)
     * vers des coordonnées dans un repère en millimètre de dimension( 100x100).\n
     * Utile pour convertir les coordonnées d'un point de la tablette ayant ses dimensions propres en mm vers
     * des coordonnées correspondant à la résolution de l'écran qui est en pixels.
     *
     *
     * \param lx1 : Taille axe X du repère carthésien source
     * \param ly1 : Taille axe Y du repère carthésien source
     * \param unit1 : Unité du répére carthésien source
     * \param lx2 : Taille axe X du repère carthésien destination
     * \param ly2 : Taille axe Y du repère carthésien destination
     * \param unit2 : Unité du repère de destination
     * \param returnUnit : Unité finale de la conversion
     */
    void SetScale(int lx1, int ly1, unit unit1, int lx2, int ly2, unit unit2, unit returnUnit = pixels );

    /*!
     * \brief Mise à jour de l'unité avec conversion
     * \param unite
     */
    void SetUnit(unit unite);
    /*!
     * \brief Change l'unité du point sans conversion.
     *
     * Utile pour connecter le slot à QLineEdit::textChanged(const QString&) et QLineEdit::textEdited(const QString &)
     * La conversion est faite en appelant la méthode publique : PointData::convertStringtoUnit(QString a)
     *
     * \param unite : Format QString
     */
    void SetUnitDialog(QString unite);
private :
    /*!
     * \brief check
     *
     * Fonction appelée dans SetyCoord(QString y) et SetxCoord(QString x). Un booléen est mis à faux si la conversion a échoué.
     * Si v est faux, crée une boite de dialogue qui affiche : "Veuillez entrer une valeur entière" puis la supprime.
     * Fonction utile pour informer l'utilisateur que la valeur qu'il a entrée n'est pas valide.
     * Evite les bugs lors de Display::Pointe();
     *
     * \param v
     */
    void check(bool* v);
    /*!
     * \brief Fonction qui initialise la constante de conversion selon l'axe X et Y.
     *
     * Cette fonction récupère les dimensions en milimètres et en pixels de l'écran et calcule des constantes de conversions.
     * Ces constantes constituent des attributs privés de l'objet et ne sont pas modifiables une fois l'objet créé.
     * Sur certains appareils les dimensions en millimètres peuvent ne pas être disponibles.
     *
     * Voir : QScreen::physicalSize() const.
     */
    void init(); // initialisation des constantes de conversion
    /*!
     * \brief Constante flottante de conversion de mm vers pixels selon X
     *
     * Calculée dans init() et jamais modifiée ni modifiable après la création de l'objet.
     */
    float PIXELS_PAR_MMX; // constante de conversion
    /*!
     * \brief Identique à PIXELS_PAR_MMX selon l'axe Y.
     */
    float PIXELS_PAR_MMY;// constante de conversion
    /*!
     * \brief Couleur du point.
     */
    QColor* m_couleur;
    /*!
     * \brief Coordonnée du point .
     */
    QPoint* m_point;
    /*!
     * \brief Rayon du point.
     */
    int m_r;
    /*!
     * \brief Unité du point.
     */
    unit m_unite ;


};

/*!
 * @class DialogSetPoint
 * @brief Boite de dialogue demandant des données d'un point à placer à l'utilisateur.
 *
 * Boite de dialogue utilisée par MainWindow pour demander à l'utilisateur de placer un point sur le display.
 */
class DialogSetPoint : public QDialog
{

    Q_OBJECT
public:
    /*!
     * \brief DialogSetPoint
     *
     * Hérite de QDialog
     * Voir QDialog dans la documentation Qt.
     *
     * \param parent :
     */
    explicit DialogSetPoint(QWidget *parent = 0);
    /*!
     * \brief Renvoie le point enregistré lors de l'appel à GetData()
     *
     * Fonction à utiliser après que l'utilisateur ait fait ok dans la boite de dialogue.
     * Voir QDialog pour savoir comment utiliser une boite de dialogue.
     *
     * \return
     */
    PointData* GetData();
signals:

public slots:
    /*!
     * \brief Slot qui ouvre une boite QColorDialog pour choisir la couleur.
     * Voir QColorDialog pour plus d'information.
     */
    void SetCouleur();

private :
    /*!
     * \brief Champ pour entrer la coordonnée x.
     */
    QLineEdit* m_getx ;
    /*!
     * \brief Champ pour entrer la coordonnée y.
     */
    QLineEdit* m_gety ;
    /*!
     * \brief Place un menu déroulant.
     *
     * En patique ce menu déroulant possède deux choix : Milimètres et Pixels.
     * Son signal QComboBox::activated(QString) doit être connecté au slot PointData::SetUnitDialog(QString unite) de m_donnee
     */
    QComboBox* m_getunit;
    /*!
     * \brief Champ pour obtenir la taille.
     */
    QLineEdit* m_gettaille;
    /*!
     * \brief Titre couleur devant le boutons ouvrant QColorDialog.
     */
    QLabel* m_couleur ;
    /*!
     * \brief QPushButton ouvrant une QColorDialog.
     *
     * Son signal QPushButton::pressed() est connecté au slot DialogSetPoint::SetCouleur()
     */
    QPushButton* m_Bcouleur;
    /*!
     * \brief Bouton pour valider le choix (renvoie QDialog::Accepted)
     *
     * voir QDialog pour plus d'information.
     */
    QPushButton* m_valider;
    /*!
     * \brief Bouton pour annuler le choix (provoque QDialog::Rejected)
     *
     * Voir QDialog pour plus d'information.
     */
    QPushButton* m_annuler;
    /*!
     * \brief Variable qui stocke les données du point choisies par l'utilisateur.
     */
    PointData* m_donnee; // La donnée mise à jour par l'utilisateur dans cette boite de dialogue
    /*!
     * \brief QFormLayout contenant tous les boutons sauf Cancel et oK
     */
    QFormLayout* m_formulaire ;
    /*!
     * \brief Layout contenant les boutons ok et cancel.
     */
    QHBoxLayout* m_choixCouleur;
    /*!
     * \brief Layout de la fenêtre contient tous les autres layouts.
     */
    QVBoxLayout* m_dialogueForme;


};

#endif // DIALOGSETPOINT_H
