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
    ../bank-view/utilities.cpp

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
    ../bank-view/utilities.h \
    ../bank-view/abstractsqltableadpater.h

FORMS += \
        mainwindow.ui \
    ../bank-view/fronts/frontaxisparameter.ui \
    ../bank-view/fronts/frontconfigurationtransfer.ui \
    ../bank-view/fronts/frontcylinderpanel.ui \
    ../bank-view/fronts/frontmainpanel.ui \
    ../bank-view/fronts/frontsinglefilter.ui \
    ../bank-view/fronts/fronttwinfilter2.ui \
    ../bank-view/fronts/frontunitpanel.ui

INCLUDEPATH += ../bank-view/fronts/ \
                ../bank-view/definitions/ \
                ../bank-view/helpers/ \
                ../bank-view/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
