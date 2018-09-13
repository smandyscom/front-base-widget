#-------------------------------------------------
#
# Project created by QtCreator 2018-09-05T09:48:34
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = assembleTestUI
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
INCLUDEPATH += ../bank-view/ \
                ../../qt-tries/try-property-bind/

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    ../bank-view/interfacechannel.cpp \
    ../bank-view/interfaceclient.cpp \
    ../bank-view/junctionbankdatabase.cpp \
    ../bank-view/utilities.cpp \
    ../bank-view/definitionsbaselayer.cpp \
    ../bank-view/controllerbase.cpp \
    ../bank-view/controllermainpanel.cpp \
    ../bank-view/adsclient.cpp \
    ../bank-view/definitionauxiliarykeys.cpp \
    tempcontroller.cpp \
    ../../qt-tries/try-property-bind/commonhelper.cpp \
    ../bank-view/definitionbasicblocks.cpp

HEADERS += \
        mainwindow.h \
    ../bank-view/interfacechannel.h \
    ../bank-view/interfaceclient.h \
    ../bank-view/interfacerequest.h \
    ../bank-view/junctionbankdatabase.h \
    ../bank-view/utilities.h \
    ../bank-view/definitionsbaselayer.h \
    ../bank-view/controllerbase.h \
    ../bank-view/controllermainpanel.h \
    ../bank-view/adsclient.h \
    ../bank-view/definitionauxiliarykeys.h \
    tempcontroller.h \
    ../../qt-tries/try-property-bind/commonhelper.h \
    ../bank-view/definitionunitblocks.h \
    ../bank-view/definitionbasicblocks.h \
    ../bank-view/definitionmaincontrolblock.h \
    ../bank-view/definitionunitblocks.h

FORMS += \
        mainwindow.ui


win32: LIBS += -L$$PWD/../../../../../TwinCAT/AdsApi/TcAdsDll/Lib/ -lTcAdsDll

INCLUDEPATH += $$PWD/../../../../../TwinCAT/AdsApi/TcAdsDll/Include
DEPENDPATH += $$PWD/../../../../../TwinCAT/AdsApi/TcAdsDll/Include

DISTFILES += \
    ../style2.qss
