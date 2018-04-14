#-------------------------------------------------
#
# Project created by QtCreator 2017-11-06T15:51:30
#
#-------------------------------------------------

QT       += core gui sql widgets serialbus serialport concurrent
#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#QT       -= gui

TARGET = bank-view
TEMPLATE = app

#DEFINES += BANKVIEW_LIBRARY

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
    modbussetupwidget.cpp \
    utilities.cpp \
    enumcomboboxdynamic.cpp \
    modbuschannel.cpp \
    modbusserializedclient.cpp \
    baselayerdefinitions.cpp \
    controllermanualmode.cpp \
    basicblocksdefinition.cpp \
    frontmanaualmode.cpp \
    junctionbankdatabase.cpp \
    controllerbanktransfer.cpp \
    tablemodelcommandblock.cpp \
    tablemodelaxis.cpp \
    mainwindow.cpp \
    main.cpp \
    tablemodeliooverride.cpp \
    frontiooverride.cpp \
    frontcontrolpanel.cpp \
    abstractqvariantsqltable.cpp \
    tablemodelcylinder.cpp \
    frontcylinderpanel.cpp \
    frontbanktransfer.cpp \
    frontsinglefilter.cpp \
    fronttwinfilter.cpp \
    controllermainpanel.cpp \
    junctiondatabases1.cpp \
    tablemodelmatslotn.cpp \
    tablemodelunits.cpp \
    tablemodelsignals.cpp

HEADERS += \
    modbussetupwidget.h \
    utilities.h \
    enumcomboboxdynamic.h \
    enumcomboboxgeneric.h \
    modbuschannel.h \
    modbusserializedclient.h \
    controllermanualmode.h \
    frontmanaualmode.h \
    junctionbankdatabase.h \
    controllerbanktransfer.h \
    tablemodelcommandblock.h \
    tablemodelaxis.h \
    mainwindow.h \
    tablemodeliooverride.h \
    frontiooverride.h \
    frontcontrolpanel.h \
    abstractqvariantsqltable.h \
    tablemodelcylinder.h \
    frontcylinderpanel.h \
    frontbanktransfer.h \
    frontsinglefilter.h \
    fronttwinfilter.h \
    controllermainpanel.h \
    definitionaxisblocks.h \
    definitionsbaselayer.h \
    definitionbasicblocks.h \
    definitionunitblocks.h \
    junctiondatabases1.h \
    tablemodelmatslotn.h \
    tablemodelunits.h \
    tablemodelsignals.h \
    definitioncommandblock.h \
    definitioncylinderblock.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    ptpzrn_viewer.ui \
    modbussetupwidget.ui \
    frontmanaualmode.ui \
    mainwindow.ui \
    frontiooverride.ui \
    frontcontrolpanel.ui \
    frontcylinderpanel.ui \
    frontbanktransfer.ui \
    fronttwinfilter.ui \
    frontsinglefilter.ui
