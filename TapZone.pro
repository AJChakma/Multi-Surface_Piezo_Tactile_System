QT += widgets
QT += gui
QT+= core
QT += serialport

INCLUDEPATH= ./incld/
HEADERS += \
    incld/display.h \
    incld/mainwindow.h \
    incld/dialogsetpoint.h \
    incld/serialpannel.h \
    incld/tserial.h \
    incld/tserialmonitor.h \
    incld/tdialogcalibrage.h \
    incld/initialisation.h \
    incld/fonction_traitement.h

SOURCES += \
    src/display.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/dialogsetpoint.cpp \
    src/serialpannel.cpp \
    src/tserial.cpp \
    src/tserialmonitor.cpp \
    src/tdialogcalibrage.cpp \
    src/initialisation.c \
    src/fonction_traitement.c

DISTFILES += \
    Notes

FORMS += \
    src/ui/serialpannel.ui \
    src/ui/tserialmonitor.ui \
    src/ui/tdialogcalibrage.ui
