#include "dialogsetpoint.h"
// class PointData
#include <QDebug>
#include <QMessageBox>
#include <QColorDialog>


//Constructeurs :

PointData:: PointData () : m_unite (pixels)
{
    m_couleur = new QColor(Qt::red);
    m_point = new QPoint(0,0);
    m_r = 4 ;
    init();
}

PointData::PointData(QPoint point, int r,QColor couleur) : m_r (r), m_unite(pixels)
{
    m_couleur = new QColor(couleur);
    m_point = new QPoint(point);
    init();
}

PointData::PointData(int x, int y, int ra, int r, int g , int b, int a ) : m_r (ra), m_unite(pixels)
{
    m_couleur = new QColor(r,g,b,a);
    m_point =new QPoint(x, y);
    init();
}
 //méthodes :

void PointData::SetBrutalUnit(unit unite)
{
    m_unite = unite ;
}

void PointData::SetCouleur(int r, int g, int b, int a)
{
    m_couleur->setRgb(r, g, b, a);
}

void PointData::SetCouleur(QColor *couleur)
{
    delete m_couleur ;
    m_couleur = couleur ;
}

void PointData::SetPoint(int x, int y)
{
    m_point->setX(x);
    m_point->setY(y);
}

void PointData::SetPoint(QPoint *point)
{
    delete m_point ;
    m_point = point;
}

void PointData::SetRayon(int r)
{
    m_r = r ;
}
void PointData::SetRayon(QString r)
{
    bool v ;
    SetRayon(r.toUInt(&v));
    if (r != "")
        check(&v);
}

QColor PointData::GetCouleur() const
{
    return(*m_couleur);
}

QPoint PointData::GetPoint() const
{
    return(*m_point);
}

int PointData::GetRayon() const
{
    return(m_r);
}
int PointData::GetUnit() const
{return m_unite; }

void PointData::SetScale(int lx1, int ly1, unit unit1, int lx2, int ly2, unit unit2, unit returnUnit)
{
    /*Fonction qui convertit le point d'un repère carthésien vers un autre
     * Elle calcule les coordonnées du point dans le cadre de dimension lx1*ly1
     * dans un cadre de dimension lx2*ly2 par une règle de trois.
     * unit1 précise l'unité des dimensions du premier cadre de pour le second et unit2
     *
     */
    // adaptation de l'unité de point à celle de retour
    SetUnit(returnUnit);
    // Conversion
    QPoint* point = m_point ;
    if (unit1==unit2)
    {
        /*Mêmes unités*/
        float coeffx = (float) (lx2)/lx1;
        float coeffy = (float) (ly2)/ly1;
        if (returnUnit== unit1)
        {
            // Pas besoin de conversion juste une règle de 3

            SetPoint(coeffx*point->x(), coeffy*point->y());
        }
        else
        {
            // On doit convertir vers une autre échelle et dimension.
            if(returnUnit==mm)
            {
                // On a donc des données en pixels à convertir en MM
                SetPoint(convertPtoMMX(coeffx*point->x()),convertPtoMMY( coeffy*point->y()));
            }
            else
            {
                // On a des données en mm
                SetPoint(convertMMtoPX(coeffx*point->x()),convertMMtoPY(coeffy*point->y()));
            }
        }

    }
    else if (unit1==mm)
    {
        // On a dans ce cas unit 2 en pixels
        if(returnUnit == pixels)
        {
            // Faut juste changer l'unité de lx1 et ly1 en pixels
            SetScale(convertMMtoPX(lx1), convertMMtoPY(ly1), pixels, lx2, ly2, unit2, returnUnit);

        }
        else
        {
            // sinon c'est l'inverse faut passer lx2 et ly2 en mm
            SetScale(lx1, ly1, unit1, convertPtoMMX(lx2), convertPtoMMY(ly2), mm, returnUnit);
        }
    }
    else if (unit1==pixels)
    {
        //cas Inverse : unit2 en mm
        if (returnUnit== mm)
        {
            // On converti 1 en milimètres
            SetScale(convertPtoMMX(lx1), convertPtoMMY(ly1), mm, lx2, ly2, unit2, returnUnit);
        }
        else
        {
            //On converti 2 en pixels.
            SetScale(lx1, ly1, unit1, convertMMtoPX(lx2), convertMMtoPY(ly2), pixels, returnUnit);
        }
    }
    else return; // cas de secours si rien d'autre n'a marché.
}
//fonctions de conversion de MM vers pixels et inversement selon les axes X et Y.
int PointData::convertMMtoPX(int Ax)

{
    // converti a (qui est en mm ) en pixels sur l'axe X

    return(qCeil(Ax*PIXELS_PAR_MMX));
}
int PointData::convertMMtoPY(int Ay)

{
    // converti a (qui est en mm ) en pixels sur l'axe X

    return(qCeil(Ay*PIXELS_PAR_MMY));
}
float PointData::convertPtoMMX(int Ax)
{
    // converti a (qui est en pixels) en mm sur X
    return((float)(Ax/PIXELS_PAR_MMX));
}
float PointData::convertPtoMMY(int Ay)
{
    // converti a (qui est en pixels) en mm sur Y
    return((float)(Ay/PIXELS_PAR_MMY));
}

unit PointData::convertStringtoUnit(QString a)
{
    // COnversion d'un type string en type unit.

    if (a=="Milimètres")
    {
        return(mm);
    }
    else if (a== "Pixels")
    {
        return(pixels);
    }
    return(pixels); // Par on revoie pixels
}

