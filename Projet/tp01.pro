#-------------------------------------------------
#
# Project created by QtCreator 2019-01-12T12:27:11
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

CONFIG += c++14
CONFIG += link_pkgconfig

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


DEFINES += QT_DEPRECATED_WARNINGS

TARGET = tp01
TEMPLATE = app

unix:!macx {
    PKGCONFIG += bullet
    LIBS += -lglut -lGLU
    LIBS += -L$$PWD/../OpenMesh/liblinux/ -lOpenMeshCore

    INCLUDEPATH += $$PWD/../OpenMesh/inc/
    DEPENDPATH += $$PWD/../OpenMesh/inc/
    DEPENDPATH += $$PWD/../OpenMesh/liblinux/
}

macx: {
    PKG_CONFIG = /usr/local/bin/pkg-config
    QT_CONFIG -= no-pkg-config
    CONFIG += link_pkgconfig
    PKGCONFIG += bullet
    INCLUDEPATH += $$PWD/../OpenMesh/inc/
    LIBS += -L$$PWD/../OpenMesh/libosx/ -lOpenMeshCore -lOpenMeshTools
}

SOURCES += main.cpp\
	boite.cpp \
        mesh.cpp \
        physics.cpp \
        princ.cpp \
        glarea.cpp \
        seedgenerator.cpp \
	geometry.cpp \
	line.cpp \
	plane.cpp \
	voronoi.cpp

HEADERS  += princ.h \
	boite.h \
	glarea.h \
	physics.h \
	line.h \
	plane.h \
	seedgenerator.h \
	MeshFormat.h \
	glarea.h \
	mesh.h \
	geometry.h \
	glarea.h \
	voronoi.h

FORMS    += princ.ui

RESOURCES += \
    tp01.qrc
