#ifndef TSERIALMONITOR_H
#define TSERIALMONITOR_H

#include <QWidget>
#include<QByteArray>
#include"tserial.h"

/*!
 * @file tserialmonitor.h
 * @author Rimbaud Sébastien
 * @date 22 Mai 2016
 * @brief Fichier contenant la classe moniteur série.
 */


/*!
 * @brief Enumération qui décrit le mode d'affichage choisi.
 * Cette énumération permet de définir le mode actuel. Tout ajout de mode doit se faire
 * avec l'ajout d'un mode dans l'énumération.
 * Par défaut None = 0, soit aucun mode (utile pour les if)
 */
enum Mode {None = 0, Brut, DoubleInt};

namespace Ui {
class TSerialMonitor;
}

/*!
 * \brief Classe qui hérite de QWidget et qui permet l'affichage de données reçues par une liaison série.
 *
 * Ce widget contient un espace texte qui possède une scroll barr quand la surface n'est plus assez grande pour afficher tout ce qui est reçu.
 * Cette classe possède deux autres boutons qu'on peut implémenter pour avoir d'autres modes d'affichage. Pour implémenter un nouveau mode d'affichage il faut
 * créer un slot qui est connecté au signal QAbstractButton::clicked(bool). Ce slot doit envoyer un signal vide qui informe d'une demande de ce mode d'affichage.
 * La MainWindow doit réaliser la connexion entre un signal qui émet la donnée dans le format que l'on veut afficher avec un nouveau slot que vous avez défini et qui
 * affiche la donnée à l'écran.
 * La MainWindow qui réalise cette connexion doit s'assurer de déconnecter les autres modes d'affichage afin de ne pas les empiler tous en même temps.
 * clicked() -> slotDemandeformat -> SignalFOrmat -> MainWindow::SlotConexionSécrusiéAvecTSerialMonitor::Slotd'affichageFormat;
 */
class TSerialMonitor : public QWidget
{
    Q_OBJECT

public:
    /*!
     * \brief Construit la classe. Par défaut aucun mode n'est choisit. L'utilisateur doit en choisir un.
     * \param parent
     */
    explicit TSerialMonitor(QWidget *parent = 0);
    ~TSerialMonitor();
    /*!
     * \brief Met à jour le pointeur internet sur le TSerial. Doit être appelée juste après la création de la fenêtre. Utile pour les connexions.
     * \param serial : Port Série auquel est relié le moniteur.
     */
    void setSerial(TSerial* serial);
    /*!
     * \brief Met à jour le mode du moniteur (voir enum Mode)
     * \param etat
     */
    void SetMode(Mode etat);
    /*!
     * \brief Donne le mode actuel du moniteur.
     * \return
     */
    Mode GetMode() const ;

signals :
    /*!
     * \brief Signal émis lorsque l'utilisateur coche le radioBoutton affichageBrut. Emis par ModeAffichageBrut(bool).
     *
     */
    void SignalBrut(); // Singal émit lorsque la case affichage brut est cochée.
    void SignalDoubleInt(); /*!< Signal émis lorsque la case DoubleINT cochée. Emis par ModeAffichageDoubleINT(bool)*/
public slots :
    /*!
     * \brief Slot qui affiche dans à l'écran un byte bytes.
     * \param bytes : byte à afficher à l'écran.
     *
     * Ce slot doit être connecté avec TSerial::BytesLu(QBytesArray).
     * Ne pas connecter ce slot et afficheDoubleInt(int, int) en même temps.
     */
    void afficheBrut(QByteArray bytes);
    /*!
     * \brief Slot qui affiche à l'écran un doublet d'entier (int1 ; int2).
     * \param x : Premier entier reçu de la liaison.
     * \param y : Second entier reçu de la liaison.
     *
     *
     * Ce slot doit être connecté avec TSerial::IntLus(int, int)
     * Ne pas connecter ce slot et afficheBrut(QByteArray) en même temps.
     *
     */
    void afficheDoubleInt(int x, int y);
    /*!
     * \brief Slot qui émet SignalBrut() quand le radioBoutton auquel il est connecté envoie checked = vrai.
     * \param checked
     */
    void ModeAffichageBrut(bool checked);
    /*!
     * \brief Slot qui émet SignalDoubleInt() quand le radioBoutton auquel il est connecté envoie checked = vrai.
     * \param checked
     */
    void ModeAffichageDoubleINT(bool checked);




private:
    Ui::TSerialMonitor *ui;
    Mode m_Mode ;/*!< Mode d'affichage dans lequel est le moniteur. A la création : None (Aucun mode).*/
    TSerial* m_serial; /*!< Port série auquel est connecté le moniteur. Doit être mis à jour avec setSerial juste après la création du moniteur.*/

};

#endif // TSERIALMONITOR_H
