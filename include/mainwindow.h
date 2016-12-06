#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"display.h"
#include"dialogsetpoint.h"
#include"serialpannel.h"
#include "tserialmonitor.h"
#include "tdialogcalibrage.h"
#include<QLabel>
#include<QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include<QGridLayout>
#include <QWidget>
#include<QPalette>
#include<QMessageBox>
#include<QString>
/*!
 * \file mainwindow.h
 * @author Rimbaud Sébastien
 * @date 22 Mai 2016
 * @brief Fichier contenant les données de la fenêtre principale.
 * requiert :
 * \arg QT+= core
 * \arg QT+= gui
 * \arg QT += widgets
 * \arg QT += serialports
 *
 */



/*!
 * \brief La classe de la fenêtre principale.
 *
 * Cette fenêtre fait la liaison entre tous les autres widgets de l'application. Elle se charge de transmettre les signaux,
 * réalise les connexions et déconnexions nécessaires au bon fonctionnement de l'application. Elle vérifie que l'on ne cherche
 * pas à calculer une position tant que le calibrage n'a pas été fait.
 */
class MainWindow : public QWidget
{
    /*!
     * \brief Enumération qui décrit le mode de traitement choisit. Par table ou par résolution numérique.
     */
    enum ModeTraitement {Table, CalculSysteme};
    Q_OBJECT
public:
    /*!
      * \brief Constructeur de la classe. Il réalise les connexions de slots/signaux minimales.
      * \param parent
      *
      * Pour voir l'ensemble des connexions réalisées veuillez consulter le code source.
      *
      */
     explicit MainWindow(QWidget *parent = 0);
     ~MainWindow();
    /*!
      * \brief Permet d'obtenir un pointeur sur le panel série
      * \return Renvoie un pointeur sur le panel série.
      *
      * Utile pour réaliser des connexions avec l'objet Tserial qui est dans le panel série en dehors de la classe MainWindow.
      */
     SerialPannel* GetPannel();
signals:
     /*!
     * \brief Signal qui transporte un point alloué dynamiquement
     * \param point : Coordonnées.
     *
     * Signal utile si l'on veut connecter directement les entiers lus par tserial au display. Il suffit alors de connecter
     * IntLus(int, int) avec le slot SetPoint(int x, int y) et ce signal avec Pointe(QPoint*) du display.
     */
    void Pointchange(QPoint* point);
    /*!
     * \brief Signal émis lors de l'ouverture du moniteur série.
     * \deprecated Signal non émis ni utilisé.
     */
    void StartSerialMonitor();
    /*!
     * \brief Tansporte les données de la table de calibrage.
     * @deprecated Signal DATACalibrage() utilisé à la place (anciennement connecté a Display::GetTableCalibrage())
     */
    void TableCalibrage(QList<int>);
    /*!
     * \brief Transporte les données du calibrage/
     * @deprecated Signal DATACalibrage utilisé à la place (anciennment connecté à Display::GetTempsCalibrage() )
     */
    void TempsCalibrage(QList<int>);
    /*!
     * \brief Tansporte toutes les données de calibrage. (voir format dans Display::GetDATACalibrage(QList<QList <int > >)
     * \param donnee : Les données du calibrage
     *
     * Signal émis lorsque l'utilisateur appuie sur ok dans l'interface de calibrage (voir utilisation de QDialog).@n
     * Signal émis dans le slot StartCalibre()
     */
    void DATACalibrage(QList<QList<int> > donnee);
public slots:
    // Gestion display
    /*!
     * \brief Slot qui démarre une boite DialogSetPoint pour choisir un point à placer.
     */
    void GenererPoint();
    /*!
     * \brief Slot utilsé pour mettre à jour la variable m_point. Emet le signal Pointchange(QPoint* point);
     * \param x
     * \param y
     * @deprecated Non utilisé
     *
     * Utile pour connecter directement tserial au display.
     */
    void SetPoint(int x, int y);

