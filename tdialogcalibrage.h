#ifndef TDIALOGCALIBRAGE_H
#define TDIALOGCALIBRAGE_H

#include <QDialog>
#include<QLineEdit>
#include<QDebug>
#include<QAbstractButton>
#include"tserial.h"
#define MINI 200
#define MOY 300
#define GRAN 500
/*!
 * @file tdialogcalibrage.h
 * @author Rimbaud Sébastien
 * @date 22 Mai 2016
 * @brief Module qui demande à l'utilisateur les paramètres de calibrage.
 *
 * La Précision est une donnée dont on ne tient pas compte
 * pour l'instant dans les calculs.
 *
 * Requiert :
 * \arg QT+= core
 * \arg QT+= gui
 * \arg QT+= widgets
 * \arg QT+= serialports
 *
 * Distance
 * \arg Nord/Sud
 * \arg Est/Ouest
 *
 * tableau de temps
 * \arg Touest-Test ==> T0 et T2
 * \arg Tnord-Tsud ==> T1, T3
 *
 * A chaque temps correspond un tap.
 */

/*!
 * \brief Enumération capteur : {N= 1, S = 3, E = 2, O= 0} correspond aux 4 points cardinaux.
 */
enum Capteur {N= 1, S = 3, E = 2, O= 0};

namespace Ui {
class TDialogCalibrage;
}
/*!
 * \brief Enumération qui définit le mode de calibrage {Perso, Mini, Moyen, Grand}
 */
enum CalibMode{Perso, Mini, Moyen, Grand}; // Définit le mode de calibrage.

/*!
 * \brief Boite de dialogue qui met à jour les données de calibrage et les envoie au display.
 *
 */
class TDialogCalibrage : public QDialog
{

    Q_OBJECT

public:
    /*!
     * \brief Constructeur. Il ferme puis réouvre le port série avec Ouvrir() et ferme(). Les messages d'erreurs sont affichés via des QMessageBox.
     * \param serie : Port série utilisé par le programme principal.
     * \param parent : Fenêtre qui a générée la boite de dialogue.
     *
     * Le constructeur va désactiver tous les boutons 'valider' et le bouton 'ok'.
     * Par défaut il met la configuration en mode mini.
     *
     */
    explicit TDialogCalibrage(TSerial* serie, QWidget *parent = 0);
    ~TDialogCalibrage();
    /*!
     * \brief Accesseur pour les dimensions entrées par l'utilisateur.
     * \return Renvoie les dimensions entrées par l'utilisateur.
     */
    QList<int> GetDonnee() const;
    /*!
     * \brief Accesseur pour les temps de la phase d'initialisation.
     * \return Renvoie les temps que l'utilisateur a calibrés.
     */
    QList<int> GetTemps() const;

public slots :
    /*!
     * \brief Slot qui réappelle une des 4 méthodes privées concernant les dimensions.
     * \param Mode : Mode renvoyé par l'utilisateur (utiliser un QComboBox est idéal)
     *
     * \arg "Taille Minimale" appelle ModeMini();
     * \arg "Taille Moyenne" appelle ModeMoyen()
     * \arg "Taille Maximale" appelle ModeGrand()
     * \arg "Taille Personnalisée" appelle ModePerso()
     *
     * Autrement rien n'est appelé. Ce slot sert de redirection vers des méthodes privées et donc protégées.@n
     * Il faut toujours l'appeler et le surcharger si besoin.
     */
    void Mode(QString Mode);
    /*!
     * \brief Mets à jour les distances NS dans la liste des données.
     * \param d : Chaîne de caractère format "valeur mm" ex : "200 mm".
     *
     */
    void UpdateDNS(QString d);
    /*!
     * \brief Similaire à UpdateDNS
     * \param d : Chaîne de caractère format "valeur mm". ex : "200 mm".
     */
    void UpdateDEO(QString d); // Prennent des QString car QLineEdit émet un signal avec un QString.

    /*!
     * \brief Slot appelé quand l'utilisateur appuie sur 'capteur N'. Met en forme la liaison et la fenêtre.
     *
     * Toujours passer par ce slot pour utiliser le calibrage d'un capteur associé à un point cardinal.
     * Ce slot désactive les autres boutons des autres capteurs pour ne pas calibrer plusieurs capteurs à la fois. Il active le bouton
     * valider et connecte UpdateLineEditN(int OE, int NS) avec TSerial::IntLus(int, int).
     * Ce slot est identique pour le capteur Est, Ouest et Sud. On a le format : Start<Point Cardinal> ()
     */

