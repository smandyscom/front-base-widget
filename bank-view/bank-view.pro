#-------------------------------------------------
#
# Project created by QtCreator 2017-11-06T15:51:30
#
#-------------------------------------------------

QT       += widgets sql serialbus

#QT       -= gui

TARGET = bank-view
TEMPLATE = lib

DEFINES += BANKVIEW_LIBRARY

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
        bankview.cpp \
    ptpzrn_viewer.cpp \
    manualpanel.cpp

HEADERS += \
        bankview.h \
        bank-view_global.h \ 
    ptpzrn_viewer.h \
    manualpanel.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    ptpzrn_viewer.ui \
    manualpanel.ui