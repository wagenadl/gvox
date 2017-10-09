# QMake project file for gvox                  -*- mode: shell-script; -*-


TEMPLATE = app
TARGET = gvox
CONFIG += debug
QT += widgets
CONFIG += c++11

HEADERS += Voxmap.h Viewer.h Transform3.h
SOURCES += Voxmap.cpp Viewer.cpp Transform3.cpp main.cpp
