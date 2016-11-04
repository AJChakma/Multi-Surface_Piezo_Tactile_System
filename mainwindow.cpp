#include "mainwindow.h"




MainWindow::MainWindow(QWidget *parent) : QWidget(parent), m_mode(Table), m_CalibrageDone(false)
{
    m_SerialMonitor= new TSerialMonitor();
    m_Point = QPoint();
    m_display = new Display(this);
    m_clear = new QPushButton("Clear", this);
    m_clear->setToolTip("Supprime tous les points de la surface.");
    m_generatePoint = new QPushButton("Placer point", this);
    m_generatePoint->setToolTip("Place un point sur la surface de taille et coordonnée voulue.");

    m_zoneTitle = new QLabel("Zone d'affichage ", this);
    m_Calibrage = new QPushButton("Calibrer", this);
    // Création serial pannel
    m_Pannel = new SerialPannel(this);

    //Création start stop
    m_start = new QPushButton("START", this);
    m_start->setToolTip("Demarre la tablette. ATTENTION : Vous devez avoir calibré avant.\nPensez à cliquer sur stop avant de fermer l'application.");
    m_start->setFixedHeight(50);
    m_start->setPalette(QPalette(QColor(0, 200, 0)));
    m_stop = new QPushButton("STOP", this);
    m_stop->setFixedHeight(50);

     //Layouts

    m_GlobalLayout = new QGridLayout(this);
    m_StartStopLayout = new QHBoxLayout();

    m_StartStopLayout->addWidget(m_start);
    m_StartStopLayout->addWidget(m_stop);


    m_GlobalLayout->addWidget(m_Pannel, 0, 0, 2, 1);
    m_GlobalLayout->addWidget(m_zoneTitle, 0, 1);
    m_GlobalLayout->addWidget(m_generatePoint, 0, 2);
    m_GlobalLayout->addWidget(m_clear, 0, 3);
    m_GlobalLayout->addWidget(m_display, 1, 1, 2, 3);
    m_GlobalLayout->addLayout(m_StartStopLayout, 4, 0, 1, 4);

    m_GlobalLayout->addWidget(m_Calibrage, 2, 0);




    // Connections
    //Le display
    QObject::connect(m_generatePoint, SIGNAL(pressed()), this, SLOT(GenererPoint()));
    QObject::connect(m_clear, SIGNAL(pressed()), m_display, SLOT(clearBoard()));
    QObject::connect(this, SIGNAL(Pointchange(QPoint*)), m_display, SLOT(Pointe(QPoint*)));

    //Liaison série
    //QObject::connect(m_Pannel->GetSerial(), SIGNAL(IntLus(int,int)), this, SLOT(SetPoint(int,int)));
    QObject::connect(m_start, SIGNAL(pressed()), this, SLOT(StartLiaison()));
    QObject::connect(m_stop, SIGNAL(pressed()), m_Pannel->GetSerial(), SLOT(ferme()));
    QObject::connect(m_Pannel->GetSerialMonitorButton(), SIGNAL(pressed()), this, SLOT(ButtonSerialMonitorPressed()));

    // Connection erreur ouverture
    QObject::connect(m_Pannel->GetSerial(), SIGNAL(echecOuverture()), this, SLOT(DialogBoxFailOpeningSerial()));

    // Connections moniteur série.
    QObject::connect(m_SerialMonitor, SIGNAL(SignalBrut()), this, SLOT(ConnectionAffichageBrut()));
    QObject::connect(m_SerialMonitor, SIGNAL(SignalDoubleInt()), this, SLOT(ConnectionAffichageDoubleInt()));
    /* Le moniteur série émet des signaux selon le mode d'affichage, Mainwindow Récupère ces signaux
     * et effectue les connections et déconnections nécessaire pour avoir un affichage correct sur
     * le moniteur série.
     */

    //Connections mode de traitement
    QObject::connect(m_Pannel->GetSerial(), SIGNAL(IntLus(int,int)), m_display, SLOT(TraitementTable(int,int)));
    // Par défaut on est en mode table.
    QObject::connect(m_Pannel->GetCalcRadioButton(), SIGNAL(clicked(bool)), this, SLOT(ConnectionTraitementCalculatoire(bool)));
    // Connection du mode calculatoire
    QObject::connect(m_Pannel->GetTableRadioButton(), SIGNAL(clicked(bool)), this, SLOT(ConnectionTraitementTable(bool)));
    //Connection avec le mode Table.
    // De base on traite par table.

    //Calibrage :
    QObject::connect(m_Calibrage, SIGNAL(pressed()), this, SLOT(StartCalibre()));
    //QObject::connect(this, SIGNAL(TableCalibrage(QList<int>)), m_display, SLOT(GetTableCalibrage(QList<int>)));
    //QObject::connect(this, SIGNAL(TempsCalibrage(QList<int>)), m_display, SLOT(GetTempsCalibrage(QList<int>)));
    QObject::connect(this, SIGNAL(DATACalibrage(QList<QList<int> >)), m_display, SLOT(GetDATACalibrage(QList<QList<int> >)));

}MainWindow::~MainWindow()
{
    // On ferme la liaison série pour éviter les bugs.
    m_Pannel->GetSerial()->ferme();
}

