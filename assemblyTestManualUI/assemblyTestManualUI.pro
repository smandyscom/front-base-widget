#-------------------------------------------------
#
# Project created by QtCreator 2018-09-13T15:06:08
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = assemblyTestManualUI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    ../bank-view/controllerbase.cpp \
    ../bank-view/controllermanualmode.cpp \
    ../bank-view/definitionauxiliarykeys.cpp \
    ../bank-view/definitionbasicblocks.cpp \
    ../bank-view/definitionsbaselayer.cpp \
    ../bank-view/interfacechannel.cpp \
    ../bank-view/interfaceclient.cpp \
    ../bank-view/utilities.cpp

HEADERS += \
        mainwindow.h \
    ../bank-view/definitions/definitionmanualblock.h \
    ../bank-view/abstractsqltableadpater.h \
    ../bank-view/controllerbase.h \
    ../bank-view/controllermanualmode.h \
    ../bank-view/definitionauxiliarykeys.h \
    ../bank-view/definitionbasicblocks.h \
    ../bank-view/definitionsbaselayer.h \
    ../bank-view/interfacechannel.h \
    ../bank-view/interfaceclient.h \
    ../bank-view/interfacerequest.h \
    ../bank-view/utilities.h

INCLUDEPATH += ../bank-view/ \
                ../bank-view/definitions/ \


FORMS += \
        mainwindow.ui
