#-------------------------------------------------
#
# Project created by QtCreator 2017-11-06T15:51:30
#
#-------------------------------------------------

QT       += core gui sql widgets serialbus serialport concurrent
QTPLUGIN += qtvirtualkeyboardplugin
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
    utilities.cpp \
    enumcomboboxdynamic.cpp \
    controllermanualmode.cpp \
    frontmanaualmode.cpp \
    junctionbankdatabase.cpp \
    controllerbanktransfer.cpp \
    mainwindow.cpp \
    main.cpp \
    frontiooverride.cpp \
    frontcontrolpanel.cpp \
    frontbanktransfer.cpp \
    fronttwinfilter.cpp \
    controllermainpanel.cpp \
    tablemodeliooverride.cpp \
    controllermaterialtransfer.cpp \
    frontunitpanel.cpp \
    definitionbasicblocks.cpp \
    definitionsbaselayer.cpp \
    definitionauxiliarykeys.cpp \
    frontconfigurationtransfer.cpp \
    tablemodelcylindervisual.cpp \
    controllersafety.cpp \
    frontsafetypanel.cpp \
    controllerauth.cpp \
    abstractauthreceiver.cpp \
    frontmaterialselection.cpp \
    delegatematerialselector.cpp \
    frontmaterialselectionv2.cpp \
    delegatedataselector.cpp \
    frontreportquerytable.cpp \
    adsclient.cpp \
    interfacechannel.cpp \
    interfaceclient.cpp \
    controllerbase.cpp \
    abstractauthreceiver.cpp \
    adsclient.cpp \
    controllerauth.cpp \
    controllerbanktransfer.cpp \
    controllerbase.cpp \
    controllermainpanel.cpp \
    controllermanualmode.cpp \
    controllermaterialtransfer.cpp \
    controllersafety.cpp \
    definitionauxiliarykeys.cpp \
    definitionbasicblocks.cpp \
    definitionsbaselayer.cpp \
    delegatedataselector.cpp \
    delegatematerialselector.cpp \
    enumcomboboxdynamic.cpp \
    frontbanktransfer.cpp \
    frontcontrolpanel.cpp \
    frontiooverride.cpp \
    frontmanaualmode.cpp \
    frontmaterialselection.cpp \
    frontmaterialselectionv2.cpp \
    frontreportquerytable.cpp \
    frontsafetypanel.cpp \
    frontslot.cpp \
    fronttwinfilter.cpp \
    frontunitpanel.cpp \
    interfacechannel.cpp \
    interfaceclient.cpp \
    junctionbankdatabase.cpp \
    main.cpp \
    mainwindow.cpp \
    tablemodelcylindervisual.cpp \
    tablemodeliooverride.cpp \
    utilities.cpp \
    fronts/frontcommon.cpp \
    helpers/commonhelper.cpp \
    fronts/frontmainpanel.cpp \
    fronts/frontaxisparameter.cpp \
    fronts/fronttwinfilter.cpp \
    fronts/frontcylinderpanel.cpp \
    fronts/frontunitpanel.cpp \
    fronts/frontsinglefilter.cpp \
    fronts/fronttwinfilter2.cpp \
    fronts/frontconfigurationtransfer.cpp \
    fronts/frontcommonmanual.cpp
