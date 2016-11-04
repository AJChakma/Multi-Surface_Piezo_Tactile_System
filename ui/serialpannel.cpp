#include "serialpannel.h"
#include "ui_serialpannel.h"



SerialPannel::SerialPannel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialPannel),
    m_parent(parent)
{
    ui->setupUi(this);
    ui->m_ModeTable->setChecked(true); // De base on est en mode Table.
    // Construction
    if(!Initiate()) // Pour une raison quelconque Pas de port série trouvé
    {
        m_Serial = new TSerial();
        ui->m_AskPortName->setText("Unknown ");
        ui->m_DeviceInfo->setText("No device");
        SetStateLabel(fermer);
    }
    else
        // ça a bien marché
    {
        SetInfo();
        ui->m_AskPortName->setText(m_Serial->portName()); // pas de pb si pas d'alloc
        if(m_Serial->isOpen()) SetStateLabel(Open);
        else if (!m_Serial->isOpen()) SetStateLabel(fermer);
        else SetStateLabel();

    }
    // A ce stade le port série et son info sont crées



    //On lie la lecture à la détection d'un truc.
    QObject::connect(m_Serial, SIGNAL(readyRead()), m_Serial, SLOT(lecture()));
    QObject::connect(ui->m_AskPortName, SIGNAL(textEdited(QString)), this, SLOT(DeviceUpdate(QString)));
    QObject::connect(m_Serial, SIGNAL(SerialStateChange(state)), this, SLOT(DeviceStateUpdate(state)));

    //Communication du changement du nom du device :




}

SerialPannel::~SerialPannel()
{
    delete ui;
}

QRadioButton* SerialPannel::GetCalcRadioButton() const
{
    return(ui->m_ModeCalc); // Renvoie le pointeur du radioBouton de mode calculatoire utile pour les connections.
}

QRadioButton* SerialPannel::GetTableRadioButton() const
{
    return(ui->m_ModeTable);
    //Renvoie le pointeur du radio bouton du mode Table.
}

TSerial* SerialPannel::GetSerial()
{
    return (m_Serial);
}

bool SerialPannel::Initiate()
{
    /*Fonction qui créée des QSerialPorts et info selon ceux détectés sur le systême.
     * Si plusieurs sont disponibles elle demande à l'utilisateur lequel choisir.
     * Si la fonction n'arrive pas à créer des QSerialPorts elle renvoie faux
     * il faudra donc les initialiser dans le constructeur à l'ancienne et
     * laisser l'utilisateur mettre à jour le QLabel.
     */

    QList<QSerialPortInfo> AvailablePorts = QSerialPortInfo::availablePorts();
    int lenght = AvailablePorts.length();
    QString PortName ;
    if (lenght > 1)
    {
        //Plusieurs choix on demande à l'utilisateur d'en prendre un
        QStringList ports ;
        qDebug() << "Longueur plus grande que 1";
        for (int i = 0; i< lenght; i++)
        {
            // Construction des items du combo box
            // Forme :" Nom de Port (Description device)"
            QString chaine (AvailablePorts.at(i).portName()) ;
            chaine += " ( " ;
            chaine += AvailablePorts.at(i).description();
            chaine +=" )";
            ports << chaine ;
        }
        // Utilisation d'un QInputDialog (cf documentaion)
        bool ok ;
        PortName = QInputDialog::getItem(this, "Plusieurs ports série détectés", "Choisissez un port série", ports, 0, false, &ok);
        PortName = PortName.split(" (")[0];
        if (ok && !PortName.isEmpty())
        {
            m_Serial = new TSerial(PortName, this);
            m_SerialInfo = new QSerialPortInfo(PortName);




            return(true);
        }
    }
    else if (lenght ==1 )
    {

        m_Serial = new TSerial(AvailablePorts.at(0).portName(), this);
        m_SerialInfo = new QSerialPortInfo(AvailablePorts.at(0).portName());


        return(true);
    }

    return(false);

}

void SerialPannel::SetStateLabel(state etat)
{
    if (etat== Open)
    {
        ui->m_DeviceState->setText("<font color =\"#00FF00\">Open</font>");


    }
    else if (etat == fermer)
    {
        ui->m_DeviceState->setText("<font color=\"#FF0000\">Close</font>");

    }
    else
    {
        // On sait pas trop
        ui->m_DeviceState->setText("<font color=\"#0000FF\">Unknown</font>");

    }
}

QPushButton* SerialPannel::GetSerialMonitorButton() const
{
   return(ui->m_MoniteurSerieButton);
}

QSerialPortInfo* SerialPannel::GetInfo() const
{
    return(m_SerialInfo) ;
}
 // Slots

void SerialPannel::DeviceStateUpdate(state etat)
{
    if(etat==Open && m_Serial->isOpen())
    {
        SetStateLabel(etat);
    }
    if(etat == fermer && !m_Serial->isOpen())
    {
        SetStateLabel(etat);
    }
}

void SerialPannel::DeviceUpdate(QString PortName)
{
    // Mise à jour du port Série.
    m_Serial->setPortName(PortName);
    if(m_SerialInfo)
    {
        // On libère le pointeur si m_SerialInfo existe déjà.
        delete m_SerialInfo;
    }
    m_SerialInfo = new QSerialPortInfo(PortName);
    SetInfo();
}

void SerialPannel::SetInfo()
{
    if(m_SerialInfo)
    {
        ui->m_DeviceInfo->setText(m_SerialInfo->description());



    }
}


