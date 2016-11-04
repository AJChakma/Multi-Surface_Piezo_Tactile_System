#include "tdialogcalibrage.h"
#include "ui_tdialogcalibrage.h"



TDialogCalibrage::TDialogCalibrage(TSerial* serie, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TDialogCalibrage),
    m_serie(serie)
{
    ui->setupUi(this);
    //Paramètrage combo box

    ui->m_ComboBoxLosange->addItem("Taille Minimale");
    ui->m_ComboBoxLosange->addItem("Taille Moyenne");
    ui->m_ComboBoxLosange->addItem("Taille Maximale");
    ui->m_ComboBoxLosange->addItem("Taille Personnalisée");


    // Résolution est fixée !!
    ui->m_LineEditPrecision->setText("10");

    // Les line edit sont non modifiable par défaut et en mode mini

    ui->m_LineeditDistance1->setEnabled(false);
    ui->m_LineEditDistance2->setEnabled(false);
    ui->m_LineEditPrecision->setEnabled(false);

    // Mise à disable du bouton ok :
    int i =0; // compteur
    QList<QAbstractButton*> Boite_boutons = ui->buttonBox->buttons();
    for(i=0; i < 2; i++)
    {
        if (ui->buttonBox->buttonRole(Boite_boutons[i])==QDialogButtonBox::AcceptRole)
        {
            m_ok = Boite_boutons[i];
            m_ok->setEnabled(false); // Disable pour ne pas envoyer de donnée nulle au display ce qui provoquerait des bugs de calculs.
        }
    }


    // Création Des données.

     ModeMini(); // On met à jour le combo box en Mini et la donnée[0] en Mini
     m_Donnee.append(MINI); // Dist N/S
     m_Donnee.append(MINI); // E/O

     // Création de la liste des temps et des vérifications de mises à jour.
     for(i =0; i < 4 ; i++)
     {
         m_Temps.append(0);
         m_MisAJour.append(false);
     }
    // Pour éviter tout problème de base on ferme la liaison série.
    m_serie->ferme();
    m_serie->Ouvrir();

    // COOOONNEEEEXIIIIOOOOOONNNN


    //Autres
    QObject::connect(ui->m_ComboBoxLosange, SIGNAL(activated(QString)), this, SLOT(Mode(QString)));


    //Mise à jour des données.
    QObject::connect(ui->m_LineeditDistance1, SIGNAL(textChanged(QString)), this, SLOT(UpdateDNS(QString)));
    QObject::connect(ui->m_LineEditDistance2, SIGNAL(textChanged(QString)), this, SLOT(UpdateDEO(QString)));
    /* Important d'utiliser textChanged au lieu de textEdited comme signaux car textChanged tiens compte d'un changement
     * fait automatiquement par un programme contrairement à textEdited
     */


    //Connection des boutons
    //Nord
    QObject::connect(ui->m_ButtonN, SIGNAL(pressed()), this, SLOT(StartNord()));
    QObject::connect(ui->m_ButtonVN, SIGNAL(pressed()), this, SLOT(StopNord()));
    //Sud
    QObject::connect(ui->m_ButtonS, SIGNAL(pressed()), this, SLOT(StartSud()));
    QObject::connect(ui->m_ButtonVS, SIGNAL(pressed()), this, SLOT(StopSud()));
    //Est
    QObject::connect(ui->m_ButtonE, SIGNAL(pressed()), this, SLOT(StartEst()));
    QObject::connect(ui->m_ButtonVE, SIGNAL(pressed()), this, SLOT(StopEst()));
    //Ouest
    QObject::connect(ui->m_ButtonO, SIGNAL(pressed()), this, SLOT(StartOuest()));
    QObject::connect(ui->m_ButtonVO, SIGNAL(pressed()), this, SLOT(StopOuest()));

    // Connection Vérification de mise à jour
    QObject::connect(ui->m_LineEditN1, SIGNAL(textChanged(QString)), this, SLOT(NordMisAjour(QString)));
    QObject::connect(ui->m_LineEditTS1, SIGNAL(textChanged(QString)), this, SLOT(SudMisAjour(QString)));
    QObject::connect(ui->m_LineEditE1, SIGNAL(textChanged(QString)), this, SLOT(EstMisAjour(QString)));
    QObject::connect(ui->m_LineEditTO1, SIGNAL(textChanged(QString)), this, SLOT(OuestMisAjour(QString)));



}

TDialogCalibrage::~TDialogCalibrage()
{
    delete ui;
    m_serie->ferme();
    //Fermeture à la fermeture de la boite de dialogue.
}

/*Distance Nord/Sud
 *         Est/Ouest
 *         tableau de temps
 *         Touest-Test ==> T0 et T2
 *         Tnord-Tsud ==> T1, T3
 * A chaque temps correspond un tap.
 */



QList<int> TDialogCalibrage::GetDonnee() const
{
    return(m_Donnee);
}
QList<int> TDialogCalibrage::GetTemps() const
{
    return(m_Temps);
}
//Slots

