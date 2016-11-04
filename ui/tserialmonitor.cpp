#include "tserialmonitor.h"
#include "ui_tserialmonitor.h"

TSerialMonitor::TSerialMonitor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TSerialMonitor),

    m_Mode(None)
{
    ui->setupUi(this);

    ui->m_TextArea->setVerticalScrollBar(ui->m_ScrollBar);
    // Fonctionnel jusqu'ici =)
    QObject::connect(ui->m_RBBrut, SIGNAL(clicked(bool)), this, SLOT(ModeAffichageBrut(bool)));
    QObject::connect(ui->m_RBDoubleInt, SIGNAL(clicked(bool)), this, SLOT(ModeAffichageDoubleINT(bool)));
    // Lorsque les Radiobouttons sont cochés on renvoie un signal qui sera intercepté par la mainWindow.


}

TSerialMonitor::~TSerialMonitor()
{
    delete ui;
}

void TSerialMonitor::setSerial(TSerial *serial)
{
    m_serial = serial ;
}

void TSerialMonitor::SetMode(Mode etat)
{
    m_Mode= etat ;
}

Mode TSerialMonitor::GetMode() const
{
    return(m_Mode);
}

//Slots

void TSerialMonitor::afficheBrut(QByteArray bytes)
{
    for (int i = 0 ; i < bytes.length() ; i ++)
    {
        ui->m_TextArea->append(bytes);
    }
}

void TSerialMonitor::afficheDoubleInt(int x, int y)
{
    QString str ;
    str += "( ";
    str+= QString::number(x);
    str += " ; ";
    str+= QString::number(y) ;
    str += " )\n";
    ui->m_TextArea->append(str);

}
//Il faut créer les slots de tous les radiobuttons
void TSerialMonitor::ModeAffichageBrut(bool checked)
{
    if(checked) emit SignalBrut();
}

void TSerialMonitor::ModeAffichageDoubleINT(bool checked)
{
    if (checked) emit SignalDoubleInt();
}

