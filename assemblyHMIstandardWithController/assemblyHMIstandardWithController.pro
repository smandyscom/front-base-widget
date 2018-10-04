#-------------------------------------------------
#
# Project created by QtCreator 2018-09-21T22:24:59
#
#-------------------------------------------------

QT       += core gui widgets sql

TARGET = assemblyHMIstandard
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
    ../bank-view/fronts/frontaxisparameter.cpp \
    ../bank-view/fronts/frontcommon.cpp \
    ../bank-view/fronts/frontcommonmanual.cpp \
    ../bank-view/fronts/frontconfigurationtransfer.cpp \
    ../bank-view/fronts/frontcylinderpanel.cpp \
    ../bank-view/fronts/frontmainpanel.cpp \
    ../bank-view/fronts/frontsinglefilter.cpp \
    ../bank-view/fronts/fronttwinfilter2.cpp \
    ../bank-view/fronts/frontunitpanel.cpp \
    ../bank-view/definitions/definitionauxiliarykeys.cpp \
    ../bank-view/definitions/definitionbasicblocks.cpp \
    ../bank-view/definitions/definitionsbaselayer.cpp \
    ../bank-view/helpers/commonhelper.cpp \
    ../bank-view/base/junctionbankdatabase.cpp \
    ../bank-view/base/utilities.cpp \
    ../bank-view/base/enumcomboboxdynamic.cpp \
    ../bank-view/helpers/loadinghelper.cpp \
    ../bank-view/controllers/controllerbanktransfer.cpp \
    ../bank-view/controllers/controllerbase.cpp \
    ../bank-view/controllers/controllermainpanel.cpp \
    ../bank-view/controllers/controllermanualmode.cpp \
    ../bank-view/interface/interfacechannel.cpp \
    ../bank-view/interface/interfaceclient.cpp \
    ../bank-view/helpers/loadinghelpercontrollers.cpp \
    ../bank-view/interface/adsclient.cpp \
    ../bank-view/controllers/controlleriomonitor.cpp \
    ../bank-view/fronts/frontiooverride.cpp \
    ../bank-view/delegates/delegatedataselector.cpp \
    ../bank-view/delegates/delegatematerialselector.cpp \
    ../bank-view/delegates/delegateviewitemfeaturemarker.cpp \
    ../bank-view/fronts/tableviewfeature.cpp \
    ../bank-view/fronts/tabwidgetoperationmode.cpp

HEADERS += \
        mainwindow.h \
    ../bank-view/fronts/frontaxisparameter.h \
    ../bank-view/fronts/frontcommon.h \
    ../bank-view/fronts/frontcommonmanual.h \
    ../bank-view/fronts/frontconfigurationtransfer.h \
    ../bank-view/fronts/frontcylinderpanel.h \
    ../bank-view/fronts/frontmainpanel.h \
    ../bank-view/fronts/frontsinglefilter.h \
    ../bank-view/fronts/fronttwinfilter2.h \
    ../bank-view/fronts/frontunitpanel.h \
    mainwindow.h \
    ../bank-view/definitions/definitionauxiliarykeys.h \
    ../bank-view/definitions/definitionaxisblocks.h \
    ../bank-view/definitions/definitionbasicblocks.h \
    ../bank-view/definitions/definitioncommandblock.h \
    ../bank-view/definitions/definitioncylinderblock.h \
    ../bank-view/definitions/definitionioattributes.h \
    ../bank-view/definitions/definitionmaincontrolblock.h \
    ../bank-view/definitions/definitionmanualblock.h \
    ../bank-view/definitions/definitionmaterialkeys.h \
    ../bank-view/definitions/definitionsbaselayer.h \
    ../bank-view/definitions/definitionsignalblock.h \
    ../bank-view/definitions/definitionslotblock.h \
    ../bank-view/definitions/definitionunitblocks.h \
    ../bank-view/helpers/commonhelper.h \
    ../bank-view/base/abstractsqltableadpater.h \
    ../bank-view/base/junctionbankdatabase.h \
    ../bank-view/base/utilities.h \
    ../bank-view/base/enumcomboboxdynamic.h \
    ../bank-view/base/enumcomboboxgeneric.h \
    ../bank-view/helpers/loadinghelper.h \
    ../bank-view/controllers/controllerbanktransfer.h \
    ../bank-view/controllers/controllerbase.h \
    ../bank-view/controllers/controllermainpanel.h \
    ../bank-view/controllers/controllermanualmode.h \
    ../bank-view/interface/interfacechannel.h \
    ../bank-view/interface/interfaceclient.h \
    ../bank-view/interface/interfacerequest.h \
    ../bank-view/helpers/loadinghelpercontrollers.h \
    ../bank-view/interface/adsclient.h \
    ../bank-view/controllers/controlleriomonitor.h \
    ../bank-view/fronts/frontiooverride.h \
    ../bank-view/delegates/delegatedataselector.h \
    ../bank-view/delegates/delegatematerialselector.h \
    ../bank-view/delegates/delegateviewitemfeaturemarker.h \
    ../bank-view/fronts/tableviewfeature.h \
    ../bank-view/definitions/definitionioattributes.h \
    ../bank-view/fronts/tabwidgetoperationmode.h

FORMS += \
        mainwindow.ui \
    ../bank-view/fronts/frontaxisparameter.ui \
    ../bank-view/fronts/frontconfigurationtransfer.ui \
    ../bank-view/fronts/frontcylinderpanel.ui \
    ../bank-view/fronts/frontmainpanel.ui \
    ../bank-view/fronts/frontsinglefilter.ui \
    ../bank-view/fronts/fronttwinfilter2.ui \
    ../bank-view/fronts/frontunitpanel.ui \
    ../bank-view/fronts/frontiooverride.ui

INCLUDEPATH += ../bank-view/fronts/ \
                ../bank-view/definitions/ \
                ../bank-view/helpers/ \
                ../bank-view/base/ \
                ../bank-view/controllers/ \
                ../bank-view/interface/ \
                ../bank-view/delegates/ \
                ../bank-view/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../build-assemblyHMIstandardWithController-Desktop_Qt_5_9_1_MinGW_32bit-Debug/widgetMain.qss \
    ../build-assemblyHMIstandardWithController-Desktop_Qt_5_9_1_MinGW_32bit-Debug/tabAxis.qss \
    ../build-assemblyHMIstandardWithController-Desktop_Qt_5_9_1_MinGW_32bit-Debug/tabCylinder.qss

win32: LIBS += -L$$PWD/../../../../../TwinCAT/AdsApi/TcAdsDll/Lib/ -lTcAdsDll

INCLUDEPATH += $$PWD/../../../../../TwinCAT/AdsApi/TcAdsDll/Include
DEPENDPATH += $$PWD/../../../../../TwinCAT/AdsApi/TcAdsDll/Include