void TDialogCalibrage::Mode(QString Mode)
{
      /* Fonction intermédiaire pour rediriger vers
       * les mode Mini, moyen large
       */
    if(Mode == "Taille Minimale")
    {
        ModeMini();
    }
    else if (Mode== "Taille Moyenne")
    {
        ModeMoyen();
    }
    else if (Mode=="Taille Maximale")
    {
        ModeGrand();
    }
    else if (Mode == "Taille Personnalisée")
    {
        ModePerso();
    }
}

void TDialogCalibrage::UpdateDEO(QString d)
{

    d = d.split(" ")[0]; // Suppression de l'unité

    m_Donnee[2] = d.toInt();
    // m_Donnee[2] car on l'a crée dans le constructeur avec 300 par défaut.
}

void TDialogCalibrage::UpdateDNS(QString d)
{

    d = d.split(" ")[0]; // Suppression unité

    m_Donnee[1] = d.toInt() ;
}



// Config Nord
void TDialogCalibrage::UpdateLineEditN(int OE, int NS)
{
    //On considère que le a est la donnée intéressante.
    ui->m_LineEditN1->setText(QString::number(OE));
    ui->m_LineEditN2->setText(QString::number(NS));

    m_Temps[N] = NS;


}

void TDialogCalibrage::StartNord()
{
    // On mets les champs en 'clair' ainsi que valider
    ui->m_LineEditN1->setDisabled(false);
    ui->m_LineEditN2->setDisabled(false);
    ui->m_ButtonVN->setDisabled(false);
    //On désactive les autres start :
    ui->m_ButtonE->setDisabled(true);
    ui->m_ButtonS->setDisabled(true);
    ui->m_ButtonO->setDisabled(true);

    //Connection au sérial
    QObject::connect(m_serie, SIGNAL(IntLus(int, int)),this, SLOT(UpdateLineEditN(int,int)));
    //Démarrage liaison série.


}

void TDialogCalibrage::NordMisAjour(QString texte)
{
    if(!texte.isEmpty()) CheckMisAJour(N);

}

void TDialogCalibrage::StopNord()
{
    // On mets les champs en 'clair' et le valider en sombre
    ui->m_LineEditN1->setDisabled(true);
    ui->m_LineEditN2->setDisabled(true);
    ui->m_ButtonVN->setDisabled(true);
    //On réactive les boutons start :
    ui->m_ButtonE->setDisabled(false);
    ui->m_ButtonS->setDisabled(false);
    ui->m_ButtonO->setDisabled(false);

    //Connection au sérial
    QObject::disconnect(m_serie, SIGNAL(IntLus(int, int)),this, SLOT(UpdateLineEditN(int,int)));
}

//Condif Sud

void TDialogCalibrage::UpdateLineEditS(int OE, int NS)
{
    //On considère que le a est la donnée intéressante.
    ui->m_LineEditTS1->setText(QString::number(OE));
    ui->m_LineEditTS2->setText(QString::number(NS));

    m_Temps[S] = NS;


}

void TDialogCalibrage::StartSud()
{
    // On mets les champs en 'clair'
    ui->m_LineEditTS1->setDisabled(false);
    ui->m_LineEditTS2->setDisabled(false);
    ui->m_ButtonVS->setDisabled(false);
    // autre valider
    ui->m_ButtonE->setDisabled(true);
    ui->m_ButtonN->setDisabled(true);
    ui->m_ButtonO->setDisabled(true);

    //Connection au sérial
    QObject::connect(m_serie, SIGNAL(IntLus(int, int)),this, SLOT(UpdateLineEditS(int,int)));


}

void TDialogCalibrage::SudMisAjour(QString texte)
{
    if(!texte.isEmpty() ) CheckMisAJour(S);
}

void TDialogCalibrage::StopSud()
{
    // On mets les champs en 'clair'
    ui->m_LineEditTS1->setDisabled(true);
    ui->m_LineEditTS2->setDisabled(true);
    ui->m_ButtonVS->setDisabled(true);
    //réactive les start
    ui->m_ButtonE->setDisabled(false);
    ui->m_ButtonN->setDisabled(false);
    ui->m_ButtonO->setDisabled(false);

    //Connection au sérial
    QObject::disconnect(m_serie, SIGNAL(IntLus(int, int)),this, SLOT(UpdateLineEditS(int,int)));
}

//Condif Est

void TDialogCalibrage::UpdateLineEditE(int OE, int NS)
{
    //On considère que le a est la donnée intéressante.
    ui->m_LineEditE1->setText(QString::number(OE));
    ui->m_LineEditTE2->setText(QString::number(NS));

    m_Temps[E] = OE;


}

void TDialogCalibrage::StartEst()
{
    // On mets les champs en 'clair'
    ui->m_LineEditE1->setDisabled(false);
    ui->m_LineEditTE2->setDisabled(false);
    ui->m_ButtonVE->setDisabled(false);
    //On désactive les autres start
    ui->m_ButtonN->setDisabled(true);
    ui->m_ButtonS->setDisabled(true);
    ui->m_ButtonO->setDisabled(true);

    //Connection au sérial
    QObject::connect(m_serie, SIGNAL(IntLus(int, int)),this, SLOT(UpdateLineEditE(int,int)));


}