    //Gestion Liaison
    /*!
     * \brief Slot qui ouvre une QMessageBox annonçant la dernière erreur sur le port série.
     *
     * Cette boite de dialogue affiche les erreurs "No device found" et Permission denied. Sinon elle
     * affiche un nombre correspondant à l'erreur. Il faut se référer à la documentation de QSerialPort pour savoir à
     * quoi il correspond.
     */
    void DialogBoxFailOpeningSerial();
    /*!
     * \brief Slot utilisé pour le lancement du moniteur série.
     *
     * Ce slot met à jour l'argument Tserial du serialmonitor pour qu'il puisse se connecter directement au port série.@n
     * Fait appel à TSerialMonitor::setSerial(TSerial* serial)
     */
    void ButtonSerialMonitorPressed();
    /*!
     * \brief Slot qui vérifie le bool m_CalibrageDone.
     *
     * Ce slot ouvre une boite de dialogue indiquant qu'il faut effectuer le calibrage si le booléen est faux.@n
     * Ce slot doit être obligatoirement utilisé pour démarrer la liaison série et le fonctionnement en mode tablette.
     */
    void StartLiaison();

    /*!Connection avec le moniteur série Mode Brut/Double entier */
    /*!
     * \brief Permet de connecter le signal TSerial::BytesLu(QByteArray Bytes) avec TSerialMonitor::afficheBrut(QByteArray Bytes)
     *
     * Ce slot doit être connecté avec le signal TSerialMonitor::SignalBrut().
     */
    void ConnectionAffichageBrut();
    /*!
     * \brief Même fonctionnement que ConnectionAffichageBrut() mais pour les signaux DoubleInt().
     */
    void ConnectionAffichageDoubleInt();

    // Connections méthodes Table/Calcul
    /*!
     * \brief Fonction qui connecte IntLus avec le mode de traitement par table dans le display.
     * \param checked : Booléen émis pas QRadioButton
     *
     * En connectant ce slot au QRadioButton qui correspond à traitement Table on s'assure un changement de mode de traitement sécurisé.
     */
    void ConnectionTraitementTable(bool checked);
    /*!
     * \brief Même fonctionnement que ConnectionTraitementTable() pour le traitement calculatoire.
     * \param checked
     */
    void ConnectionTraitementCalculatoire(bool checked);
    
    //Calibrage
    /*!
     * \brief Lance la boite de dialogue permettant le calibrage. Emet le signal DATACalibrage().
     *
     * Ce slot déconnecte le display de la liaison série et connecte la liaison série à la boite de dialogue. A la fermeture de la boite la liaison série est reconnectée.
     * Cela évite de calculer une position alors que le calibrage n'a pas été fait.
     */
    void StartCalibre();

private :
    TSerialMonitor* m_SerialMonitor ;/*!< Pointeur sur la boite de dialogue du moniteur série*/
    Display* m_display ;
    QLabel* m_zoneTitle ;
    QPushButton* m_clear;
    QPushButton* m_generatePoint ;
    SerialPannel* m_Pannel ;
    QPushButton* m_start;
    QPushButton* m_stop;



    QHBoxLayout* m_UtilBarDisplay;/*!< @deprecated m_GlobalLayout le remplace */
    QVBoxLayout* m_DisplayLayout;/*!< @deprecated m_GlobalLayout le remplace */
    QHBoxLayout* m_Pannel_DisplayLayout;/*!< @deprecated m_GlobalLayout le remplace */
    QGridLayout* m_GlobalLayout;/*!< Layout global en grille de la fenêtre */
    QHBoxLayout* m_StartStopLayout;/*!< @deprecated m_GlobalLayout le remplace */

    //Donnée
    QPoint m_Point; /*!< @deprecated utile seulement si on veut court-circuiter la phase de calcul et afficher directement les entiers lus.*/
    ModeTraitement m_mode; /*!< Stocke le mode de traitement demandé par l'utilisateur (voir ModeTraitement) */

    //Calibrage
    bool m_CalibrageDone; /*!< Booléen qui indique si le calibrage a été fait ou non. A la construction on considère que non*/
    QPushButton* m_Calibrage;

};

#endif // MAINWINDOW_H