void MainWindow::GenererPoint()
{
    DialogSetPoint DialogBox (this);
    if(DialogBox.exec()==QDialog::Accepted)
    {m_display->SetData(DialogBox.GetData());}

}
 SerialPannel* MainWindow::GetPannel()
 {
     return(m_Pannel);
 }

//Slots

void MainWindow::SetPoint(int x, int y)
{
    m_Point.setX(x);
    m_Point.setY(y);
    emit Pointchange(&m_Point);
}

void MainWindow::DialogBoxFailOpeningSerial()
{
    int error = m_Pannel->GetSerial()->error();
    QString* message = new QString("Erreur lors de l'ouverture du port série erreur : ") ;
    switch(error)
    {
        case 0 :
            return ;
        case 1 :
            *message += "No Device Found";
            break ;
    case 2 :
        *message += "Permission denied";
        break ;
    case 3 :
        *message += "Message Already opened";
        // Cas modérément utile normalement la fonction ouvrir protège de cette erreur.
        break;
    default :
        QString *errorstr = new QString () ;
        errorstr->setNum(error);
        *message += *errorstr;
        *message += "\nVérifiez la documentation de Qt QSerialPort pour plus d'information.";
        break;
    }
    QMessageBox* MessageBox = new QMessageBox(this);
    MessageBox->setText(*message);
    MessageBox->setIcon(QMessageBox::Warning);
    MessageBox->exec();
}

void MainWindow::ButtonSerialMonitorPressed()
{
    // Slots qui envoie le signal de lancement du moniteur série.

    m_SerialMonitor->setSerial(m_Pannel->GetSerial());
    m_SerialMonitor->show();
}

void MainWindow::ConnectionAffichageBrut()
{
    /* Réalise la connection entre le tserial et le sérialmonitor en s'assurant
     * de déconnecter le précédent état.
     */

    Mode CurrentMode = m_SerialMonitor->GetMode();
    switch(CurrentMode)
    {
    case None :
        QObject::connect(m_Pannel->GetSerial(), SIGNAL(BytesLu(QByteArray)), m_SerialMonitor, SLOT(afficheBrut(QByteArray)));
        m_SerialMonitor->SetMode(Brut);
        break ;
    case Brut :
        break; // Connection déjà existante.
    case DoubleInt :
        QObject::disconnect(m_Pannel->GetSerial(), SIGNAL(IntLus(int,int)), m_SerialMonitor, SLOT(afficheDoubleInt(int,int)));
        m_SerialMonitor->SetMode(Brut);
        QObject::connect(m_Pannel->GetSerial(), SIGNAL(BytesLu(QByteArray)), m_SerialMonitor, SLOT(afficheBrut(QByteArray)));
        break;

    }
}

void MainWindow::ConnectionAffichageDoubleInt()
{
    Mode CurrentMode = m_SerialMonitor->GetMode();
    switch(CurrentMode)
    {
    case None :
        QObject::connect(m_Pannel->GetSerial(), SIGNAL(IntLus(int,int)), m_SerialMonitor, SLOT(afficheDoubleInt(int,int)));
        m_SerialMonitor->SetMode(DoubleInt);
        break;
    case Brut :
        QObject::disconnect(m_Pannel->GetSerial(), SIGNAL(BytesLu(QByteArray)), m_SerialMonitor, SLOT(afficheBrut(QByteArray)));
        QObject::connect(m_Pannel->GetSerial(),SIGNAL(IntLus(int,int)), m_SerialMonitor, SLOT(afficheDoubleInt(int,int)));
        m_SerialMonitor->SetMode(DoubleInt);
        break ;
    case DoubleInt :
        break ;
    }
}

