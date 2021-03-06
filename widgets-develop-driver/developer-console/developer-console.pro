QT += gui
QT += widgets
QT += serialbus

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    task.cpp \
    test1.cpp \
    ../../bank-view/baselayerdefinitions.cpp \
    testtask2.cpp \
    timersource.cpp \
    ../../bank-view/modbusserializedclient.cpp \
    ../../bank-view/modbuschannel.cpp \
    modbuschanneltest.cpp \
    ../../bank-view/utilities.cpp

HEADERS += \
    ../../bank-view/baselayerdefinitions.h \
    task.h \
    testtask2.h \
    timersource.h \
    ../../bank-view/modbusserializedclient.h \
    serializedclienttest.h \
    ../../bank-view/modbuschannel.h \
    ../../bank-view/utilities.h \
    ../../bank-view/basicblocksdefinition.h

INCLUDEPATH += ../../bank-view/