void TDialogCalibrage::EstMisAjour(QString texte)
{
    if(!texte.isEmpty() ) CheckMisAJour(E);
}

void TDialogCalibrage::StopEst()
{
    // On mets les champs en 'clair'
    ui->m_LineEditE1->setDisabled(true);
    ui->m_LineEditTE2->setDisabled(true);
    ui->m_ButtonVE->setDisabled(true);
    //Réactivation
    ui->m_ButtonN->setDisabled(false);
    ui->m_ButtonS->setDisabled(false);
    ui->m_ButtonO->setDisabled(false);

    //Connection au sérial
    QObject::disconnect(m_serie, SIGNAL(IntLus(int, int)),this, SLOT(UpdateLineEditE(int,int)));
}

//Condif Ouest

void TDialogCalibrage::UpdateLineEditO(int OE, int NS)
{
    //On considère que le a est la donnée intéressante.
    ui->m_LineEditTO1->setText(QString::number(OE));
    ui->m_LineEditO2->setText(QString::number(NS));

    m_Temps[O] = OE;


}

void TDialogCalibrage::StartOuest()
{
    // On mets les champs en 'clair'
    ui->m_LineEditTO1->setDisabled(false);
    ui->m_LineEditO2->setDisabled(false);
    ui->m_ButtonVO->setDisabled(false);
    //Désact other start
    ui->m_ButtonE->setDisabled(true);
    ui->m_ButtonS->setDisabled(true);
    ui->m_ButtonN->setDisabled(true);

    //Connection au sérial
    QObject::connect(m_serie, SIGNAL(IntLus(int, int)),this, SLOT(UpdateLineEditO(int,int)));


}

void TDialogCalibrage::OuestMisAjour(QString texte)
{
    if(!texte.isEmpty() ) CheckMisAJour(O);
}

void TDialogCalibrage::StopOuest()
{
    // On mets les champs en 'clair'
    ui->m_LineEditTO1->setDisabled(true);
    ui->m_LineEditO2->setDisabled(true);
    ui->m_ButtonVO->setDisabled(true);
    //Réac start
    ui->m_ButtonE->setDisabled(false);
    ui->m_ButtonS->setDisabled(false);
    ui->m_ButtonN->setDisabled(false);

    //Connection au sérial
    QObject::disconnect(m_serie, SIGNAL(IntLus(int, int)),this, SLOT(UpdateLineEditO(int,int)));
}
//Private

void TDialogCalibrage::ModeGrand()
{
    // Passage en Non enable si jamais on avait Perso avant.
    ui->m_LineeditDistance1->setEnabled(false);
    ui->m_LineEditDistance2->setEnabled(false);

    //On affiche les dimensions.
    ui->m_LineeditDistance1->setText(QString::number(GRAN));
    ui->m_LineEditDistance2->setText(QString::number(GRAN));

    //Mise à jour donnée
    if(m_Donnee.isEmpty()) m_Donnee.append(Grand);// Nécessaire pour pas crash dans le constructeur
    m_Donnee[0] = Grand;
    // Pas de mise à jour de DE/O ni DN/S car elle se fait automatiquement par les signaux (cf constructeur)


}

void TDialogCalibrage::ModeMoyen()
{
    ui->m_LineeditDistance1->setEnabled(false);
    ui->m_LineEditDistance2->setEnabled(false);

    ui->m_LineeditDistance1->setText(QString::number(MOY));
    ui->m_LineEditDistance2->setText(QString::number(MOY));

    //mise à jour donnéee
    if(m_Donnee.isEmpty()) m_Donnee.append(Moyen);
    m_Donnee[0] = Moyen ;
}

void TDialogCalibrage::ModeMini()
{
    ui->m_LineeditDistance1->setEnabled(false);
    ui->m_LineEditDistance2->setEnabled(false);

    ui->m_LineeditDistance1->setText(QString::number(MINI));
    ui->m_LineEditDistance2->setText(QString::number(MINI));

    //mise à jour donnéee
    if(m_Donnee.isEmpty()) m_Donnee.append(Mini);
    m_Donnee[0] = Mini ;
}

void TDialogCalibrage::ModePerso()
{
    /* On active alors les lineEdit pour permettre à l'utilisateur de
     * rentrer des valeurs (en mm);
     */
    ui->m_LineeditDistance1->setEnabled(true);
    ui->m_LineEditDistance2->setEnabled(true);

    //mise à jour donnéee
    m_Donnee[0] = Perso ;
}


void TDialogCalibrage::CheckMisAJour(Capteur PointCardinal)
{
    qDebug() << "CALL : " << PointCardinal ;
    m_MisAJour[PointCardinal] = true;
    for(int i =0; i < 4; i++)
    {
        if(!m_MisAJour[i])
        {
            qDebug() << "Pas de mise a jour !!!!";
            return ; // Si une des points cardinaux n'est pas mis à jour on fait rien
        }
    }
    // Cas ou tout a été mis à jour :
    m_ok->setEnabled(true); // On active le bouton ok.
    qDebug() << "Bouton ok activé !";
}


