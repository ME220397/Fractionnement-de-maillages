#-------------------------------------------------
#
# Project created by QtCreator 2019-01-12T12:27:11
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


DEFINES += QT_DEPRECATED_WARNINGS

TARGET = tp01
TEMPLATE = app

unix:!macx {
    LIBS += -lglut -lGLU
    LIBS += -L$$PWD/../OpenMesh/liblinux/ -lOpenMeshCore

    INCLUDEPATH += $$PWD/../OpenMesh/inc/
    DEPENDPATH += $$PWD/../OpenMesh/inc/
    DEPENDPATH += $$PWD/../OpenMesh/liblinux/
}

macx: {
    INCLUDEPATH += $$PWD/../OpenMesh/inc/
    LIBS += -L$$PWD/../OpenMesh/libosx/ -lOpenMeshCore -lOpenMeshTools
}

SOURCES += main.cpp\
        princ.cpp \
        glarea.cpp

HEADERS  += princ.h \
        glarea.h

FORMS    += princ.ui

RESOURCES += \
    tp01.qrc
