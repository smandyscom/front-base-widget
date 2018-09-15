QT += testlib widgets sql
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_testadaptor1.cpp \
    ../bank-view/definitionbasicblocks.cpp \
    ../bank-view/utilities.cpp \
    ../bank-view/helpers/commonhelper.cpp

HEADERS += \
    ../bank-view/abstractsqltableadpater.h \
    ../bank-view/definitionbasicblocks.h \
    ../bank-view/utilities.h \
    ../bank-view/helpers/commonhelper.h \
    ../bank-view/definitioncommandblock.h

INCLUDEPATH += ../bank-view/ \
                ../bank-view/helpers/
