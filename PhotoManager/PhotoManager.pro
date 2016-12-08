#-------------------------------------------------
#
# Project created by QtCreator 2016-07-27T20:43:23
#
#-------------------------------------------------

QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PhotoManager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    databasemanager.cpp \
    tableabstractelem.cpp \
    formworksystem.cpp \
    categorie.cpp \
    feature.cpp \
    filemanager.cpp \
    configurator.cpp \
    sqlservermanager.cpp \
    filecopierthread.cpp \
    projectname.cpp \
    previewworker.cpp \
    previewsession.cpp \
    photoloader.cpp \
    interfacemanager.cpp \
    tableabstractelemmanager.cpp \
    photosearcher.cpp \
    attributeseditdialog.cpp

HEADERS  += mainwindow.h \
    databasemanager.h \
    tableabstractelem.h \
    formworksystem.h \
    categorie.h \
    feature.h \
    filemanager.h \
    configurator.h \
    sqlservermanager.h \
    filecopierthread.h \
    projectname.h \
    previewworker.h \
    previewsession.h \
    photoloader.h \
    interfacemanager.h \
    tableabstractelemmanager.h \
    photosearcher.h \
    fileinfostruct.h \
    attributeseditdialog.h

FORMS    += mainwindow.ui \
    attributeseditdialog.ui

TRANSLATIONS = mainwindow_en.ts mainwindow_ru.ts
