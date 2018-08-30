#-------------------------------------------------
#
# Project created by QtCreator 2018-08-23T10:05:28
#
#-------------------------------------------------

QT       += widgets sql testlib concurrent serialbus

QT       -= gui

TARGET = tst_unittestertest
CONFIG   += console
CONFIG   -= app_bundle

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
        tst_unittestertest.cpp \ 
    ../bank-view/adsclient.cpp \
    ../bank-view/interfaceclient.cpp \
    ../bank-view/utilities.cpp \
    tst_unittestinterfacechannel.cpp \
    ../bank-view/interfacechannel.cpp \
    ../bank-view/definitionsbaselayer.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../bank-view/adsclient.h \
    ../bank-view/interfaceclient.h \
    ../bank-view/interfacerequest.h \
    ../bank-view/utilities.h \
    ../bank-view/interfacechannel.h \
    ../bank-view/definitionsbaselayer.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../TwinCAT/AdsApi/TcAdsDll/Lib/ -lTcAdsDll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../TwinCAT/AdsApi/TcAdsDll/Lib/ -lTcAdsDll

INCLUDEPATH += $$PWD/../../../../../TwinCAT/AdsApi/TcAdsDll/Include
DEPENDPATH += $$PWD/../../../../../TwinCAT/AdsApi/TcAdsDll/Include

INCLUDEPATH += ../bank-view/
