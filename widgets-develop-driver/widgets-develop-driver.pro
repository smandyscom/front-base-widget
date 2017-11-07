TEMPLATE = subdirs

SUBDIRS += \
    mainviewer \
bank_viewer

bank_viewer.subdir = ..\bank-view

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../bank-view/release/ -lbank-view
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../bank-view/debug/ -lbank-view
else:unix: LIBS += -L$$OUT_PWD/../bank-view/ -lbank-view

INCLUDEPATH += $$PWD/../bank-view
DEPENDPATH += $$PWD/../bank-view
