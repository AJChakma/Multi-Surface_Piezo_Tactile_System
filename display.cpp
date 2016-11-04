#include "display.h"

// Display class
Display::Display(QWidget *parent) : QWidget(parent)
{
    m_Points = new QVector<QPoint*> (0) ;
    m_len = 0 ;

    m_PointSize = new QVector<int> ();
    m_CurrentPointSize = 4;

    m_fill = new QVector<QColor*> ();
    m_CurrentColor = new QColor(Qt::red);


    m_info = new QMessageBox ;
    m_info->setIcon(QMessageBox::Warning);
    m_info->setWindowTitle("Attention");


    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true); // Ces deux lignes fixent la couleur blanche du widget.


}

Display::~Display()
{


    delete m_fill;
    delete m_info ;

    vider();
    delete m_Points;
}

void Display::Print()
{
    // Permet l'affichage du point sur la tableau blanc
    update();

}

//Protected
void Display::paintEvent(QPaintEvent *)
{ 
    // Surchage de paintEvent pour dessiner dans le Widget. cf doc Qt.
    QPainter painter(this) ;




    for(int i =0 ; i<m_len; i++)
    {


       QColor couleur = getColor(i);

       QPoint point = getPoint(i);

       int r = getSize(i);

       painter.setPen(couleur);
       painter.setBrush(couleur);


       painter.drawEllipse(point,r,r);
    }




}

QSize Display::minimumSizeHint() const
{
    return(QSize(400,400));
    // Donne la taille minimale du widgets dans un layout c'est une surcharge d'une fonction QWdget (cf doc).
}

//Private :
void Display::ajouter(QPoint *point)
{
    /* Ajoute un point et ses données à toutes les listes.
     * La couleur et la taille sont fixés par m_CurrentColor et m_CurrentPointSize
     * Les coordonnées constituent l'arguement de la fonction
     */

    point = new QPoint(*point);
    m_Points->append(point);

    m_fill->append(m_CurrentColor);

    m_PointSize->append(m_CurrentPointSize);
    m_len++;

    //Permet d'ajouter un élément en gérant le compte de point.
}

void Display::vider()
{
    // Vide les listes en faisant les free qui s'imposent.
    for(int i =0 ; i < m_len ; i++)
    {

        m_Points->pop_back();

        m_fill->pop_back();
        m_PointSize->pop_back();
    }

    m_len = 0;
}
// Fonctions de chargement des données :
/*! Un Point affiché à l'écran se définit par son rang i soit un entier variant de
 * 0 à n-1 (n nombre maximums de points enregistré).
 * Avec le rang on peut par ces 3 fonctions retrouver toutes les données d'un point :
 * Sa couleur, sa taille, ses coordonnée (stockées par un QPoint)
 */
QPoint Display::getPoint(int i) const
{
    // Retourne le QPoint au rang i enregistré.


    return(*((*m_Points)[i]));
}

QColor Display::getColor(int i) const
{
    // Retourne la couleur au rang i enregistré.
    return(*((*m_fill)[i]));
}

int Display::getSize(int i) const
{
    //Retourne la taille du point au rang i
    return((*m_PointSize)[i]);
}


// Slots

void Display::Pointe(int x, int y)
{
    // Surcharge avec 2 entiers au lieu d'un QPoint.
    QPoint* newPoint = new QPoint(x, y); // Ce point sera libéré lorsqu'on effacera le tabler avec clearBoard.
    return(Pointe(newPoint));
}

void Display::Pointe( QPoint* point)
{
    /*! Ajoute le point aux données du l'objet Display en vérifiant que celui-ci est bien visible dans le widget
     * Le cas échéant une boite de dialogue s'ouvre et informe l'utilisateur qu'un point est hors de l'écran
     * et qu'il faut redimensionner le widget.
     */
    if (point->rx() >= width() || point->ry()>= height())
    {
        RiseMessage("Attention la zone d'affichage est trop petite\npour afficher tous les points !\n"
                    "Veuillez l'agrandir si possible.");
    }

    ajouter(point);// ajoute le point aux données.

    update();// Fait appel à PaintEvent.

}

void Display::SetCouleur(const QColor &couleur)
{
    //Fixe la couleur courante.
    *m_CurrentColor = couleur;
}

void Display::SetCouleur(int r, int g, int b, int a)
{
    // Surcharge de la précédente.
    m_CurrentColor->setRgb(r,g,b, a);
}

