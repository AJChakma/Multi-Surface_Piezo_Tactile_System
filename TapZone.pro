QT += widgets
QT += gui
QT+= core
QT += serialport

HEADERS += \
    display.h \
    mainwindow.h \
    dialogsetpoint.h \
    serialpannel.h \
    tserial.h \
    tserialmonitor.h \
    tdialogcalibrage.h \
    initialisation.h \
    fonction_traitement.h

SOURCES += \
    display.cpp \
    main.cpp \
    mainwindow.cpp \
    dialogsetpoint.cpp \
    serialpannel.cpp \
    tserial.cpp \
    tserialmonitor.cpp \
    tdialogcalibrage.cpp \
    initialisation.c \
    fonction_traitement.c

DISTFILES += \
    Notes

FORMS += \
    serialpannel.ui \
    tserialmonitor.ui \
    tdialogcalibrage.ui
