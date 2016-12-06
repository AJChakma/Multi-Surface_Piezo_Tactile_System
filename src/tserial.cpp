#include "tserial.h"
#include<QSerialPort>



TSerial::TSerial(QString portName, QWidget *parent) : QSerialPort(portName), m_parent (parent)
{
    setBaudRate(QSerialPort::Baud57600);
    setDataBits(QSerialPort::Data8);
    setStopBits(QSerialPort::OneStop);
    setParity(QSerialPort::NoParity);
    m_count = 0;

}

TSerial::TSerial() : QSerialPort()
{
    setBaudRate(QSerialPort::Baud57600);
    setDataBits(QSerialPort::Data8);
    setStopBits(QSerialPort::OneStop);
    setParity(QSerialPort::NoParity);
    m_count = 0;
}

QByteArray TSerial::LireDonnee()
{
    /* Lit le buffer ligne par ligne
     * D'après notre format, il nous faut au moins 2 lignes pour que ça marche.
     * Buggué
     */
    QByteArray tmp ;
    QByteArray lu ;
    int i = 0;
    while(i  < 2)
    {
        tmp = readLine();
        qDebug() << tmp ;
        if (!tmp.isEmpty())
        {
            // Une ligne a été lue.
            lu.append(tmp);
            lu.append('\n');
        }
    }
    qDebug() << lu ;
    return(lu);
}

//Slots
void TSerial::countlecture()
{
    m_count ++;
    qDebug() << "Appel n°= " << m_count ;
    if(m_count == 5)
    {
        lecture();
        m_count = 0;
    }
}

void TSerial::lecture()
{
    /*
     * fonction de lecture des données séries.
     * Format :
     * \nRangEntier\nRangEntier\nRangEntier.
     * Les entiers seront éventuellements numérotés.
     * De ce fait il se peut qu'ils soient mélangé.
     * On doit trouver un moyen de reconstituer l'ordre les entiers seront émis par paire
     * voire plus nous devons trouver un moyen de numéroter les nombres.
     * Format
     * \nRang\tEntier\nRang\tEntier\n ....
     * le rang est un char (pas de 256 octets à la fois donc.
     */
    //if(!waitForReadyRead(-1)) return ;
    QByteArray bytes ;

    //Lecture
    bytes = lectureBlock(this);
    // Vérification d'avoir au moins 2 entiers.



    emit BytesLu(bytes);//On envoie ce qu'on lit si jamais c'est utilie ailleur.
    //qDebug()<< "Lancement extraction ordonnée";
    qDebug() << bytes ;
    qDebug() << bytes.length();
    qDebug() << bytes.toInt(NULL, 10);
    //traitement des données pour extraire les 2 entiers qui nous interessent.
    QList<int> entiers = extractionBlock(bytes); // On veut 2 entiers.

    // On est sûr d'avoir au moins les 2 entiers sans qu'ils soient coupés.
    // Debug check for
    //qDebug()<< entiers[1] << ";" << entiers[2];
    //qDebug() << "Size :" << entiers.size();
    // Quand on accéde aux paramètres de la liste :
    int a = entiers[1];
    int b = entiers[2];
    emit IntLus(a,b);


}



void TSerial::Ouvrir()
{
    if(!isOpen())
    {
        open(QIODevice::ReadOnly);
        if(isOpen())
        {
            emit Ouverture();
            emit SerialStateChange(Open);
        }
        if(!isOpen())
        {
            emit echecOuverture();
        }
    }

}

void TSerial::ferme()
{
    if(isOpen())
    {
        close();
        if(!isOpen())
        {
        emit Fermeture();
        emit SerialStateChange(fermer);
        }
    }
}



//Other functions
QByteArray nettoyage(QByteArray liste, char sep)
{
    /* Fonction qui supprime tous les éléments précédents
     * la première itération du séparateur (sep)
     * Pas de passage par référence, je verrai après si il y a une bonne raison
     * on si on peut gagner en efficacité.
     * Renvoie les valeurs supprimées.
     */

    while (liste.at(0)!= sep)
    {
        liste.remove(0, 1);
    }
    return(liste);
}

QList<int> extractionOrdonnee(QByteArray *liste)
{
    /* extrait les entiers
     * Les données ont un format particulier :
     * "'s'RangDonnée\nRangDonnée'e'..."
     * Le rang constitue le numéro de la donnée (son ordre d'émission
     * afin de ne pas mélanger les coordonnées x et y.
     * Le rang sera codé sur 1 char. Soit 10 valeurs de 0 à 9.
     */

    QList<int> donnee;

    for(int i = 0 ; i < 10 ; i ++)
    {
        donnee.append(0);
    }

    QByteArray copie = *liste;// On évite de changer les données enregistrées.


    Supprimesep(&copie, 's'); // Format RangDonnee\nRangDonnee'e'
    int rang = extraireRang(&copie);
    donnee[rang]= extraireData(copie);
    copie = nettoyage(copie, '\n');
    Supprimesep(&copie);
    rang = extraireRang(&copie);
    donnee[rang] = extraireData(copie, 'e');
    // En théorie on a lu les données.

    return(donnee);
}

int extraireData(QByteArray liste, char sep)
{
    /* Retourne l'entier précédent le premier \n croisé
     * Format donnée en entrée :
     * Donnée\nRangDonnée....
     * en sortie même format.
     * Cette fonction doit être appelée après extraireRang !
     */
    QByteArray extrait ;
    int compteur = 0 ;
    while(liste[compteur] != sep )
    {
        extrait.append(liste[compteur]);
        compteur++;
    }
    return(extrait.toInt());
}

int extraireRang(QByteArray* liste)
{
    /*fonction qui extrait le rang de la donnée,
     * et la supprime dans l'en tête de la donnée
     * Format donnée en entrée :
     * RangDonnée\n....
     * En sortie :
     * Donnée\nRang....
     * Précède ExtraireData.
     */
    int rang = (liste->at(0)) - '0';
    // Conversion en int
    liste->remove(0, 1);
    return(rang);
}

void Supprimesep(QByteArray *liste, char sep)
{
    // Supprime tous les \n au début de la liste.

    while(liste->at(0) == sep)
    {
        liste->remove(0, 1);
    }
}

QByteArray lectureFormat(TSerial *serie)
{
    /*Lit le buffer tant que la trame ne commence pas par
     * \s et qu'il n'y a pas de \e dans la trame.
     */
    int s =0 ;
    int e = 0;
    QByteArray trame ;
    trame.append(serie->read(13));

    if(trame.contains('s'))
    {
        s= trame.indexOf('s');
    }
    if(trame.contains('e'))
    {
        e = trame.indexOf('e');
    }

    if (e>s) return trame ;
    else qDebug() << "Fail";

}

QByteArray lectureBlock(TSerial *serie)
{
    QByteArray trame ;
    trame.append(serie->read(NOMBRE_BYTE_));
    return(trame);
}

QList<int> extractionBlock(QByteArray trame)
{
    int rang = 0;
    QList<int> entiers ;
    QByteArray blockUn = trame.mid(NOMBRE_BYTE_/2);
    QByteArray blockDeux  = trame.mid(0, NOMBRE_BYTE_/2);
    for (int i = 0; i < 10 ; i++)
    {
        entiers.append(0);
    }

    rang = extraireRang(&blockUn);
    entiers[rang]= blockUn.toInt();
    rang = extraireRang(&blockDeux);
    entiers[rang] = blockDeux.toInt();
    return(entiers);
}
