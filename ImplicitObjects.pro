#-------------------------------------------------
#
# Project created by QtCreator 2015-06-04T14:40:33
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ImplicitObjects
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    HashTable.cpp \
    MarchingCubes.cpp \
    GLData.cpp \
    Object.cpp

HEADERS += \
    HashTable.h \
    MarchingCubes.h \
    MCTables.h \
    GLData.h \
    Object.h


INCLUDEPATH += "/users/rsg/mmi/gmtl-0.6.1/build/include/gmtl-0.6.1/"


QMAKE_CXXFLAGS += -std=c++11