    void StartNord(); // Initialise les widgets pour recevoir des données dans les champs correspond, et connecte les champs à la liaison série
    /*!
     * \brief Met les entiers dans le QLineEdit qui correspondent au capteur.
     * \param OE : Différence de temps entre les capteurs Ouest et Est en microsecondes.
     * \param NS : Différence de temps entre les capteurs Nord et Sud en microsecondes
     * .
     * Ce slot est identique pour chaque point cardinal sous la forme : UpdateLineEdit<Première lettre du point cardinal en majuscule>(int OE, int NS)
     */
    void UpdateLineEditN(int OE, int NS);// Reçoit les int et les mets dans line Edit du capteur qui correspond
    /*!
     * \brief Slot connecté à textChanged(Qstring) d'un QlineEdit du capteur. Il s'assure que le champ est bien mis à jour.
     * \param texte : Texte inséré dans les champs des QLineEdit.
     *
     * En pratique il met à jour une liste de 4 booléens. Il met à jour le booléen correspondant à son capteur. Quand tous les booléens sont vrais
     * il active le bouton ok.
     * Ce slot est identique pour chaque point cardinal sous la forme : <PointCardinal>MisAjour(QString texte).
     */
    void NordMisAjour(QString texte); // Est appelé quand les lineEdit sont mis à jour pour assurer l'existe de donnée de mise à jour.
    /*!
     * \brief Slot appelé lorsque l'utilisateur appuie sur valider. Réactive ce qui est désactivé par StartNord() et déconnecte de la liaison série.
     * Ce slot est identique pour chaque point cardinal sous la forme ! Stop<PointCardinal>();
     */
    void StopNord(); // Désactive les champs et déconnecte les line edit de la liaison série.



    /*!
     * \brief Voir StartNord().
     */
    void StartSud();
    /*!
     * \brief Voir UpdateLineEditN(int, int)
     * \param OE
     * \param NS
     */
    void UpdateLineEditS(int OE, int NS);
    /*!
     * \brief Voir NordMisAjour
     * \param texte
     */
    void SudMisAjour(QString texte);
    /*!
     * \brief Voir StopNord.
     */
    void StopSud();



    /*!
     * \brief voir StartNord
     */
    void StartEst();
    /*!
     * \brief Voir UpdateLineEditN(int, int)
     * \param OE
     * \param NS
     */
    void UpdateLineEditE(int OE, int NS);
    /*!
     * \brief Voir NordMisAjour
     * \param texte
     */
    void EstMisAjour(QString texte);
    /*!
     * \brief Voir StopNord.
     */
    void StopEst();



    /*!
     * \brief Voir StartNord
     */
    void StartOuest();
    /*!
     * \brief Voir UpdateLineEditN(int, int)
     * \param OE
     * \param NS
     */
    void UpdateLineEditO(int OE, int NS);
    /*!
     * \brief Voir NordMisAjour
     * \param texte
     */
    void OuestMisAjour(QString texte);
    /*!
     * \brief Voir StopNord.
     */
    void StopOuest();


signals:

private:
    Ui::TDialogCalibrage *ui;/*!< Interface graphique réalisée sous QtDesigner */
    QList<int> m_Donnee; /*!< Liste les données. {Mode, DistN/S, DistE/O} */
    QList<int> m_Temps;  /*!< {TNord, TSUD, TEST, TOuest} */
    QAbstractButton* m_ok; /*!< Stocke le bouton ok pour pouvoir l'enable et le disable. */
    QList<bool> m_MisAJour; /*!<  Liste pour savoir si des données de calibrage ont été entrées (pour savoir si on peut cliquer sur ok sans qu'il n'y ait des données non chargées)*/
    /*!
     * \brief Met à jour les champs correspondant à la taille la plus petite ainsi que m_Donnee.
     */
    void ModeMini();
    /*!
     * \brief Met à jour les champs correspondant à la taille moyenne ainsi que m_Donnee.
     */
    void ModeMoyen();
    /*!
     * \brief Met à jour les champs correspondant à la taille grande ainsi que m_Donnee.
     */
    void ModeGrand();
    /*!
     * \brief Permet à l'utilisateur d'entrer les valeurs qu'il souhaite (en mm). Met à jour m_Donnee.
     *
     * Active les champs pour que l'utilisateur puisse les utiliser.
     */
    void ModePerso();

    /*!
     * \brief Vérifie si tous les champs sont mis à jour, si oui, active le bouton 'ok'. Met à vrai le booléen correspondant à pointCardinal.
     * \param PointCardinal
     */
    void CheckMisAJour(Capteur PointCardinal);

    TSerial* m_serie ;/*!< Pointeur sur TSerial. Utile pour les connexions.*/
};

#endif // TDIALOGCALIBRAGE_H