HEADERS += \
    utilities.h \
    enumcomboboxdynamic.h \
    enumcomboboxgeneric.h \
    controllermanualmode.h \
    frontmanaualmode.h \
    junctionbankdatabase.h \
    controllerbanktransfer.h \
    mainwindow.h \
    frontiooverride.h \
    frontcontrolpanel.h \
    frontcylinderpanel.h \
    frontbanktransfer.h \
    frontsinglefilter.h \
    controllermainpanel.h \
    tablemodeliooverride.h \
    abstractsqltableadpater.h \
    definitionioattributes.h \
    definitionauxiliarykeys.h \
    controllermaterialtransfer.h \
    frontunitpanel.h \
    frontconfigurationtransfer.h \
    tablemodelcylindervisual.h \
    definitionslotblock.h \
    controllersafety.h \
    frontsafetypanel.h \
    controllerauth.h \
    abstractauthreceiver.h \
    frontslot.h \
    frontmaterialselection.h \
    delegatematerialselector.h \
    frontmaterialselectionv2.h \
    delegatedataselector.h \
    frontreportquerytable.h \
    definitionmaterialkeys.h \
    C://TwinCAT/AdsApi/TcAdsDll/Include/TcAdsAPI.h \
    C://TwinCAT/AdsApi/TcAdsDll/Include/TcAdsDef.h \
    adsclient.h \
    interfacechannel.h \
    interfacerequest.h \
    interfaceclient.h \
    controllerbase.h \
    abstractauthreceiver.h \
    abstractsqltableadpater.h \
    adsclient.h \
    controllerauth.h \
    controllerbanktransfer.h \
    controllerbase.h \
    controllermainpanel.h \
    controllermanualmode.h \
    controllermaterialtransfer.h \
    controllersafety.h \
    definitionauxiliarykeys.h \
    definitionaxisblocks.h \
    definitionbasicblocks.h \
    definitioncommandblock.h \
    definitioncylinderblock.h \
    definitionioattributes.h \
    definitionmaterialkeys.h \
    definitionsbaselayer.h \
    definitionsignalblock.h \
    definitionslotblock.h \
    definitionunitblocks.h \
    delegatedataselector.h \
    delegatematerialselector.h \
    enumcomboboxdynamic.h \
    enumcomboboxgeneric.h \
    frontbanktransfer.h \
    frontconfigurationtransfer.h \
    frontcontrolpanel.h \
    frontcylinderpanel.h \
    frontiooverride.h \
    frontmanaualmode.h \
    frontmaterialselection.h \
    frontmaterialselectionv2.h \
    frontreportquerytable.h \
    frontsafetypanel.h \
    frontsinglefilter.h \
    frontslot.h \
    frontunitpanel.h \
    interfacechannel.h \
    interfaceclient.h \
    interfacerequest.h \
    junctionbankdatabase.h \
    mainwindow.h \
    tablemodelcylindervisual.h \
    tablemodeliooverride.h \
    utilities.h \
    fronts/frontcommon.h \
    helpers/commonhelper.h \
    fronts/frontmainpanel.h \
    definitions/definitionmanualblock.h \
    fronts/frontaxisparameter.h \
    fronts/fronttwinfilter.h \
    fronts/frontcylinderpanel.h \
    fronts/frontunitpanel.h \
    fronts/frontsinglefilter.h \
    fronts/fronttwinfilter2.h \
    fronts/frontconfigurationtransfer.h \
    fronts/frontcommonmanual.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    frontmanaualmode.ui \
    mainwindow.ui \
    frontiooverride.ui \
    frontcontrolpanel.ui \
    frontcylinderpanel.ui \
    frontbanktransfer.ui \
    fronttwinfilter.ui \
    frontunitpanel.ui \
    frontsafetypanel.ui \
    frontslot.ui \
    frontmaterialselection.ui \
    frontmaterialselectionv2.ui \
    frontreportquerytable.ui \
    fronts/frontmainpanel.ui \
    fronts/frontaxisparameter.ui \
    fronts/fronttwinfilter.ui \
    fronts/frontcylinderpanel.ui \
    fronts/frontunitpanel.ui \
    fronts/frontsinglefilter.ui \
    fronts/fronttwinfilter2.ui \
    fronts/frontconfigurationtransfer.ui

INCLUDEPATH += C://TwinCAT/AdsApi/TcAdsDll/Include/ \
    fronts/ \
    definitions/
LIBS += -L "C://TwinCAT/AdsApi/TcAdsDll/Lib/" -lTcAdsDll
