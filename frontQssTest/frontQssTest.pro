#-------------------------------------------------
#
# Project created by QtCreator 2018-09-21T21:36:12
#
#-------------------------------------------------

QT       += core gui widgets sql

TARGET = frontQssTest
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
    form.cpp \
    ../bank-view/fronts/frontcommon.cpp \
    ../bank-view/helpers/commonhelper.cpp \
    ../bank-view/controllerbase.cpp \
    ../bank-view/interfacechannel.cpp \
    ../bank-view/interfaceclient.cpp \
    ../bank-view/utilities.cpp \
    ../bank-view/definitions/definitionsbaselayer.cpp

HEADERS += \
        mainwindow.h \
    form.h \
    ../bank-view/fronts/frontcommon.h \
    ../bank-view/helpers/commonhelper.h \
    ../bank-view/controllerbase.h \
    ../bank-view/interfacechannel.h \
    ../bank-view/interfaceclient.h \
    ../bank-view/interfacerequest.h \
    ../bank-view/definitions/definitionbasicblocks.h \
    ../bank-view/utilities.h \
    ../bank-view/definitions/definitionsbaselayer.h

FORMS += \
        mainwindow.ui \
    form.ui

INCLUDEPATH += ../bank-view \
                ../bank-view/fronts/ \
                ../bank-view/helpers/ \
                ../bank-view/definitions/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../build-frontQssTest-Desktop_Qt_5_9_1_clang_64bit-Debug/frontQssTest.app \
    centralWidget.qss
