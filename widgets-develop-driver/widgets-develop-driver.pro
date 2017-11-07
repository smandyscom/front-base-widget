TEMPLATE = subdirs

SUBDIRS += \
    mainviewer \
bank_viewer

bank_viewer.subdir = ../bank-view

mainviewer.depends=bank_viewer
