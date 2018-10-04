#-------------------------------------------------
#
# Project created by QtCreator 2018-10-04T16:38:32
#
#-------------------------------------------------

QT       += sql testlib widgets

QT       -= gui

TARGET = tst_unittestcontrolleriotest
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
        tst_unittestcontrolleriotest.cpp \ 
    ../bank-view/controllers/controlleriomonitor.cpp \
    ../bank-view/definitions/definitionbasicblocks.cpp \
    ../bank-view/controllers/controllerbase.cpp \
    ../bank-view/interface/interfacechannel.cpp \
    ../bank-view/interface/interfaceclient.cpp \
    ../bank-view/base/utilities.cpp \
    ../bank-view/definitions/definitionsbaselayer.cpp

INCLUDEPATH += ../bank-view/controllers/ \
                ../bank-view/interface/ \
                ../bank-view/definitions/ \
                ../bank-view/base/

DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../bank-view/controllers/controlleriomonitor.h \
    ../bank-view/definitions/definitionbasicblocks.h \
    ../bank-view/controllers/controllerbase.h \
    ../bank-view/interface/interfacechannel.h \
    ../bank-view/interface/interfaceclient.h \
    ../bank-view/interface/interfacerequest.h \
    ../bank-view/base/utilities.h \
    ../bank-view/definitions/definitionsbaselayer.h
