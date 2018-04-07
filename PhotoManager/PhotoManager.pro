#-------------------------------------------------
#
# Project created by QtCreator 2016-07-27T20:43:23
#
#-------------------------------------------------

QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PhotoManager
TEMPLATE = app

INCLUDEPATH += exiv2/include

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
    attributeseditdialog.cpp \
    bigpreview.cpp \
    jpegfilecopier.cpp \
    aboutwindow.cpp \
    confirmdeletedialog.cpp \
    passwordmanager.cpp \
    resultcontextmenu.cpp \
    imageprinter.cpp \
    page.cpp \
    additionpage.cpp \
    editdbpage.cpp \
    searchpage.cpp \
    busyindicator.cpp

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
    attributeseditdialog.h \
    bigpreview.h \
    jpegfilecopier.h \
    aboutwindow.h \
    confirmdeletedialog.h \
    passwordmanager.h \
    resultcontextmenu.h \
    imageprinter.h \
    page.h \
    additionpage.h \
    editdbpage.h \
    searchpage.h \
    busyindicator.h

FORMS    += mainwindow.ui \
    attributeseditdialog.ui \
    bigpreview.ui \
    aboutwindow.ui \
    confirmdeletedialog.ui

TRANSLATIONS = mainwindow_en.ts mainwindow_ru.ts

LIB_PATH = $$PWD/exiv2/lib

CONFIG(debug, debug|release) {
    LIBS     += -L$$LIB_PATH/debug \
                -llibexiv2 \
                -llibexpat \
                -lxmpsdk \
                -lzlib1
    }

CONFIG(release, debug|release) {
    LIBS     += -L$$LIB_PATH/release \
                -llibexiv2 \
                -llibexpat \
                -lxmpsdk \
                -lzlib1
}

win32: RC_ICONS = periphotoproject.ico

RESOURCES += \
    photomangerres.qrc
