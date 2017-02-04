#ifndef TSERIAL_H
#define TSERIAL_H
#include<QSerialPort>
#include<QIODevice>
#include<QDebug>
#define NOMBRE_BYTE_ 20
#define BUFFER_SIZE 100
/*!
 * \file tserial.h
 * @brief fichier contenant la définition de la liaison série.
 * @author Rimbaud Sébastien
 * @date 22 Mai 2016
 *
 * Requiert :
 * \arg QT += core
 * \arg QT+= serialports
 * \arg QT += widgets
 *
 */

/*!
 * \brief Enumération qui définit l'état du port. Open= Ouvert, fermer = fermé, unknown = inconnu (cas anti-bug=
 */
enum state {Open, fermer, unknown};

/*!
 * \brief La classe TSerial est une surcharge de QSerialPort. Elle se charge de lire les octets, les mettre en forme et les envoyer au reste du programme.
 */
class TSerial : public QSerialPort
{
    Q_OBJECT
public:
    /*!
     * \brief Crée un port série  ayant le nom portName de 57600 Bauds, Pas de parité, un bit de stop, 8 bits de data.
     * \param portName : nom du port à charger.
     * \param parent
     */
    TSerial(QString portName, QWidget* parent = NULL);
    /*!
     * \brief Construit un port série dont le nom reste à définir. 57600 Bauds, Pas de parité, un bit de stop, 8 bits de data.
     */
    TSerial();
    /*!
     * \brief Fonction de lecture des données.
     * @deprecated Non utilisée et non fonctionnelle.
     * @bug Ne fonctionne pas.
     * \return
     */
    QByteArray LireDonnee();

public slots:
    /*!
     * \brief Fonction qui lit les données et émet deux signaux. Un pour les données lues sous forme de bytes et un pour les données décryptées sous forme de 2 entiers.
     * @bug Sous windows et linux cette fonction ne fonctionne pas de la même façon. Il faut commenter le while(QSerialPort::waitForBytesWritten(-1) && bytesAvailable() >20 car windows crash. Sous Linux il vaut mieux le laisser.
     *
     * Cette fonction est un slot et est appelée dès que le signal QIODevice::readyRead() est émis. ELle doit être utilisée de concert avec ce signal. Elle lit 10 octets, les découpe en 2 entiers et
     * émet les octets lus ainsi que les entiers sous forme de signaux.
     */
    void lecture();

    /*!
     * \brief Ouvrir le port série en vérifiant qu'il n'y a pas d'erreur. Controle les erreurs et informe le programme du changement d'état du port.
     *
     * Cette fonction ouvre le port s'il ne l'est pas déjà et vérifie si l'ouverture a fonctionné. Si elle a fonctionné, il émet Ouverture() et SerialStateChange(open).
     * Si cela n'a pas marché il émet echecOuverture().
     */
    void Ouvrir();
    /*!
     * \brief ferme le port série et émet : SerialStateChange(ferme) et Fermeture().
     */
    void ferme();
    void countlecture();

signals :
    void BytesLu(QByteArray Bytes);/*!< Transporte les octets lus*/
    void IntLus(int a, int b); /*!< Transporte les entiers extraits des octets lus*/
    void echecOuverture();/*!< Emis par Ouvrir() */
    void Ouverture();/*!< Emis par Ouvrir() */
    void Fermeture();/*!< Emis par ferme() */
    void SerialStateChange(state etat);/*!< Emis par Ouvrir() et ferme() */




private :
    QWidget* m_parent ;
    int m_count ;
};



//traitement de la donnée émise par arduino.
/*!
 * \brief Supprime tous les éléments précédents le premier séparateur sep croisé dans la chaine et renvoie la nouvelle liste.
 * \param liste
 * \param sep
 * \return Renvoie une liste dont tous les éléments avant le caractère sep ont été supprimés.
 */
QByteArray nettoyage(QByteArray liste ,char sep) ;
/*!
 * \brief Extrait des entiers sur un format de chaine particulier voir détail. Modifie Liste.
 * \param liste : Extrait les entiers de liste et la modifie.
 * \return Renvoie les entiers extraits.
 * @bug Fonction pas stable. Ne marche pas toujours.
 * @deprecated Non utilisée dans le programme.
 *
 * Extrait les entiers\n
 * Les données ont un format particulier :\n
 *
 * @p 's'RangDonnée\nRangDonnée'e'...
 *
 * Le rang constitue le numéro de la donnée (son ordre d'émission)
 * afin de ne pas mélanger les coordonnées x et y.
 * Le rang sera codé sur 1 char. Soit 10 valeurs de 0 à 9.
 */

QList<int> extractionOrdonnee(QByteArray* liste);
/*!
 * \brief extrait l'entier présent sous forme de chaine de caractères jusqu'au séparateur. Modifie la liste.
 * \param liste
 * \param sep
 * \return Entier contenu jusqu'au premier séparateur.
 * @deprecated Non utilisé
 *
 * ex : "23487\n345433" renvoie 23487 et la liste devient \n34533.
 */
int extraireData(QByteArray liste, char sep = '\n');
/*!
 * \brief Extrait le rang de la donnée (le premier caractère est extrait et renvoyé). Modifie la liste.
 * \param liste : liste dont on doit extraire le rang.
 * \return le premier caractère converti en entier.
 *
 * ex : "1435\3408" renvoie : 1 et la chaine vaut "435\3408".
 */
int extraireRang(QByteArray* liste);
/*!
 * \brief Supprime tous les caractères sep présents au début de la liste. Modifie la liste.
 * \param liste : liste dont on doit supprimer le sep.
 * \param sep : le séparateur sous forme d'un char.
 * @deprecated Non utilisé.
 */
void Supprimesep(QByteArray* liste, char sep = '\n');
/*!
 * \brief lit le buffer tant que la trame de commence pas par \s et ne finit pas par \e. Ne marche pas bien.
 * \param serie : poiteur sur le port série.
 * \return Renvoie la trame lue.
 * @deprecated Non utilisée et instable.
 * @bug Fonction pas stable.
 */
QByteArray lectureFormat(TSerial* serie);
/*!
 * \brief Renvoie NOMBRE_BYTE_ octets lu dans le buffer série. NOMBRE_BYTE_ est un define valant 10.
 * \param serie: pointeur sur le port série à lire.
 * \return  : Renvoie les octets lus.
 */
QByteArray lectureBlock(TSerial* serie);
/*!
 * \brief Extrait des entiers dans une trame selon un format simple "<CaractèreRang><4caractères d'entier><CaractèreRang><4caractères d'entier>". Ne fonctionne qu'avec lectureFormar(TSerial*)
 * \param trame : trame dans laquelle on doit extraire les entiers.
 * \return Renvoie un QList<int> de taille 10 dont chaque case correspond au rang d'un entier lu.
 *
 *
 * Elle découpe en 2 trames de taille 5 la trame passée en paramètre. Elle utilise extraireRang(QByteArray*) pour obtenir le rang de l'entier de chacun d'elle et place ensuite l'entier dans une liste
 * dont l'indice de chaque entier correpond à son rang.
 *
 * ATTENTION La taille de 5 est définie dans le code il faut la modifier si NOMBRE_BYTE_ est modifié ou si le format change.
 */
QList<int> extractionBlock(QByteArray trame);
#endif // TSERIAL_H
