CONFIG      += plugin debug_and_release
TARGET      = $$qtLibraryTarget(bank_viewerplugin)
TEMPLATE    = lib

HEADERS     = bank_viewerplugin.h
SOURCES     = bank_viewerplugin.cpp
RESOURCES   = icons.qrc
LIBS        += -L. -lbank_viewer

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += designer
} else {
    CONFIG += designer
}

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target


