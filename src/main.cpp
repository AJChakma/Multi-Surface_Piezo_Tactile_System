#include "display.h"
#include"dialogsetpoint.h"
#include"mainwindow.h"
#include"serialpannel.h"
#include"tserialmonitor.h"
#include"initialisation.h"
#include"fonction_traitement.h"
#include<QApplication>
#include<QGuiApplication>
#include<QDebug>
#include<QPushButton>
#include<QVBoxLayout>
#include"tserial.h"
#include"tdialogcalibrage.h"
#include<QDebug>


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    MainWindow root;
    root.show();

    return a.exec() ;
}


