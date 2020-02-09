#-------------------------------------------------
#
# Project created by QtCreator 2020-01-27T03:11:44
#
#-------------------------------------------------

QT       += core gui
#QT += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = ssdb
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += QCUSTOMPLOT_USE_OPENGL

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    infplt.cpp \
        main.cpp \
        mainwindow.cpp \
    infprg.cpp \
    qcustomplot/qcustomplot.cpp

HEADERS += \
    infplt.h \
        mainwindow.h \
    infprg.h \
    qcustomplot/qcustomplot.h \
    fast-cpp-csv-parser-master/csv.h

FORMS += \
    infplt.ui \
        mainwindow.ui \
    infprg.ui

RESOURCES += \
    ssdb.qrc

TRANSLATIONS += \
    language_ru_RU.ts

CODECFORSRC = UTF-8

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