void MainWindow::ConnectionTraitementCalculatoire(bool checked)
{
    if (checked)
    {
        switch(m_mode) // Un switch si de nouveaux modes apparaissent.
        {
        case CalculSysteme :
            break ; // On est déjà dans le bon mode
        case Table :
            QObject::disconnect(m_Pannel->GetSerial(), SIGNAL(IntLus(int,int)), m_display, SLOT(TraitementTable(int,int)));
            m_mode = CalculSysteme;
            QObject::connect(m_Pannel->GetSerial(), SIGNAL(IntLus(int,int)), m_display, SLOT(TraitementCalculatoire(int,int)));
            break;
        }
    }
}

void MainWindow::ConnectionTraitementTable(bool checked)
{
    if(checked)
    {
        switch(m_mode)
        {
        case Table : // On est déjà en mode table (mode par défaut
            break;
        case CalculSysteme :
            QObject::disconnect(m_Pannel->GetSerial(), SIGNAL(IntLus(int,int)), m_display, SLOT(TraitementCalculatoire(int,int)));
            m_mode = Table;
            QObject::connect(m_Pannel->GetSerial(), SIGNAL(IntLus(int,int)), m_display, SLOT(TraitementTable(int,int)));
            break;

        }
    }
}

void MainWindow::StartCalibre()
{
    //Pour le bon fonctionnement du pannel il faut temporairement déconnecter les liaisons série avec le display.
    switch(m_mode)
    {

    case CalculSysteme :
        QObject::disconnect(m_Pannel->GetSerial(), SIGNAL(IntLus(int,int)), m_display, SLOT(TraitementCalculatoire(int,int)));
        break;
    case Table:
        QObject::disconnect(m_Pannel->GetSerial(), SIGNAL(IntLus(int,int)), m_display, SLOT(TraitementTable(int,int)));
        break;
    // Si ni l'un ni l'autre n'est vrai on fait rien

    }

    TDialogCalibrage calib(m_Pannel->GetSerial(),this);
    // Connection de la boite de dialogue à la com série

    if(calib.exec()== QDialog::Accepted)
    {
        QList<QList<int> > DonneeDeCalibrage ;
        DonneeDeCalibrage.append(calib.GetDonnee());
        DonneeDeCalibrage.append(calib.GetTemps());
        emit DATACalibrage(DonneeDeCalibrage);
        //On restaure les connexions.
        switch(m_mode)
        {

        case CalculSysteme :
            QObject::connect(m_Pannel->GetSerial(), SIGNAL(IntLus(int,int)), m_display, SLOT(TraitementCalculatoire(int,int)));
            break;
        case Table:
            QObject::connect(m_Pannel->GetSerial(), SIGNAL(IntLus(int,int)), m_display, SLOT(TraitementTable(int,int)));
            break;
         // Si ni l'un ni l'autre n'est vrai on fait rien
        }
        // LE calibrage est fait
        m_CalibrageDone = true ;
    }
}


void MainWindow::StartLiaison()
{
    //FOnction que l'on appelle lors de l'appuis sur start on vérifie que le calibrage a bien été fait pour pas faire planter la phase de calcul du display.
    if(m_CalibrageDone)
    {
        //Si le calibrage est fait le logiciel va bien fonctionner on autorise la liaison série.
        m_Pannel->GetSerial()->Ouvrir();
    }
    else // Sinon le calibrage n'a pas été fait on ne démarre pas la liaison.
    {
        QMessageBox* Message = new QMessageBox(QMessageBox::Critical, "Calibrage Introuvable",
                                          "Le calibrage n'a pas été effectué, impossible de commencer la liaison, veuillez effectuer le calibrage en appuyant sur le bouton 'calibrage' .");
        Message->exec();
        delete Message;
    }
}