void PointData::SetUnit(unit unite)
{
    // mise à jour de l'unité sécurisée et en faisant les conversions qui s'imposent pour rester cohérent
    if (unite == pixels)
    {
        //Passage en pixels
        if (m_unite == mm)
        {
            // De mm à pixels
            m_point->setX(convertMMtoPX(m_point->x()));
            m_point->setY(convertMMtoPY(m_point->y()));
            m_unite = pixels ;

        }
    }
    else if (unite == mm)
    {
        if(m_unite == pixels)
        {
            m_point->setX(convertPtoMMX(m_point->x()));
            m_point->setY(convertPtoMMY(m_point->y()));
            m_unite=mm;
        }
    }
    return;
}

void PointData::SetUnitDialog(QString unite)
{
    /* Change la valeur de l'unité. Fonction utilisée par la boite de dialogue.
     * Il faut pas faire le calcul ici mais juste indiquer dans la structure
     * que la valeur entrée est en mm.
     */


    m_unite = convertStringtoUnit(unite);
}

void PointData::SetxCoord(QString x)
{
    /* met à jour la coordonnée X en vérifiant la validité de la donnée*/
    bool v ;
    m_point->setX(x.toUInt(&v));
    if (x != "")  check(&v);
}
void PointData::SetyCoord(QString y)
{
    /* met à jour la coord y en vérifiant que c'est bien un entier.*/
    bool v ;
    m_point->setY(y.toUInt(&v));
    if (y != "") check(&v);
}

//private
void PointData::check(bool* v)
{
    /* Affiche une boite de dialogue si v est faux.
     * Fonction utile pour informer l'utilisateur que la valeur qu'il veut placer n'est
     * pas valide.
     * Evite les bugs lors de Pointe();
     */
    if (*v == false)
    {
        QMessageBox *info = new QMessageBox ;
        info->setIcon(QMessageBox::Warning);
        info->setWindowTitle("Attention");
        info -> setText("Veuillez entre une valeur entière");
        info -> exec();
        delete info ;
    }
}

void PointData::init()
{
    /* Cette fonction a pour but de calculer le coefficien
     * PIXELS_PAR_MMX/Y, afin d'obtenir les conversions.
     */
    QScreen* screen = QGuiApplication::primaryScreen();
    QSizeF physicalDimension = screen->physicalSize();
    QSize VirtualDimesion = screen->virtualSize();
    PIXELS_PAR_MMX = VirtualDimesion.width()/physicalDimension.rwidth() ;
    PIXELS_PAR_MMY = VirtualDimesion.height()/physicalDimension.rheight();
    // Chaque coeff est en Pixels/mm.


}



// Boite de dialogue
DialogSetPoint::DialogSetPoint(QWidget *parent) : QDialog(parent)
{
    m_getx = new QLineEdit(this); // Champ d'entrée coordonnée X
    m_gety= new QLineEdit(this);// Champ d'entrée coordonnée Y
    m_gettaille = new QLineEdit(this); // champt d'entrée taille
    m_gettaille->setText("4"); // De base une taille de 4.
    // Définition du menu déroulant de choix de l'unité
    m_getunit = new QComboBox(this); // Menu déroulant de choix de l'unité.

    m_getunit->addItem("Pixels");
    m_getunit->addItem("Milimètres");

    m_getunit->setItemText(m_getunit->findText("Pixels"), "Pixels"); // Paramètrage en pixels par défut

    m_valider = new QPushButton("Valider", this);
    m_annuler = new QPushButton("Annuler", this);
    m_Bcouleur= new QPushButton("Choisir couleur", this);

    m_couleur = new QLabel("Couleur");

    // Layouts
    m_formulaire = new QFormLayout();
    m_choixCouleur = new QHBoxLayout();
    m_dialogueForme = new QVBoxLayout(this);


    m_donnee= new PointData();// donnée que l'on transmet

    // Organisation des Layouts
    m_formulaire ->addRow("Coordonnee x ", m_getx);
    m_formulaire ->addRow("Coordonnee y ", m_gety);
    m_formulaire ->addRow("Unité ", m_getunit);
    m_formulaire ->addRow("Rayon point ", m_gettaille);
    m_formulaire->addRow("Choix Couleur", m_Bcouleur);

    m_choixCouleur->addWidget(m_valider);
    m_choixCouleur->addWidget(m_annuler);

    m_dialogueForme->addLayout(m_formulaire);
    m_dialogueForme->addLayout(m_choixCouleur);

    /*COOOONEEEEXIOOOOOOOOOOOOOOOOOON*/


    QObject::connect(m_annuler, SIGNAL(pressed()), this, SLOT(reject()));
    QObject::connect(m_valider, SIGNAL(pressed()), this, SLOT(accept()));
    QObject::connect(m_getx, SIGNAL(textEdited(QString)), m_donnee, SLOT(SetxCoord(QString)));
    QObject::connect(m_gety, SIGNAL(textEdited(QString)), m_donnee, SLOT(SetyCoord(QString)));
    QObject::connect(m_getunit, SIGNAL(activated(QString)), m_donnee, SLOT(SetUnitDialog(QString)));
    QObject::connect(m_gettaille, SIGNAL(textEdited(QString)), m_donnee, SLOT(SetRayon(QString)));
    QObject::connect(m_Bcouleur, SIGNAL(pressed()), this, SLOT(SetCouleur()));

}
PointData* DialogSetPoint::GetData()
{
    return(m_donnee);
}

//Slots
void DialogSetPoint::SetCouleur()
{
    QColor* couleur = new QColor(Qt::red) ;
    *couleur=  QColorDialog::getColor(Qt::red, this, "Choix de la couleur du point");
    m_donnee->SetCouleur(couleur);
}
