#-------------------------------------------------
#
# Project created by QtCreator 2015-11-19T17:24:03
#
#-------------------------------------------------

QT       += core gui  opengl  sql  xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = 201512022154
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    about.cpp \
    qcustomplot.cpp \
    fft.cpp \
    diagram.cpp \
    login.cpp \
    password.cpp

HEADERS  += mainwindow.h \
    about.h \
    qcustomplot.h \
    fft.h \
    connection.h \
    diagram.h \
    login.h \
    password.h

FORMS    += mainwindow.ui \
    about.ui \
    diagram.ui \
    login.ui \
    password.ui

RC_FILE += Detector.rc

RESOURCES += \
    menu.qrc