void Display::RiseMessage(const QString &message, const QString &mode)
{
    /*Fonction globale pour afficher un message de tout type, faire appel à cette fonction pour des boites de
     * dialogues de type informatif (sans questionnaire ou demande de donnée à l'utilisateur)
     */
    m_info->setText(message);
    if (mode == "Warning" )
        m_info->setIcon(QMessageBox::Warning);
    else if (mode == "Information")
        m_info->setIcon(QMessageBox::Information);
    else if(mode == "Critical" )
        m_info ->setIcon(QMessageBox::Critical);
    m_info->exec();


}

void Display::clearBoard()
{
    vider();
    Print();
}

void Display::SetSize(const QSize &size)
{
    resize(size);
}

void Display::SetSize(int x, int y)
{
    resize(x, y);
}

void Display::SetPointSize(int t)
{
    m_CurrentPointSize = t ;
}

void Display::SetData(PointData* donnee)
{
    /*! Fonction qui enregistre un point dans les listes, point sur la forme PointData */
    donnee->SetUnit(pixels);
    SetPointSize(donnee->GetRayon());
    SetCouleur(donnee->GetCouleur());
    QPoint point = donnee->GetPoint();
    Pointe(&point);
}


/*!Slots traitement*/

void Display::TraitementTable(int dtOE, int dtNS)
{
    /*! Fonction qui traite les deux entier reçus et calcule les coordonnées par le biais
     * d'une table de coordonnée
     */
    //int a = dtOE , b = dtNS ;
    m_vitesse = 3000000;
    COORD coordonnee = rechercheglobale(m_Table, m_DonneeCalib[2], m_DonneeCalib[1], m_vitesse, dtOE, dtNS, 0.001);
    // Coordonnee en cm.
    //Traitement(a,b) voir si la fonction modifie a et b.
    coordonnee.x *= 10 ; // Passage en mm
    coordonnee.y *= 10;
    qDebug() << "Coordonnées :  ( " << coordonnee.x << "; " << coordonnee.y << ")";
    PointData point;
    point.SetPoint(abs(point.convertMMtoPX(coordonnee.x)), abs(point.convertMMtoPY(coordonnee.y)));


    SetData(&point); // On met à jour et on affiche le point.

}

void Display::TraitementCalculatoire(int x, int y)
{
    /*! Fonction qui traite les deux entier reçus et calcule les coordonnées par une
     * résolution d'un systême non linéaire
     * (Implémentation non prévu le 21 Avril 2016).
     */
    int a = x , b = y ;
    //Traitement(a,b) voir si la fonction modifie a et b.
    Pointe(a, b); // On fait appel à pointe.
}


void Display::GetTableCalibrage(QList<int> data)
{
//Obsolète
}

void Display::GetTempsCalibrage(QList<int> data)
{
//obsolète
}

void Display::GetDATACalibrage(QList<QList<int> > data)
{
    m_DonneeCalib = data[0];
    m_Temps = data[1];
    qDebug()<< "temps" << m_Temps ;

    //Construction liste d'entier.
    int Temps[4];
    for(int i =0 ; i <4 ; i++)
    {
        Temps[i] = m_Temps[i];
    }

    m_vitesse= calcul_vitesse(m_DonneeCalib[2], m_DonneeCalib[1], Temps);

    switch(m_DonneeCalib[0])
    {
    case Perso :
        initialisation_fichiers(AUCUN,m_DonneeCalib[2], m_DonneeCalib[1], Temps);
        break;
    case Mini :
        initialisation_fichiers(PETIT,m_DonneeCalib[2], m_DonneeCalib[1], Temps);
        break;
    case Moyen :
        initialisation_fichiers(MOYEN,m_DonneeCalib[2], m_DonneeCalib[1], Temps);
        break;
    case Grand :
        initialisation_fichiers(GRAND,m_DonneeCalib[2], m_DonneeCalib[1], Temps);
        break;

    }


    switch(m_DonneeCalib[0])
    {
    case AUCUN :
        m_Table= lecturetab(const_cast<char*> ("losangeredefini.txt"),  m_DonneeCalib[2]*m_DonneeCalib[1]);
        break;
    case PETIT :
         m_Table= lecturetab(const_cast<char*>("petitlosange.txt"), m_DonneeCalib[2]*m_DonneeCalib[1]);
        break;
    case MOYEN:
        m_Table= lecturetab(const_cast<char*>("moyenlosange.txt"), m_DonneeCalib[2]*m_DonneeCalib[1]);
        break;
    case GRAND :
        m_Table= lecturetab(const_cast<char*> ("grandlosange.txt"), m_DonneeCalib[2]*m_DonneeCalib[1]);
        break;
    }



}
