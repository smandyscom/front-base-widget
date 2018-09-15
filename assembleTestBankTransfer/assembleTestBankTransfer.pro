#-------------------------------------------------
#
# Project created by QtCreator 2018-09-15T22:18:00
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = assembleTestBankTransfer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    ../bank-view/controllerbanktransfer.cpp \
    ../bank-view/controllerbase.cpp \
    ../bank-view/controllermanualmode.cpp

HEADERS += \
        mainwindow.h \
    ../bank-view/controllerbanktransfer.h \
    ../bank-view/controllerbase.h \
    ../bank-view/definitionunitblocks.h \
    ../bank-view/definitions/definitionmanualblock.h \
    ../bank-view/controllermanualmode.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += ../bank-view/definitions

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
