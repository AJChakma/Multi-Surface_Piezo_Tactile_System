#ifndef SERIALPANNEL_H
#define SERIALPANNEL_H

#include <QWidget>
#include<QtSerialPort/QSerialPortInfo>
#include<QInputDialog>
#include<QLineEdit>
#include<QString>
#include<QtSerialPort/QSerialPort>
#include <QDebug>
#include"tserial.h"
#include<QRadioButton>

/**
 * @file serialpannel.h
 * @author Rimbaud Sébastien
 * @date 22 Mai 2016
 * @brief Panel série qui fait le lien entre la communication série et le reste du programme.
 *
 * Requiert :
 * \arg QT+= core
 * \arg QT+= serialports
 * \arg QT+= gui
 * \arg QT+= widgets
 */

/*!
 * @namespace ui
 * @brief Espace de nom défini par Qt designer. Représente l'interface graphique.
 */
namespace Ui {
class SerialPannel;
}

/*!
 * \brief Panel série qui fait le lien entre communication série et le reste du programme.
 *
 * L'ensemble des méthodes publiques get sont utiles surtout aux connexions hors du panel.
 */
class SerialPannel : public QWidget
{
    Q_OBJECT

public:
    /*!
     * \brief Constructeur. Scanne les ports séries et en choisit un par défaut. Ouvre une boite de dialogue s'il y en a plusieurs.
     * \param parent
     */
    explicit SerialPannel(QWidget* parent = 0);
    ~SerialPannel();
    /*!
     * \brief Permet d'obtenir un pointeur sur TSerial* du pannel. Utile pour les connexions.
     * \return Pointeur sur un objet TSerial*
     */
    TSerial* GetSerial();
    /*!
     * \brief Permet d'obtenir un pointeur sur QPushButton* du pannel. Utile pour les connexions.
     * \return Pointeur sur un QPushButton.
     */
    QPushButton* GetSerialMonitorButton() const ;
    /*!
     * \brief Même rôle que les méthodes publiques à l'extension Get du SerialPannel
     * \return
     */
    QRadioButton* GetCalcRadioButton() const ;
    /*!
     * \brief Même rôle que les méthodes publiques à l'extension Get du SerialPannel
     * \return
     */
    QRadioButton* GetTableRadioButton() const;
    /*!
     * \brief Même rôle que les méthodes publiques à l'extension Get du SerialPannel
     * \return
     */
    QSerialPortInfo* GetInfo() const ;

signals :

public slots :
    /*!
     * \brief Change le label qui indique l'état (open, close, unknow). Y ajoute de la couleur.
     * \param etat : Enumération dans Tserial.
     *
     * Il faut absolument connecter ce slot a TSerial::SerialStateChange(state etat) afin d'avoir une bonne mise à jour de l'état dans l'interface utilisateur.
     */
    void DeviceStateUpdate(state etat);
    /*!
     * \brief Slot pour mettre à jour le nom du port série.
     * \param PortName : Sous forme de chaine de caractère.
     *
     * Slot utile quand il est connecté à un QLineEdit. Il détruit et récrée le QSerialPortInfo et appelle setInfo().
     */
    void DeviceUpdate(QString PortName);
    /*!
     * \brief Slot appelé par DeviceStateUpdate(Qstring) il met à jour le label d'information sur le port série.
     * Utilise le QSerialPortInfo crée par le SerialPannel.
     */
    void SetInfo();

private:
    //Functions
    /*!
     * \brief Méthode qui cherche les dispositifs série comme décrit dans le constructeur.
     * \return Renvoie Faux si aucun port n'est trouvé. Vrai sinon.
     */
    bool Initiate(); // Cherche un port série par défaut.
    /*!
     * \brief Met à jour le label d'état de la communication série. Par défaut unknown.
     * \param etat : Enumération de TSerial.
     */
    void SetStateLabel(state etat=unknown) ;
    //Attributes
    Ui::SerialPannel *ui;
    TSerial* m_Serial ;
    QSerialPortInfo* m_SerialInfo ;
    QWidget* m_parent;
};


#endif // SERIALPANNEL_H
