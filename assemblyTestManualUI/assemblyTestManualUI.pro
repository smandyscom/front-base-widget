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
    ../bank-view/interfacechannel.cpp \
    ../bank-view/interfaceclient.cpp \
    ../bank-view/utilities.cpp \
    ../bank-view/fronts/frontcommon.cpp \
    ../bank-view/abstractauthreceiver.cpp \
    ../bank-view/fronts/fronttwinfilter2.cpp \
    ../bank-view/fronts/frontaxisparameter.cpp \
    ../bank-view/definitions/definitionauxiliarykeys.cpp \
    ../bank-view/definitions/definitionbasicblocks.cpp \
    ../bank-view/definitions/definitionsbaselayer.cpp \
    ../bank-view/fronts/frontcommonmanual.cpp \
    ../bank-view/fronts/frontcylinderpanel.cpp \
    ../bank-view/fronts/frontsinglefilter.cpp \
    ../bank-view/fronts/frontunitpanel.cpp \
    ../bank-view/adsclient.cpp \
    ../bank-view/junctionbankdatabase.cpp

HEADERS += \
        mainwindow.h \
    ../bank-view/definitions/definitionmanualblock.h \
    ../bank-view/abstractsqltableadpater.h \
    ../bank-view/controllerbase.h \
    ../bank-view/controllermanualmode.h \
    ../bank-view/interfacechannel.h \
    ../bank-view/interfaceclient.h \
    ../bank-view/interfacerequest.h \
    ../bank-view/utilities.h \
    ../bank-view/fronts/frontcommon.h \
    ../bank-view/abstractauthreceiver.h \
    ../bank-view/fronts/fronttwinfilter2.h \
    ../bank-view/fronts/frontaxisparameter.h \
    ../bank-view/definitions/definitionauxiliarykeys.h \
    ../bank-view/definitions/definitionaxisblocks.h \
    ../bank-view/definitions/definitionbasicblocks.h \
    ../bank-view/definitions/definitioncommandblock.h \
    ../bank-view/definitions/definitionmanualblock.h \
    ../bank-view/definitions/definitionsbaselayer.h \
    ../bank-view/fronts/frontcommonmanual.h \
    ../bank-view/fronts/frontcylinderpanel.h \
    ../bank-view/fronts/frontsinglefilter.h \
    ../bank-view/definitions/definitioncylinderblock.h \
    ../bank-view/fronts/frontunitpanel.h \
    ../bank-view/definitions/definitionunitblocks.h \
    ../bank-view/adsclient.h \
    ../bank-view/junctionbankdatabase.h

INCLUDEPATH += ../bank-view/ \
                ../bank-view/definitions/ \
                ../bank-view/fronts/


FORMS += \
        mainwindow.ui \
    ../bank-view/fronts/fronttwinfilter2.ui \
    ../bank-view/fronts/frontaxisparameter.ui \
    ../bank-view/fronts/frontcylinderpanel.ui \
    ../bank-view/fronts/frontsinglefilter.ui \
    ../bank-view/fronts/frontunitpanel.ui

win32: LIBS += -L$$PWD/../../../../../TwinCAT/AdsApi/TcAdsDll/Lib/ -lTcAdsDll

INCLUDEPATH += $$PWD/../../../../../TwinCAT/AdsApi/TcAdsDll/Include
DEPENDPATH += $$PWD/../../../../../TwinCAT/AdsApi/TcAdsDll/Include
