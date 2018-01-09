TEMPLATE = subdirs

SUBDIRS += \
    mainviewer \
bank_viewer \
    developer-console

bank_viewer.subdir = ../bank-view

mainviewer.depends=bank_viewer
