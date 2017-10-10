# QMake project file for gvox                  -*- mode: shell-script; -*-


TEMPLATE = app
TARGET = gvox
CONFIG += release
QT += widgets
CONFIG += c++11

HEADERS += Voxmap.h Viewer.h Transform3.h IDmap.h DistinctColors.h
SOURCES += Voxmap.cpp Viewer.cpp Transform3.cpp IDmap.cpp DistinctColors.cpp
SOURCES += main.cpp
